/**
 *  @file       csv.c
 *  @brief      Source file for CSV utilities
 *
 *  @author     Gemuele Aludino
 *  @date       02 Jan 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "csv.h"
//#include "slist.h"
#include "list.h"
#include "utils.h"
#include "vector_int.h"

#define TOKENIZER_BUFFER_SIZE   1024    // 1.00 MB
#define TOKENIZER_RESERVE_SIZE  256     // 0.25 MB

#define TYPE_COUNT      3

struct tokenizer {
    FILE *file;             /**< FILE stream to tokenize */

    int delimiter;          /**< assigned delimiter character */
    int override;           /**< assigned override character */

    bool override_mode;     /**< if true, delimiter chars are treated as data */

    bool end_of_line;       /**< flag to signify end of row */
    bool end_of_file;       /**< flag to signify end of file reading */

    int pos;                /**< index of current char buffer position */

    char ch;                      /**< current character read from stream */

    //char buffer[TOKENIZER_BUFFER_SIZE];     /**< stack buffer for char stream */

    vector(char) *buffer;                 /*<< dynamic buffer for char stream */
};

/**< tokenizer: allocation/initialization/deinitialization */
static  tokenizer *     tk_allocate     (void);
static  void            tk_init         (tokenizer *tk, const char *filename, FILE *file);
static  void            tk_deinit       (tokenizer *tk);

tokenizer *tk_new(const char *filename) {
    tokenizer *tk = tk_allocate();
    tk_init(tk, filename, NULL);
    return tk;
}

tokenizer *tk_new_file(FILE *file) {
    tokenizer *tk = tk_allocate();
    tk_init(tk, NULL, file);
    return tk;
}

void tk_delete(tokenizer **tk) {
    assert(tk);

    tk_deinit((*tk));
    free((*tk));
    (*tk) = NULL;
}

void tk_set_delimiter(tokenizer *tk, const int delimiter) {
    assert(tk);
    tk->delimiter = delimiter;
}

void tk_set_override(tokenizer *tk, const int override) {
    assert(tk);
    tk->override = override;
}

char *tk_next(tokenizer *tk) {
    assert(tk);

    if (tk->end_of_file) {
        return NULL;
    }

    tk->pos = 0;

    while (tk->end_of_file == false) {
        if (feof(tk->file)) {
            tk->end_of_line = true;
            tk->end_of_file = true;
            return NULL;
        }

        tk->ch = fgetc(tk->file);

        bool ch_over = tk->ch == tk->override;
        bool ch_dlim = tk->ch == tk->delimiter;
        bool ch_endl = tk->ch == '\n';
        bool ch_ntrm = tk->ch == '\0';

        if (tk->override_mode) {
            if (ch_over) {
                tk->override_mode = false;
            } else {
                if (ch_ntrm) {
                    tk->end_of_line = true;
                    tk->end_of_file = true;
                    break;
                }
            }
        } else {
            if (ch_over) {
                tk->override_mode = true;
            } else if (ch_dlim || ch_endl || ch_ntrm) {
                if (ch_endl) {
                    tk->end_of_line = true;
                } else if (ch_ntrm) {
                    tk->end_of_line = true;
                    tk->end_of_file = true;
                }

                break;
            }
        }

        /*

         if (tk->ch == tk->delimiter) {
         ulog(stdout, KRED"[dlim]"KNRM, "tk->pos", "\tfgetc(tk->file) \t",
         (long long int)(tk->pos),
         "%c\t\toverride_mode: %s",
         tk->ch, tk->override_mode ? "on" : "off");
         } else if (tk->ch == '\n') {
         ulog(stdout, KCYN"[endl]"KNRM, "tk->pos", "\tfgetc(tk->file) \t",
         (long long int)(tk->pos),
         "%c\t\toverride_mode: %s",
         ' ', tk->override_mode ? "on" : "off");
         } else if (tk->ch == '\0') {
         ulog(stdout, KRED"[ntrm]"KNRM, "tk->pos", "\tfgetc(tk->file) \t",
         (long long int)(tk->pos),
         "%c\t\toverride_mode: %s",
         tk->ch, tk->override_mode ? "on" : "off");
         } else if (tk->ch == tk->override) {
         ulog(stdout, KYEL"[over]"KNRM, "tk->pos", "\tfgetc(tk->file) \t",
         (long long int)(tk->pos),
         "%c\t\toverride_mode: %s",
         tk->ch, tk->override_mode ? "on" : "off");
         } else {
         ulog(stdout, "[char]", "tk->pos", "\tfgetc(tk->file) \t",
         (long long int)(tk->pos),
         "%c\t\toverride_mode: %s",
         tk->ch, tk->override_mode ? "on" : "off");
         }

         //*/

        /* FOR STACK BUFFER
        tk->buffer[tk->pos++] = tk->ch;
        //*/

        //* FOR vector(char) BUFFER
         vpushb(char)(tk->buffer, tk->ch);
         ++tk->pos;
         //*/
    }

    /*
     ulog(stdout, KHIL"[TKEN]"KNRM, "strlen", KWHT_b"\ttk->buffer\t\t"KNRM,
     (long long int)(tk->pos), "token: %s", tk->buffer);
     //*/

    /* FOR STACK BUFFER
    tk->buffer[tk->pos] = '\0';
    return strdup(tk->buffer);
    //*/

    //* FOR vector(char) BUFFER
     vpushb(char)(tk->buffer, '\0');
     char *ret = strdup(*vdata(char)(tk->buffer));
     vclear(char)(tk->buffer);

     return ret;
     //*/
}

