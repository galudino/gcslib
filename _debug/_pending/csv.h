/**
 *  @file       csv.h
 *  @brief      Header file for CSV utilities
 *
 *  @author     Gemuele Aludino
 *  @date       02 Jan 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef CSV_H
#define CSV_H

#include <stdio.h>

#include "vector.h"
#include "list.h"
//#include "slist.h"

typedef struct tokenizer tokenizer;

tokenizer *tk_new(const char *filename);
tokenizer *tk_new_file(FILE *file);
void tk_delete(tokenizer **tk);

void tk_set_delimiter(tokenizer *tk, const int delimiter);
void tk_set_override(tokenizer *tk, const int override);

char *tk_next(tokenizer *tk);
bool tk_has_next(tokenizer *tk);
bool tk_endl(tokenizer *tk);






/*
 enum identifier { EMPTY, LONG, DOUBLE, STRING, UNSPECIFIED };
 struct metadata;

 struct _record {
 vector *cells;
 enum identifier data_type;

 char *sort_value;
 long int long_int_value;
 double double_value;
 };

 typedef struct _record record;

 record *record_init(tokenizer *tk, size_t max_col, int sort_value_index);
 record *record_init_copy(record *tcpy);
 void record_deinit(void *arg);

 void record_set_data_type(record *r, size_t sort_value_index);

 int compare_record(void *c1, void *c2);
 void print_record(void *arg, FILE *dest);

 void vector_print_csv(FILE *dest, vector *col, vector *rec);
 void list_print_csv(FILE *dest, list *col, list *rec);
 void slist_print_csv(FILE *dest, slist *col, slist *rec);

 struct _csv_t {
 vector *col;
 vector *rec;

 int sort_key_index;
 };

 typedef struct _csv_t csv_t;

 csv_t *csv_t_init();
 void csv_t_deinit(void *arg);

 void csv_t_import_file(csv_t *csv, const char *source_file, const char *sort_key);
 void csv_t_import_header(csv_t *csv, const char *strarr[], size_t max_col, const char *sort_key);
 void csv_t_import_record_strarr(csv_t *csv, const char *strarr[], size_t max_col);

 void csv_t_aggregate(csv_t *dest, csv_t *source);

 void csv_t_set_sort_key(csv_t *csv, const char *sort_key);
 void csv_t_sort(csv_t *csv);
 void csv_t_sort_by_key(csv_t *csv, const char *sort_key);

 void csv_t_print(FILE *dest, csv_t *csv);
 void csv_t_print_reverse(FILE *dest, csv_t *csv);
 */
#endif /* CSV_H */