bool tk_has_next(tokenizer *tk) {
    assert(tk);
    return tk->end_of_file == false;
}

bool tk_endl(tokenizer *tk) {
    assert(tk);
    bool result = tk->end_of_line;
    tk->end_of_line = result ? false : tk->end_of_line;
    return result;
}

static tokenizer *tk_allocate() {
    tokenizer *tk = NULL;
    tk = malloc(sizeof(struct tokenizer));
    assert(tk);
    return tk;
}

static void tk_init(tokenizer *tk, const char *filename, FILE *file) {
    if (file == NULL) {
        FILE *file = NULL;
        file = fopen(filename, "r");

        if (file == NULL) {
            ERROR(__FILE__, "Unable to open \"%s\"", filename);
            return;
        }

        tk->file = file;
    } else if (filename == NULL) {
        tk->file = file;
    }

    tk->delimiter = ',';
    tk->override = '"';

    tk->override_mode = false;

    tk->end_of_file = false;
    tk->end_of_line = false;

    tk->pos = 0;
    tk->ch = '\0';

    //* FOR vector(char) BUFFER
     tk->buffer = vnewr(char)(TOKENIZER_RESERVE_SIZE);
     //*/
}

static void tk_deinit(tokenizer *tk) {
    /**
     *  If the client created an instance of tokenizer
     *  using tk_new (providing a string to a file in secondary storage)
     *  -- the FILE * pointed to by tk->file was opened by tokenizer,
     *  and therefore will be closed by tokenizer.
     *
     *  If the client created an instance of tokenizer
     *  using tk_new_file (providing their own FILE * instance)
     *  -- the FILE * pointed to by tk->file does not belong to tokenizer,
     *  and therefore will not be closed by tokenizer.
     *  (it will be the client's responsibility to close their FILE *)
     */
    if (tk->file != NULL) {
        fclose(tk->file);
    }

    tk->pos = -1;
    tk->ch = '\0';

    //*
     vdelete(char)(&tk->buffer);
     //*/
}




/*
struct tokenizer *tokenizer_init(FILE *file) {
    struct tokenizer *tk = NULL;
    tk = malloc(sizeof(tokenizer));
    assert(tk);

    tk->file = file;

    tk->delimiter = ',';
    tk->override = '"';

    tk->first_char_of_cell = true;
    tk->first_char_of_line = true;

    tk->override_mode = false;

    tk->end_of_file = false;
    tk->end_of_cell = false;
    tk->end_of_line = false;

    tk->pos = 0;

    tk->ch = 0;

    return tk;
}

struct tokenizer *tokenizer_init_complete(FILE *file, const int delimiter,
                                          const int override) {
    struct tokenizer *tk = NULL;
    tk = malloc(sizeof(tokenizer));
    assert(tk);

    tk->file = file;

    tk->delimiter = delimiter;
    tk->override = override;

    tk->override_mode = false;
    tk->first_char_of_cell = true;
    tk->first_char_of_line = true;
    tk->end_of_file = false;
    tk->end_of_cell = false;
    tk->end_of_line = false;

    tk->pos = 0;

    tk->ch = 0;

    return tk;
}

void tokenizer_deinit(void *arg) {
    struct tokenizer *tk = (tokenizer *)(arg);

    tk->override_mode = false;
    tk->first_char_of_cell = false;
    tk->first_char_of_line = false;
    tk->end_of_file = true;
    tk->end_of_cell = true;
    tk->end_of_line = true;

    tk->pos = -1;
    tk->ch = 0;

    free(tk);
    tk = NULL;
}

void tokenizer_set_delimiter(struct tokenizer *tk, const int delimiter) {
    tk->delimiter = delimiter;
}

void tokenizer_set_override(struct tokenizer *tk, const int override) {
    tk->override = override;
}

char *tokenizer_get_next_token(struct tokenizer *tk) {
    if (tk->end_of_file) {
        return NULL;
    }

    // bzero(tk->buffer, tk->pos);
    tk->pos = 0;

    bool ok_to_append = false;

    while (tk->end_of_file == false) {
        if (feof(tk->file)) {
            tk->first_char_of_cell = false;
            tk->first_char_of_line = false;
            tk->end_of_cell = true;
            tk->end_of_line = true;
            tk->end_of_file = true;
            return NULL;
        }

        tk->ch = fgetc(tk->file);

        if (tk->first_char_of_cell) {
            tk->first_char_of_cell = false;

            if (tk->ch == tk->override) {
                tk->override_mode = true;

                ok_to_append = true;
            } else if (tk->ch == tk->delimiter) {
                tk->end_of_cell = true;
                tk->first_char_of_line = false;
                break;
            } else if (tk->ch == '\n') {
                if (tk->first_char_of_line == false) {
                    tk->first_char_of_cell = true;
                    tk->first_char_of_line = true;
                    tk->end_of_cell = true;
                    tk->end_of_line = true;
                    break;
                }
            } else {
                ok_to_append = true;
            }
        } else if (tk->first_char_of_cell == false && tk->override_mode) {
            if (tk->ch == tk->override) {
                tk->buffer[tk->pos++] = tk->ch;

                tk->ch = fgetc(tk->file);

                if (tk->ch != tk->override) {
                    tk->override_mode = false;

                    if (tk->ch == tk->delimiter) {
                        tk->first_char_of_cell = true;
                        tk->end_of_cell = true;
                        break;
                    } else if (tk->ch == '\n') {
                        tk->first_char_of_cell = true;
                        tk->first_char_of_line = true;
                        tk->end_of_cell = true;
                        tk->end_of_line = true;
                        break;
                    } else if (tk->ch == '\0') {
                        tk->end_of_cell = true;
                        tk->end_of_line = true;
                        tk->end_of_file = true;
                        break;
                    } else {
                        ok_to_append = true;
                    }
                } else {
                    while (tk->ch == tk->override) {
                        tk->buffer[tk->pos++] = tk->ch;
                        tk->ch = fgetc(tk->file);
                    }

                    tk->override_mode = false;

                    if (tk->ch == tk->delimiter) {
                        tk->first_char_of_cell = true;
                        tk->end_of_cell = true;
                        break;
                    } else if (tk->ch == '\n') {
                        tk->first_char_of_cell = true;
                        tk->first_char_of_line = true;
                        tk->end_of_cell = true;
                        tk->end_of_line = true;
                        break;
                    } else if (tk->ch == '\0') {
                        tk->end_of_cell = true;
                        tk->end_of_line = true;
                        tk->end_of_file = true;
                        break;
                    } else {
                        ok_to_append = true;
                    }
                }
            }
        } else {
            if (tk->ch == tk->delimiter) {
                tk->first_char_of_cell = true;
                tk->end_of_cell = true;
                ok_to_append = true;
                break;
            } else {
                ok_to_append = true;

                if (tk->ch == '\n') {
                    tk->first_char_of_cell = true;
                    tk->first_char_of_line = true;
                    tk->end_of_cell = true;
                    tk->end_of_line = true;
                    break;
                } else if (tk->ch == '\0') {
                    tk->end_of_cell = true;
                    tk->end_of_line = true;
                    tk->end_of_file = true;
                    break;
                }
            }
        }

        tk->buffer[tk->pos++] = tk->ch;
    }

    tk->buffer[tk->pos] = '\0';

    // LOG(__FILE__, "cell: %s", tk->buffer);
    return strdup(tk->buffer);
}

char *tokenizer_get_next_token(tokenizer *tk) {
    bzero(tk->buffer, tk->pos);
    tk->pos = 0;

    tk->first_char_of_cell = true;
    tk->end_of_file = false;
    tk->end_of_cell = false;
    tk->end_of_line = false;

    while (tk->end_of_file == false) {
        if (feof(tk->file)) {
            tk->end_of_cell = true;
            tk->end_of_line = true;
            tk->end_of_file = true;
            return NULL;
        }

        tk->ch = fgetc(tk->file);

        if (tk->first_char_of_cell) {
            if (tk->ch == tk->override) {
                tk->override_mode = true;

                tk->buffer[tk->pos++] = tk->ch;
                tk->first_char_of_cell = false;
            } else if (tk->ch == tk->delimiter) {
                tk->end_of_cell = true;
                tk->first_char_of_line = false;
                break;
            } else if (tk->ch == '\n') {
                if (tk->first_char_of_line) {

                } else {
                    tk->buffer[tk->pos++] = tk->ch;
                    tk->first_char_of_cell = false;
                    tk->first_char_of_line = true;
                    tk->end_of_cell = true;
                    tk->end_of_line = true;
                    break;
                }
            } else {
                tk->buffer[tk->pos++] = tk->ch;
                tk->first_char_of_cell = false;
            }
        } else if (tk->override_mode) {
            // bool previous_override = false;

            if (tk->ch == tk->override) {
                tk->buffer[tk->pos++] = tk->ch;
                tk->ch = fgetc(tk->file);

                LOG("", "");

                // previous_override = true;

                while (tk->ch == tk->override) {
                    tk->buffer[tk->pos++] = tk->ch;
                    tk->ch = fgetc(tk->file);
                }

                if (tk->ch == tk->delimiter) {
                    tk->override_mode = false;
                    tk->end_of_cell = true;
                    break;
                } else if (tk->ch == '\n') {
                    tk->override_mode = false;
                    // tk->end_of_cell = true;
                    // tk->end_of_line = true;
                    tk->buffer[tk->pos++] = tk->ch;
                    // break;
                } else {
                    // if (previous_override == false) {
                    // tk->override_mode = false;
                    //}
                    tk->override_mode = false;
                    tk->buffer[tk->pos++] = tk->ch;
                }
            } else {
                tk->buffer[tk->pos++] = tk->ch;
            }
        } else {
            if (tk->ch == tk->delimiter) {
                tk->end_of_cell = true;
                break;
            } else {
                tk->buffer[tk->pos++] = tk->ch;

                if (tk->ch == '\n' || tk->ch == '\0') {
                    tk->end_of_cell = true;
                    tk->end_of_line = true;
                    tk->first_char_of_line = true;

                    break;
                }
            }
        }
    }

    tk->buffer[tk->pos] = '\0';
    // return strdup(str_trim_right(tk->buffer, "\n"));
    printf("cell: ");
    printf("%s\n", tk->buffer);
    return tk->buffer;
}


bool tokenizer_has_next_token(struct tokenizer *tk) { return !tk->end_of_file; }

bool tokenizer_end_of_line(struct tokenizer *tk) { return tk->end_of_line; }

bool tokenizer_end_of_cell(struct tokenizer *tk) { return tk->end_of_cell; }

record *record_init(tokenizer *tk, size_t max_col, int sort_value_index) {
    record *r = NULL;
    r = malloc(sizeof(record));
    assert(r);

    r->cells = NULL;
    r->cells = vector_init(sizeof(char *));
    vector_set_comparator(r->cells, compare_pointer_to_char);
    vector_set_printfunc(r->cells, print_pointer_to_char);

    vector_reserve(r->cells, max_col);

    if (tk == NULL) {
        return r;
    }

    while (true) {
        char *token = tokenizer_get_next_token(tk);

        if (vector_size(r->cells) + 1 > max_col) {
            char message[4096];

            int j = 0;
            j += sprintf(message + j, "Too many cells in this record:\n");
            for (int i = 0; i < vector_size(r->cells); i++) {
                char *ch = *(char **)(vector_at(r->cells, i));
                j += sprintf(message + j, "\t%s\n", ch);
            }

            j += sprintf(message + j, "\t==> %s\n\n", token);
            j += sprintf(message + j, "Cell at ==> causes overflow.\n");
            j += sprintf(message + j, "Max cells permitted: %zu.", max_col);

            error_func(__FILE__, message, false);

            free(token);
            token = NULL;
            free(r->cells);
            r->cells = NULL;
            free(r);
            r = NULL;

            return NULL;
        }

        if (token != NULL) {
            char *to_push = strdup(token);
            vector_push_back(r->cells, &to_push);

            free(token);
            token = NULL;
        } else {
            return NULL;
        }

        if (tk->end_of_line) {
            break;
        }
    }

    if (sort_value_index == -1) {
        r->sort_value = NULL;
        r->data_type = UNSPECIFIED;
    } else {
        record_set_data_type(r, sort_value_index);
    }

    return r;
}

record *record_init_copy(record *tcpy) {
    record *r = NULL;
    r = malloc(sizeof(record));
    assert(r);

    memcpy(r, tcpy, sizeof(record));
    return r;
}

void record_deinit(void *arg) {
    record *r = (record *)(arg);

    vector_deinit(r->cells);
    r->cells = NULL;
    free(r);
    r = NULL;
}

void record_set_data_type(record *r, size_t sort_value_index) {
    if (sort_value_index == -1) {
        r->sort_value = NULL;
        r->data_type = UNSPECIFIED;

        return;
    }

    r->sort_value = *(char **)(vector_at(r->cells, sort_value_index));

    char temp[strlen(r->sort_value) + 1];
    strcpy(temp, r->sort_value);
    string_trim(temp, NULL);

    char *decimal_point = strchr(temp, '.');

    const bool is_integer =
    !decimal_point && sscanf(temp, "%ld", &(r->long_int_value)) == 1;

    const bool is_double =
    decimal_point && sscanf(temp, "%lf", &(r->double_value)) == 1;

    const bool is_empty = strlen(temp) <= 0;

    if (is_integer) {
        r->data_type = LONG;
        r->double_value = r->long_int_value;
    } else if (is_double) {
        r->data_type = DOUBLE;
    } else if (is_empty) {
        r->data_type = EMPTY;
    } else {
        r->data_type = STRING;
    }
}

int compare_record(void *c1, void *c2) {
    record *r1 = (record *)(c1);
    record *r2 = (record *)(c2);

    enum identifier r1_type = r1->data_type;
    enum identifier r2_type = r2->data_type;

    int outcome = -99999;

    if (r1_type == r2_type) {
        if (r1_type == LONG) {
            long int long_int_1 = r1->long_int_value;
            long int long_int_2 = r2->long_int_value;

            outcome = compare_long_int(&long_int_1, &long_int_2);
        } else if (r1_type == DOUBLE) {
            double double_1 = r1->double_value;
            double double_2 = r2->double_value;

            outcome = compare_double(&double_1, &double_2);
        } else if (r1_type == STRING) {
            char ch1[strlen(r1->sort_value) + 1];
            char ch2[strlen(r2->sort_value) + 1];

            strcpy(ch1, r1->sort_value);
            strcpy(ch2, r2->sort_value);

            string_trim(ch1, NULL);
            string_trim(ch2, NULL);

            outcome = strcmp(ch1, ch2);
        } else if (r1_type == EMPTY) {
            outcome = -1;
        }
    } else {
        char ch1[strlen(r1->sort_value) + 1];
        char ch2[strlen(r2->sort_value) + 1];

        strcpy(ch1, r1->sort_value);
        strcpy(ch2, r2->sort_value);

        string_trim(ch1, NULL);
        string_trim(ch2, NULL);

        outcome = strcmp(ch1, ch2);
    }

    return outcome;
}

void print_record(void *arg, FILE *dest) {
    record *r = NULL;
    r = (record *)(arg);

    vector *inner = NULL;
    inner = r->cells;

    vector_p_print(inner);

    printf("RECORD SORT VALUE: %s\n", r->sort_value);

    switch (r->data_type) {
        case STRING:
            fprintf(dest, "SORT VALUE TYPE  : %s\n\n", "STRING");
            break;
        case LONG:
            fprintf(dest, "SORT VALUE TYPE  : %s\n\n", "LONG");
            break;
        case DOUBLE:
            fprintf(dest, "SORT VALUE TYPE  : %s\n\n", "DOUBLE");
            break;
        case EMPTY:
            fprintf(dest, "%s\n\n", "EMPTY");
            break;
        default:
            fprintf(dest, "%s\n\n", "(no type specified)");
            break;
    }
}

void vector_print_csv(FILE *dest, vector *col, vector *rec) {
    if (vector_empty(col)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < vector_size(col); i++) {
        char *cell = *(char **)(vector_at(col, i));

        fprintf(dest, "%s", cell);

        if (i != vector_size(col) - 1) {
            fprintf(dest, "%s", ",");
        } else {
            fprintf(dest, "%s", "\n");
        }
    }

    if (vector_empty(rec)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < vector_size(rec); i++) {
        record *r = (record *)(vector_at(rec, i));

        for (int j = 0; j < vector_size(r->cells); j++) {
            char *cell = *(char **)(vector_at(r->cells, j));

            fprintf(dest, "%s", cell);

            if (j != vector_size(r->cells) - 1) {
                fprintf(dest, "%s", ",");
            } else {
                fprintf(dest, "%s", "\n");
            }
        }
    }
}

void list_print_csv(FILE *dest, list *col, list *rec) {
    if (list_empty(col)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < list_size(col); i++) {
        char *cell = *(char **)(list_at(col, i));

        fprintf(dest, "%s", cell);

        if (i != list_size(col) - 1) {
            fprintf(dest, "%s", ",");
        } else {
            fprintf(dest, "%s", "\n");
        }
    }

    if (list_empty(rec)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < list_size(rec); i++) {
        record *r = (record *)(list_at(rec, i));

        for (int j = 0; j < vector_size(r->cells); j++) {
            char *cell = *(char **)(vector_at(r->cells, j));

            fprintf(dest, "%s", cell);

            if (j != vector_size(r->cells) - 1) {
                fprintf(dest, "%s", ",");
            } else {
                fprintf(dest, "%s", "\n");
            }
        }
    }
}

void slist_print_csv(FILE *dest, slist *col, slist *rec) {
    if (slist_empty(col)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < slist_size(col); i++) {
        char *cell = *(char **)(slist_at(col, i));

        fprintf(dest, "%s", cell);

        if (i != slist_size(col) - 1) {
            fprintf(dest, "%s", ",");
        } else {
            fprintf(dest, "%s", "\n");
        }
    }

    if (slist_empty(rec)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < slist_size(rec); i++) {
        record *r = (record *)(slist_at(rec, i));

        for (int j = 0; j < vector_size(r->cells); j++) {
            char *cell = *(char **)(vector_at(r->cells, j));

            fprintf(dest, "%s", cell);

            if (j != vector_size(r->cells) - 1) {
                fprintf(dest, "%s", ",");
            } else {
                fprintf(dest, "%s", "\n");
            }
        }
    }
}

csv_t *csv_t_init() {
    csv_t *csv = NULL;
    csv = malloc(sizeof(csv_t));
    assert(csv);

    csv->col = NULL;
    csv->rec = NULL;
    csv->sort_key_index = -1;

    csv->col = vector_init_complete(sizeof(char *), compare_pointer_to_char,
                                    print_pointer_to_char, NULL);

    csv->rec = vector_init_complete(sizeof(record), compare_record,
                                    print_record, record_deinit);

    return csv;
}

void csv_t_deinit(void *arg) {
    csv_t *csv = (csv_t *)(arg);

    assert(csv);

    csv->sort_key_index = -1;

    vector_deinit(csv->rec);
    csv->rec = NULL;

    vector_deinit(csv->col);
    csv->col = NULL;

    free(csv);
    csv = NULL;
}

void csv_t_import_file(csv_t *csv, const char *source_file,
                       const char *sort_key) {
    assert(csv);

    FILE *file = NULL;
    file = fopen(source_file, "r");

    if (file == NULL) {
        char message[256];
        int j = 0;
        j += sprintf(message + j, "Source file %s not found.\n", source_file);
        j += sprintf(message + j, "Please check the filename and try again.");
        error_func(__FILE__, message, false);
        return;
    }

    tokenizer *tk = NULL;
    tk = tokenizer_init(file);

    while (!tokenizer_end_of_line(tk)) {
        char *token = tokenizer_get_next_token(tk);
        if (token != NULL) {
            char *to_push = strdup(token);
            vector_push_back(csv->col, &to_push);
            free(token);
            token = NULL;
        } else {
            break;
        }
    }

    vector_shrink_to_fit(csv->col);
    const size_t max_col = vector_size(csv->col);

    csv_t_set_sort_key(csv, sort_key);

    while (true) {
        record *r = record_init(tk, max_col, csv->sort_key_index);
        if (r != NULL) {
            vector_push_back(csv->rec, r);
        } else {
            if (!tokenizer_has_next_token(tk)) {
                break;
            }
        }
    }

    fclose(file);
    tokenizer_deinit(tk);
    tk = NULL;

    vector_shrink_to_fit(csv->rec);
}

void csv_t_import_header(csv_t *csv, const char *strarr[], size_t max_col,
                         const char *sort_key) {
    assert(csv);

    for (int i = 0; i < max_col; i++) {
        char *to_push = strdup(strarr[i]);
        vector_push_back(csv->col, &to_push);
    }

    if (sort_key == NULL) {
        return;
    }

    csv_t_set_sort_key(csv, sort_key);
}

void csv_t_import_record_strarr(csv_t *csv, const char *strarr[],
                                size_t max_col) {
    if (max_col > vector_size(csv->col)) {
        char message[4096];
        int j = 0;

        j += sprintf(message + j, "Param max_col != column size.\n\n");
        j += sprintf(message + j, "Populate column to size max_col\n");
        j += sprintf(message + j, "and import an array of strings whose\n");
        j += sprintf(message + j, "element count corresponds with max_col.");

        error_func(__FILE__, message, false);
        return;
    }

    record *r = NULL;
    r = record_init(NULL, max_col, csv->sort_key_index);

    for (int i = 0; i < max_col; i++) {
        vector_push_back(r->cells, &strarr[i]);
    }

    if (csv->sort_key_index == -1) {
        r->sort_value = NULL;
        r->data_type = UNSPECIFIED;
    } else {
        record_set_data_type(r, csv->sort_key_index);
    }

    vector_push_back(csv->rec, r);
}

void csv_t_aggregate(csv_t *dest, csv_t *source) {

    // vector *c = csv->col;
    vector *r = csv->rec;

    const size_t r_size = vector_size(r);

    for (int i = 0; i < r_size; i++) {
        record *ch_rec = (record *)(vector_at(r, i));
        vector *cells = ch_rec->cells;

        const size_t master_col_size = vector_size(master->col);

        record *conformed_rec = record_init(NULL, master_col_size, -1);

        for (int j = 0; j < master_col_size; j++) {
            char *ch_master_col = *(char **)(vector_at(master->col, j));

            void *val = &ch_master_col;
            int ch_master_col_match = vector_p_linear_search(master->col, val);

            const bool match_for_master = ch_master_col_match != -1;

            if (match_for_master) {
                char *cell = *(char **)(vector_at(cells, ch_master_col_match));

                vector_push_back(conformed_rec->cells, &cell);
            }
        }

        record_set_data_type(conformed_rec, master->sort_key_index);
        vector_push_back(master->rec, conformed_rec);
    }

    assert(dest);
    assert(source);

    const size_t r_size = vector_size(source->rec);

    for (int i = 0; i < r_size; i++) {
        const size_t dest_col_size = vector_size(dest->col);

        record *ch_rec = (record *)(vector_at(source->rec, i));
        record *conformed_rec = record_init(NULL, dest_col_size, -1);

        for (int j = 0; j < dest_col_size; j++) {
            char *ch_dest_col = *(char **)(vector_at(dest->col, j));

            void *val = &ch_dest_col;
            int ch_dest_col_match = vector_p_linear_search(source->col, val);

            const bool match_for_dest = ch_dest_col_match != -1;

            char *cell = NULL;
            if (match_for_dest) {
                vector *cells = ch_rec->cells;
                cell = *(char **)(vector_at(cells, ch_dest_col_match));

                vector_push_back(conformed_rec->cells, &cell);
            } else {
                cell = "";
                vector_push_back(conformed_rec->cells, &cell);
            }
        }

        record_set_data_type(conformed_rec, dest->sort_key_index);
        vector_push_back(dest->rec, conformed_rec);
    }
}

void csv_t_set_sort_key(csv_t *csv, const char *sort_key) {
    assert(csv);

    if (sort_key == NULL) {
        csv->sort_key_index = -1;
        return;
    }

    int sort_key_index = -1;
    void *arg = &sort_key;
    sort_key_index = vector_p_linear_search(csv->col, arg);

    if (sort_key_index < 0) {
        char message[256];
        sprintf(message, "\nKey %s not found.", sort_key);
        fprintf(stderr, "%s\n", message);
        fprintf(stderr, "%s\n", "The file contains these keys:");

        for (int i = 0; i < vector_size(csv->col); i++) {
            char *ch = *(char **)(vector_at(csv->col, i));
            printf("==> %s\n", ch);
        }

        exit(EXIT_FAILURE);
    } else {
        csv->sort_key_index = sort_key_index;

        for (int i = 0; i < vector_size(csv->rec); i++) {
            record *ch = (record *)(vector_at(csv->rec, i));
            record_set_data_type(ch, csv->sort_key_index);
        }
    }
}

void csv_t_sort(csv_t *csv) {
    if (csv->sort_key_index == -1) {
        return;
    }

    vector_p_sort(csv->rec);
}

void csv_t_sort_by_key(csv_t *csv, const char *sort_key) {
    int old_sort_key_index = csv->sort_key_index;

    csv_t_set_sort_key(csv, sort_key);
    vector_p_sort(csv->rec);

    csv->sort_key_index = old_sort_key_index;
    for (int i = 0; i < vector_size(csv->rec); i++) {
        record *ch = (record *)(vector_at(csv->rec, i));
        record_set_data_type(ch, csv->sort_key_index);
    }
}

void csv_t_print(FILE *dest, csv_t *csv) {
    assert(csv);

    if (vector_empty(csv->col)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < vector_size(csv->col); i++) {
        char *cell = *(char **)(vector_at(csv->col, i));

        fprintf(dest, "%s", cell);

        if (i < vector_size(csv->col) - 1) {
            fprintf(dest, "%s", ",");
        } else {
            fprintf(dest, "%s", "\n");
        }
    }

    if (vector_empty(csv->rec)) {

        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < vector_size(csv->rec); i++) {
        record *r = (record *)(vector_at(csv->rec, i));

        for (int j = 0; j < vector_size(r->cells); j++) {
            char *cell = *(char **)(vector_at(r->cells, j));

            fprintf(dest, "%s", cell);

            if (j < vector_size(r->cells) - 1) {
                fprintf(dest, "%s", ",");
            } else {
                fprintf(dest, "%s", "\n");
            }
        }
    }
}

void csv_t_print_reverse(FILE *dest, csv_t *csv) {
    assert(csv);

    if (vector_empty(csv->col)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = 0; i < vector_size(csv->col); i++) {
        char *cell = *(char **)(vector_at(csv->col, i));

        fprintf(dest, "%s", cell);

        if (i < vector_size(csv->col) - 1) {
            fprintf(dest, "%s", ",");
        } else {
            fprintf(dest, "%s", "\n");
        }
    }

    if (vector_empty(csv->rec)) {
        fprintf(dest, "%s", "");
        return;
    }

    for (int i = vector_size(csv->rec) - 1; i >= 0; i--) {
        record *r = (record *)(vector_at(csv->rec, i));

        for (int j = 0; j < vector_size(r->cells); j++) {
            char *cell = *(char **)(vector_at(r->cells, j));

            fprintf(dest, "%s", cell);

            if (j < vector_size(r->cells) - 1) {
                fprintf(dest, "%s", ",");
            } else {
                fprintf(dest, "%s", "\n");
            }
        }
    }
}
 */
