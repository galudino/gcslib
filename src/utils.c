/**
 *  @file       utils.c
 *  @brief      Source file utility functions, macros, etc.
 *
 *  @author     Gemuele Aludino
 *  @date       11 Feb 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */
/**
 *  Copyright © 2019 Gemuele Aludino
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the "Software"),
 *  to deal in the Software without restriction, including without limitation
 *  the rights to use, copy, modify, merge, publish, distribute, sublicense,
 *  and/or sell copies of the Software, and to permit persons to whom the
 *  Software is furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included
 *  in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *  OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 *  DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 *  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH
 *  THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "utils.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct typetable ttbl_char;
struct typetable ttbl_signed_char;
struct typetable ttbl_unsigned_char;

struct typetable ttbl_short_int;
struct typetable ttbl_signed_short_int;
struct typetable ttbl_unsigned_short_int;

struct typetable ttbl_int;
struct typetable ttbl_signed_int;
struct typetable ttbl_unsigned_int;

struct typetable ttbl_long_int;
struct typetable ttbl_signed_long_int;
struct typetable ttbl_unsigned_long_int;

#if __STD_VERSION__ >= 199901L
struct typetable ttbl_long_long_int;
struct typetable ttbl_signed_long_long_int;
struct typetable ttbl_unsigned_long_long_int;
#endif

struct typetable ttbl_float;
struct typetable ttbl_double;
struct typetable ttbl_long_double;

struct typetable ttbl_bool;

struct typetable ttbl_char_ptr;

struct typetable ttbl_str;
struct typetable ttbl_str_ignore_case;
struct typetable ttbl_str_strdup;
struct typetable ttbl_str_ignore_case_strdup;

struct typetable ttbl_cstr;
struct typetable ttbl_cstr_ignore_case;
struct typetable ttbl_cstr_strdup;
struct typetable ttbl_cstr_ignore_case_strdup;

struct typetable ttbl_void_ptr;

struct typetable ttbl_int8;
struct typetable ttbl_int16;
struct typetable ttbl_int32;

#if __STD_VERSION__ >= 199901L
struct typetable ttbl_int64;
#endif

struct typetable ttbl_uint8;
struct typetable ttbl_uint16;
struct typetable ttbl_uint32;

#if __STD_VERSION__ >= 199901L
struct typetable ttbl_uint64;
#endif

void *str_copy(void *arg, const void *other) {
    char **target = (char **)(arg);
    char **source = (char **)(other);

    /*
    (*target) = strdup((*source));
    return (*target);
    */

    (*target) = strcpy(malloc(strlen((*source)) + 1), (*source));
    return (*target) ? (*target) : NULL;
}

void *cstr_copy(void *arg, const void *other) {
    char **target = (char **)(arg);
    char **source = (char **)(other);

    (*target) = strcpy(malloc(strlen((*source)) + 1), (*source));
    return (*target) ? (*target) : NULL;
}

void *void_ptr_copy(void *arg, const void *other) {
    void **source = (void **)(other);
    return (*source);
}

void str_dtor(void *arg) {
    /**
     *  Actual argument upon invocation should be a (char **);
     *  if a string str is a (char *), then:
     *
     *          cstr_delete(&str);
     *
     *  is the call made, clientside.
     */
    char **target = (char **)(arg);

    free((*target));
    (*target) = NULL;
}

void cstr_dtor(void *arg) { str_dtor(arg); }

void void_ptr_dtor(void *arg) { /* DO NOT CHANGE - empty body */
}

void str_swap(void *arg, void *other) {
    char **dest = (char **)(arg);
    char **source = (char **)(other);

    if ((*dest)) {
        char *temp = strcpy(malloc(strlen((*dest)) + 1), (*dest));
        (*dest) = (*source);
        (*source) = temp;
    } else {
        (*dest) = (*source);
        (*source) = NULL;
    }
}

void cstr_swap(void *arg, void *other) {
    char **dest = (char **)(arg);
    char **source = (char **)(other);

    if ((*dest)) {
        char *temp = strcpy(malloc(strlen((*dest)) + 1), (*dest));
        (*dest) = (*source);
        (*source) = temp;
    } else {
        (*dest) = (*source);
        (*source) = NULL;
    }
}

int char_compare(const void *c1, const void *c2) {
    return (int)*(char *)(c1) - (int)*(char *)(c2);
}

int signed_char_compare(const void *c1, const void *c2) {
    return (int)*(signed char *)(c1) - (int)*(signed char *)(c2);
}

int unsigned_char_compare(const void *c1, const void *c2) {
    return (unsigned int)*(unsigned char *)(c1) -
           (unsigned int)*(unsigned char *)(c2);
}

int short_int_compare(const void *c1, const void *c2) {
    return *((short int *)c1) - *((short int *)c2);
}

int signed_short_int_compare(const void *c1, const void *c2) {
    return short_int_compare(c1, c2);
}

int unsigned_short_int_compare(const void *c1, const void *c2) {
    return *((unsigned short int *)c1) - *((unsigned short int *)c2);
}

int int_compare(const void *c1, const void *c2) {
    return *((int *)c1) - *((int *)c2);
}

int signed_int_compare(const void *c1, const void *c2) {
    return int_compare(c1, c2);
}

int unsigned_int_compare(const void *c1, const void *c2) {
    return *((unsigned int *)c1) - *((unsigned int *)c2);
}

int long_int_compare(const void *c1, const void *c2) {
    return (int)(*((long int *)c1) - *((long int *)c2));
}

int signed_long_int_compare(const void *c1, const void *c2) {
    return long_int_compare(c1, c2);
}

int unsigned_long_int_compare(const void *c1, const void *c2) {
    return (int)(*((unsigned long int *)c1) - *((unsigned long int *)c2));
}

#if __STD_VERSION__ >= 199901L
int long_long_int_compare(const void *c1, const void *c2) {
    return (int)(*((long long int *)c1) - *((long long int *)c2));
}

int signed_long_long_int_compare(const void *c1, const void *c2) {
    return long_long_int_compare(c1, c2);
}

int unsigned_long_long_int_compare(const void *c1, const void *c2) {
    return (int)(*((unsigned long long int *)c1) -
                 *((unsigned long long int *)c2));
}
#endif

int float_compare(const void *c1, const void *c2) {
    float result = -99999.9;
    float float1 = *((float *)c1);
    float float2 = *((float *)c2);

    float difference = float1 - float2;
    const bool equal_operands = difference == 0.0;
    const bool c1_greater = difference > 0;
    const bool c2_greater = difference < 0;

    if (equal_operands) {
        result = 0;
    } else {
        if (c1_greater) {
            result = 1;
        } else if (c2_greater) {
            result = -1;
        }
    }

    return result;
}

int double_compare(const void *c1, const void *c2) {
    double result = -99999.9;
    double double1 = *((double *)c1);
    double double2 = *((double *)c2);

    double difference = double1 - double2;
    const bool equal_operands = difference == 0.0;
    const bool c1_greater = difference > 0;
    const bool c2_greater = difference < 0;

    if (equal_operands) {
        result = 0;
    } else {
        if (c1_greater) {
            result = 1;
        } else if (c2_greater) {
            result = -1;
        }
    }

    return result;
}

int long_double_compare(const void *c1, const void *c2) {
    long double result = -99999.9;
    long double long_double1 = *((long double *)c1);
    long double long_double2 = *((long double *)c2);

    long double difference = long_double1 - long_double2;
    const bool equal_operands = difference == 0.0;
    const bool c1_greater = difference > 0;
    const bool c2_greater = difference < 0;

    if (equal_operands) {
        result = 0;
    } else {
        if (c1_greater) {
            result = 1;
        } else if (c2_greater) {
            result = -1;
        }
    }

    return result;
}

int bool_compare(const void *c1, const void *c2) {
    return *((bool *)c1) - *((bool *)c2);
}

int char_ptr_compare(const void *c1, const void *c2) {
    return str_compare(c1, c2);
}

int str_compare(const void *c1, const void *c2) {
    int result = -1;

    char *first = *((char **)(c1));
    char *second = *((char **)(c2));

#if __STD_VERSION__ >= 199901L
    char cfirst[strlen(first) + 1];
    char csecond[strlen(second) + 1];
#else
    char *cfirst = malloc(strlen(first) + 1);
    char *csecond = malloc(strlen(second) + 1);
    assert(cfirst);
    assert(csecond);
#endif

    strcpy(cfirst, first);
    strcpy(csecond, second);

    str_trim(cfirst, NULL);
    str_trim(csecond, NULL);

    result = strcmp(cfirst, csecond);

#if __STD_VERSION__ >= 199901L
    free(cfirst);
    cfirst = NULL;

    free(csecond);
    csecond = NULL;
#endif

    return result;
}

int str_compare_ignore_case(const void *c1, const void *c2) {
    char *first = *((char **)(c1));
    char *second = *((char **)(c2));

    int i = 0;

#if __STD_VERSION__ >= 199901L
    char cfirst[strlen(first) + 1];
    char csecond[strlen(second) + 1];
#else
    char *cfirst = malloc(strlen(first) + 1);
    char *csecond = malloc(strlen(second) + 1);
    assert(cfirst);
    assert(csecond);
#endif

    strcpy(cfirst, first);
    strcpy(csecond, second);

    str_trim(cfirst, NULL);
    str_trim(csecond, NULL);

    while (true) {
        i = toupper(*first) - toupper(*second);

        if (i != 0 || !*first) {
            break;
        } else {
            ++first;
            ++second;
        }
    }

#if __STD_VERSION__ >= 199901L
    free(cfirst);
    cfirst = NULL;

    free(csecond);
    csecond = NULL;
#endif

    return i;
}

int cstr_compare(const void *c1, const void *c2) { return str_compare(c1, c2); }

int cstr_compare_ignore_case(const void *c1, const void *c2) {
    return str_compare_ignore_case(c1, c2);
}

int void_ptr_compare(const void *c1, const void *c2) {
    const char *format = "%d";

    char pointer1[MAXIMUM_STACK_BUFFER_SIZE];
    char pointer2[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(pointer1, format, c1);
    sprintf(pointer2, format, c2);

    return atoi(pointer1) - atoi(pointer2);
}

int int8_compare(const void *c1, const void *c2) {
    return char_compare(c1, c2);
}

int int16_compare(const void *c1, const void *c2) {
    return short_int_compare(c1, c2);
}

int int32_compare(const void *c1, const void *c2) {
    return int_compare(c1, c2);
}

#if __STD_VERSION__ >= 199901L
int int64_compare(const void *c1, const void *c2) {
    return long_long_int_compare(c1, c2);
}
#endif

int uint8_compare(const void *c1, const void *c2) {
    return unsigned_char_compare(c1, c2);
}

int uint16_compare(const void *c1, const void *c2) {
    return unsigned_short_int_compare(c1, c2);
}

int uint32_compare(const void *c1, const void *c2) {
    return unsigned_int_compare(c1, c2);
}

#if __STD_VERSION__ >= 199901L
int uint64_compare(const void *c1, const void *c2) {
    return unsigned_long_long_int_compare(c1, c2);
}
#endif

void char_print(const void *arg, FILE *dest) {
    fprintf(dest, "%c", *(char *)arg);
}

void signed_char_print(const void *arg, FILE *dest) {
    fprintf(dest, "%c", *(char *)arg);
}

void unsigned_char_print(const void *arg, FILE *dest) {
    fprintf(dest, "%c", *(unsigned char *)arg);
}

void short_int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%hi", *(short int *)arg);
}

void signed_short_int_print(const void *arg, FILE *dest) {
    short_int_print(arg, dest);
}

void unsigned_short_int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%hu", *(unsigned short int *)arg);
}

void int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%d", *(int *)arg);
}

void signed_int_print(const void *arg, FILE *dest) { int_print(arg, dest); }

void unsigned_int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%u", *(unsigned int *)arg);
}

void long_int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%li", *(long int *)arg);
}

void signed_long_int_print(const void *arg, FILE *dest) {
    long_int_print(arg, dest);
}

void unsigned_long_int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%lu", *(unsigned long int *)arg);
}

#if __STD_VERSION__ >= 199901L
void long_long_int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%lli", *(long long int *)arg);
}

void signed_long_long_int_print(const void *arg, FILE *dest) {
    long_long_int_print(arg, dest);
}

void unsigned_long_long_int_print(const void *arg, FILE *dest) {
    fprintf(dest, "%llu", *(unsigned long long int *)arg);
}
#endif

void float_print(const void *arg, FILE *dest) {
    fprintf(dest, "%f", *(float *)arg);
}

void double_print(const void *arg, FILE *dest) {
    fprintf(dest, "%lf", *(double *)arg);
}

void long_double_print(const void *arg, FILE *dest) {
    fprintf(dest, "%LF", *(long double *)arg);
}

void bool_print(const void *arg, FILE *dest) {
    fprintf(dest, "%s", *((bool *)arg) ? "true" : "false");
}

void char_ptr_print(const void *arg, FILE *dest) { str_print(arg, dest); }

void str_print(const void *arg, FILE *dest) {
    fprintf(dest, "%s", *(char **)arg);
}

void cstr_print(const void *arg, FILE *dest) { str_print(arg, dest); }

void void_ptr_print(const void *arg, FILE *dest) { fprintf(dest, "%p", arg); }

void int8_print(const void *arg, FILE *dest) { char_print(arg, dest); }

void int16_print(const void *arg, FILE *dest) { short_int_print(arg, dest); }

void int32_print(const void *arg, FILE *dest) { int_print(arg, dest); }

#if __STD_VERSION__ >= 199901L
void int64_print(const void *arg, FILE *dest) {
    long_long_int_print(arg, dest);
}
#endif

void uint8_print(const void *arg, FILE *dest) {
    unsigned_char_print(arg, dest);
}

void uint16_print(const void *arg, FILE *dest) {
    unsigned_short_int_print(arg, dest);
}

void uint32_print(const void *arg, FILE *dest) {
    unsigned_int_print(arg, dest);
}

#if __STD_VERSION__ >= 199901L
void uint64_print(const void *arg, FILE *dest) {
    unsigned_long_long_int_print(arg, dest);
}
#endif

char *char_parse(const void *arg) {
    const char value = *(char *)arg;
    const char *format = "%c";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *signed_char_parse(const void *arg) {
    const char value = *(char *)arg;
    const char *format = "%c";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *unsigned_char_parse(const void *arg) {
    const unsigned char value = *(unsigned char *)arg;
    const char *format = "%c";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *short_int_parse(const void *arg) {
    const char value = *(short int *)arg;
    const char *format = "%hi";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);
    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *signed_short_int_parse(const void *arg) { return short_int_parse(arg); }

char *unsigned_short_int_parse(const void *arg) {
    const char value = *(unsigned short int *)arg;
    const char *format = "%hu";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *int_parse(const void *arg) {
    const char value = *(int *)arg;
    const char *format = "%d";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *signed_int_parse(const void *arg) { return int_parse(arg); }

char *unsigned_int_parse(const void *arg) {
    const char value = *(unsigned int *)arg;
    const char *format = "%u";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *long_int_parse(const void *arg) {
    const char value = *(long int *)arg;
    const char *format = "%li";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *signed_long_int_parse(const void *arg) { return long_int_parse(arg); }

char *unsigned_long_int_parse(const void *arg) {
    const char value = *(unsigned long int *)arg;
    const char *format = "%u";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

#if __STD_VERSION__ >= 199901L
char *long_long_int_parse(const void *arg) {
    const char value = *(long long int *)arg;
    const char *format = "%lld";

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *signed_long_long_int_parse(const void *arg) {
    return long_long_int_parse(arg);
}

char *unsigned_long_long_int_parse(const void *arg) {
    const char value = *(unsigned long long int *)arg;
    const char *format = "%llu";

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}
#endif

char *float_parse(const void *arg) {
    const char value = *(float *)arg;
    const char *format = "%f";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *double_parse(const void *arg) {
    const char value = *(double *)arg;
    const char *format = "%lf";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *long_double_parse(const void *arg) {
    const char value = *(long double *)arg;
    const char *format = "%Lf";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *bool_parse(const void *arg) {
    const bool value = *(bool *)arg;
    const char *format = "%s";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value ? "true" : "false");

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *char_ptr_parse(const void *arg) { return str_parse(arg); }

char *str_parse(const void *arg) {
    const char *value = *(char **)arg;
    const char *format = "%s";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, value);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *cstr_parse(const void *arg) { return str_parse(arg); }

char *void_ptr_parse(const void *arg) {
    const char *format = "%p";
    char *parsed = NULL;

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];

    sprintf(buffer, format, arg);

    parsed = strcpy(malloc(strlen(buffer) + 1), buffer);
    assert(parsed);
    return parsed;
}

char *int8_parse(const void *arg) { return char_parse(arg); }

char *int16_parse(const void *arg) { return short_int_parse(arg); }

char *int32_parse(const void *arg) { return int_parse(arg); }

#if __STD_VERSION__ >= 199901L
char *int64_parse(const void *arg) { return long_long_int_parse(arg); }
#endif

char *uint8_parse(const void *arg) { return unsigned_char_parse(arg); }

char *uint16_parse(const void *arg) { return unsigned_short_int_parse(arg); }

char *uint32_parse(const void *arg) { return unsigned_int_parse(arg); }

#if __STD_VERSION__ >= 199901L
char *uint64_parse(const void *arg) {
    return unsigned_long_long_int_parse(arg);
}
#endif

char *str_trim_left(char *to_trim, const char *charset) {
    size_t trim_length = 0;

    if (strcmp(to_trim, "") == 0) {
        return to_trim;
    }

    if (charset == NULL) {
        charset = ESC_CHARS; /**< "\t\n\v\f\r\" */
    }

    trim_length = strspn(to_trim, charset);

    if (trim_length > 0) {
        size_t str_length = strlen(to_trim);

        if (trim_length == str_length) {
            to_trim[0] = NULL_TERMINATOR;
        } else {
            memmove(to_trim, to_trim + trim_length,
                    str_length + 1 - trim_length);
        }
    }

    return to_trim;
}

char *str_trim_right(char *to_trim, const char *charset) {
    size_t i = 0;

    if (strcmp(to_trim, "") == 0) {
        return to_trim;
    }

    if (charset == NULL) {
        charset = ESC_CHARS; /**< "\t\n\v\f\r\" */
    }

    i = strlen(to_trim) - 1;

    while (strchr(charset, to_trim[i]) != NULL) {
        to_trim[i] = NULL_TERMINATOR;
        i--;
    }

    return to_trim;
}

char *str_trim(char *to_trim, const char *charset) {
    if (strcmp(to_trim, "") == 0) {
        return to_trim;
    }

    return str_trim_left(str_trim_right(to_trim, charset), charset);
}

void void_ptr_swap(void **n1, void **n2) {
    void *temp = *n1;
    *n1 = *n2;
    *n2 = temp;
}

struct typetable ttbl_char = {sizeof(char), NULL,         NULL,
                              NULL,         char_compare, char_print};

struct typetable ttbl_signed_char = {
    sizeof(signed char), NULL, NULL, NULL, signed_char_compare,
    signed_char_print};

struct typetable ttbl_unsigned_char = {
    sizeof(unsigned char), NULL, NULL, NULL, unsigned_char_compare,
    unsigned_char_print};

struct typetable ttbl_short_int = {
    sizeof(short int), NULL, NULL, NULL, short_int_compare, short_int_print};

struct typetable ttbl_signed_short_int = {
    sizeof(signed short int), NULL, NULL, NULL, signed_int_compare,
    signed_int_print};

struct typetable ttbl_unsigned_short_int = {
    sizeof(unsigned short int), NULL, NULL, NULL, unsigned_int_compare,
    unsigned_int_print};

struct typetable ttbl_int = {sizeof(int), NULL,        NULL,
                             NULL,        int_compare, int_print};

struct typetable ttbl_signed_int = {
    sizeof(signed int), NULL, NULL, NULL, signed_int_compare, signed_int_print};

struct typetable ttbl_unsigned_int = {
    sizeof(unsigned int), NULL, NULL, NULL, unsigned_int_compare,
    unsigned_int_print};

struct typetable ttbl_long_int = {sizeof(long int), NULL,          NULL, NULL,
                                  long_int_compare, long_int_print};

struct typetable ttbl_signed_long_int = {
    sizeof(signed long int), NULL, NULL, NULL, signed_long_int_compare,
    signed_long_int_print};

struct typetable ttbl_unsigned_long_int = {
    sizeof(unsigned long int), NULL, NULL, NULL, unsigned_long_int_compare,
    unsigned_long_int_print};

#if __STD_VERSION__ >= 199901L
struct typetable ttbl_long_long_int = {
    sizeof(long long int), NULL, NULL, NULL, long_long_int_compare,
    long_long_int_print};

struct typetable ttbl_signed_long_long_int = {sizeof(signed long long int),
                                              NULL,
                                              NULL,
                                              NULL,
                                              signed_long_long_int_compare,
                                              signed_long_long_int_print};

struct typetable ttbl_unsigned_long_long_int = {sizeof(unsigned long long int),
                                                NULL,
                                                NULL,
                                                NULL,
                                                unsigned_long_long_int_compare,
                                                unsigned_long_long_int_print};
#endif

struct typetable ttbl_float = {sizeof(float), NULL,          NULL,
                               NULL,          float_compare, float_print};

struct typetable ttbl_double = {sizeof(double), NULL,           NULL,
                                NULL,           double_compare, double_print};

struct typetable ttbl_long_double = {
    sizeof(long double), NULL, NULL, NULL, long_double_compare,
    long_double_print};

struct typetable ttbl_bool = {sizeof(bool), NULL,         NULL,
                              NULL,         bool_compare, bool_print};

struct typetable ttbl_char_ptr = {sizeof(char *),   NULL,          NULL, NULL,
                                  char_ptr_compare, char_ptr_print};

struct typetable ttbl_str = {sizeof(char *), str_copy,    str_dtor,
                             str_swap,       str_compare, str_print};

struct typetable ttbl_str_ignore_case = {
    sizeof(char *),          str_copy, str_dtor, str_swap,
    str_compare_ignore_case, str_print};

struct typetable ttbl_cstr = {sizeof(char *), NULL,         NULL,
                              cstr_swap,      cstr_compare, cstr_print};

struct typetable ttbl_cstr_ignore_case = {
    sizeof(char *),           NULL,      NULL, cstr_swap,
    cstr_compare_ignore_case, cstr_print};

struct typetable ttbl_cstr_strdup = {sizeof(char *), cstr_copy,    cstr_dtor,
                                     cstr_swap,      cstr_compare, cstr_print};

struct typetable ttbl_cstr_ignore_case_strdup = {
    sizeof(char *),           cstr_copy, cstr_dtor, cstr_swap,
    cstr_compare_ignore_case, cstr_print};

struct typetable ttbl_void_ptr = {sizeof(void *),   NULL,
                                  void_ptr_dtor,    NULL,
                                  void_ptr_compare, void_ptr_print};

struct typetable ttbl_int8 = {sizeof(char), NULL,         NULL,
                              NULL,         char_compare, char_print};

struct typetable ttbl_int16 = {sizeof(short int), NULL,           NULL, NULL,
                               short_int_compare, short_int_print};

struct typetable ttbl_int32 = {sizeof(int), NULL,        NULL,
                               NULL,        int_compare, int_print};

#if __STD_VERSION__ >= 199901L
struct typetable ttbl_int64 = {
    sizeof(long long int), NULL, NULL, NULL, long_long_int_compare,
    long_long_int_print};
#endif

struct typetable ttbl_uint8 = {
    sizeof(unsigned char), NULL, NULL, NULL, unsigned_char_compare,
    unsigned_char_print};

struct typetable ttbl_uint16 = {
    sizeof(unsigned short int), NULL, NULL, NULL, unsigned_int_compare,
    unsigned_int_print};

struct typetable ttbl_uint32 = {
    sizeof(unsigned int), NULL, NULL, NULL, unsigned_int_compare,
    unsigned_int_print};

#if __STD_VERSION__ >= 199901L
struct typetable ttbl_uint64 = {sizeof(unsigned long long int),
                                NULL,
                                NULL,
                                NULL,
                                unsigned_long_long_int_compare,
                                unsigned_long_long_int_print};
#endif

struct typetable *_char_ = &ttbl_char;
struct typetable *_signed_char_ = &ttbl_signed_char;
struct typetable *_unsigned_char_ = &ttbl_unsigned_char;

struct typetable *_short_int_ = &ttbl_short_int;
struct typetable *_signed_short_int_ = &ttbl_signed_short_int;
struct typetable *_unsigned_short_int_ = &ttbl_unsigned_short_int;

struct typetable *_int_ = &ttbl_int;
struct typetable *_signed_int_ = &ttbl_signed_int;
struct typetable *_unsigned_int_ = &ttbl_unsigned_int;

struct typetable *_long_int_ = &ttbl_long_int;
struct typetable *_signed_long_int_ = &ttbl_signed_long_int;
struct typetable *_unsigned_long_int_ = &ttbl_unsigned_long_int;

#if __STD_VERSION__ >= 199901L
struct typetable *_long_long_int_ = &ttbl_long_long_int;
struct typetable *_signed_long_long_int_ = &ttbl_signed_long_long_int;
struct typetable *_unsigned_long_long_int_ = &ttbl_unsigned_long_long_int;
#endif

struct typetable *_float_ = &ttbl_float;
struct typetable *_double_ = &ttbl_double;
struct typetable *_long_double_ = &ttbl_long_double;

struct typetable *_bool_ = &ttbl_bool;

struct typetable *_char_ptr_ = &ttbl_char_ptr;

struct typetable *_str_ = &ttbl_str;
struct typetable *_str_ignore_case_ = &ttbl_str_ignore_case;

struct typetable *_cstr_ = &ttbl_cstr;
struct typetable *_cstr_ignore_case_ = &ttbl_cstr_ignore_case;
struct typetable *_cstr_strdup_ = &ttbl_cstr_strdup;
struct typetable *_cstr_ignore_case_strdup_ = &ttbl_cstr_ignore_case_strdup;

struct typetable *_void_ptr_ = &ttbl_void_ptr;

struct typetable *_int8_ = &ttbl_int8;
struct typetable *_int8_t_ = &ttbl_int8;
struct typetable *_int16_ = &ttbl_int16;
struct typetable *_int16_t_ = &ttbl_int16;
struct typetable *_int32_ = &ttbl_int32;
struct typetable *_int32_t_ = &ttbl_int32;

#if __STD_VERSION__ >= 199901L
struct typetable *_int64_ = &ttbl_int64;
struct typetable *_int64_t_ = &ttbl_int64;
#endif

struct typetable *_uint8_ = &ttbl_uint8;
struct typetable *_uint8_t_ = &ttbl_uint8;
struct typetable *_uint16_ = &ttbl_uint16;
struct typetable *_uint16_t_ = &ttbl_uint16;
struct typetable *_uint32_ = &ttbl_uint32;
struct typetable *_uint32_t_ = &ttbl_uint32;

#if __STD_VERSION__ >= 199901L
struct typetable *_uint64_ = &ttbl_uint64;
struct typetable *_uint64_t_ = &ttbl_uint64;
#endif

struct typetable *_pthread_t_ = NULL;

bool ulog_disable[] = {false, false, false, false, false};
bool ulog_attrs_disable[] = {false, false, false, false, false, false, false};

/**
 *  Utility function for debugging/error messages
 *
 *  @param[in]      dest        stream destination
 *  @param[in]      level       literals "BUG", "ERROR", "WARNING", or "LOG"
 *  @param[in]      file        macro __FILE__ is to be used here (by client)
 *  @param[in]      func        macro __func__ is to be used here
 *  @param[in]      line        macro __LINE__ is to be used here
 *  @param[in]      fmt         formatting to be used
 *
 *  @return         character count of buffer (from fprintf)
 */
int ulog(FILE *dest, const char *level, const char *file, const char *func,
         long double line, const char *fmt, ...) {

    char buffer[MAXIMUM_STACK_BUFFER_SIZE];
    char temp[256];

    const char *color = KNRM;
    const char *blink = "";

    bool found = false;
    bool is_integer = false;
    bool is_currency = *file == '$';

    int j = 0;

    if (ulog_disable[ALL] ||
        (ulog_attrs_disable[DATE] && ulog_attrs_disable[TIME] &&
         ulog_attrs_disable[LEVEL] && ulog_attrs_disable[FILENAME] &&
         ulog_attrs_disable[LINE] && ulog_attrs_disable[FUNCTION] &&
         ulog_attrs_disable[MESSAGE]) ||
        dest == NULL || level == NULL || file == NULL || func == NULL ||
        line == -666 || fmt == NULL) {
        return 0;
    }

    if (streql(level, "[BUG]")) {
        if (ulog_disable[BUG]) {
            return 0;
        }

        color = KYEL_b;
        found = true;
    }

    if (found == false) {
        if (streql(level, "[LOG]")) {
            if (ulog_disable[LOG]) {
                return 0;
            }

            color = KCYN_b;
            found = true;
        }
    }

    if (found == false) {
        if (streql(level, "[ERROR]")) {
            if (ulog_disable[ERROR]) {
                return 0;
            }

            color = KRED_b;
            blink = KBNK;
            found = true;
        }
    }

    if (found == false) {
        if (streql(level, "[WARNING]")) {
            if (ulog_disable[WARNING]) {
                return 0;
            }

            color = KMAG_b;
            blink = KBNK;
        }
    }

    sprintf(temp, "%Lf", line);

    /* char digit = strchr(temp, '.'); */

#if __STD_VERSION__ >= 199901L
    is_integer = line / (long long int)(line) == 1.000000 || line == 0.00000;
#else
    is_integer = line / (long int)(line) == 1.000000 || line == 0.00000;
#endif

    is_integer = is_currency ? false : is_integer;

    if (ulog_attrs_disable[DATE] == false) {
        char date[1024];
        sprintf(date, "%s%s%s ", KGRY, __DATE__, KNRM);

        j += sprintf(buffer + j, "%s", date);
    }

    if (ulog_attrs_disable[TIME] == false) {
        char time[1024];
        sprintf(time, "%s%s%s ", KGRY, __TIME__, KNRM);

        j += sprintf(buffer + j, "%s", time);
    }

    if (ulog_attrs_disable[LEVEL] == false) {
        char leveltype[1024];
        sprintf(leveltype, "%s%s%s%s ", blink, color, level, KNRM);

        j += sprintf(buffer + j, "%s", leveltype);
    }

    if (ulog_attrs_disable[FILENAME] == false && ulog_attrs_disable[LINE]) {
        char filename[1024];
        sprintf(filename, "[%s] ", file);

        j += sprintf(buffer + j, "%s", filename);
    } else if (ulog_attrs_disable[FILENAME] &&
               ulog_attrs_disable[LINE] == false) {
        char linenumber[1024];

        if (is_integer) {
#if __STD_VERSION__ >= 199901L
            sprintf(linenumber, "[%lli] ", (long long int)(line));
#else
            sprintf(linenumber, "[%li] ", (long int)(line));
#endif
        } else {
            if (is_currency) {
                sprintf(linenumber, "[%0.2Lf] ", line);
            } else {
                sprintf(linenumber, "[%Lf] ", line);
            }
        }

        j += sprintf(buffer + j, "%s", linenumber);
    } else if (ulog_attrs_disable[FILENAME] == false &&
               ulog_attrs_disable[LINE] == false) {
        char fileline[1024];

        if (is_integer) {
#if __STD_VERSION__ >= 199901L
            sprintf(fileline, "[%s:%lli] ", file, (long long int)(line));
#else
            sprintf(fileline, "[%s:%li] ", file, (long int)(line));
#endif
        } else {
            if (is_currency) {
                sprintf(fileline, "[%s%0.2Lf] ", file, line);
            } else {
                sprintf(fileline, "[%s:%Lf] ", file, line);
            }
        }

        j += sprintf(buffer + j, "%s", fileline);
    }

    if (ulog_attrs_disable[FUNCTION] == false) {
        char function[1024];
        sprintf(function, "%s%s", KCYN, func);

        j += sprintf(buffer + j, "%s", function);
    }

    if (ulog_attrs_disable[FUNCTION] == false &&
        ulog_attrs_disable[MESSAGE] == false) {
        j += sprintf(buffer + j, "%s", ": ");
    }

    if (ulog_attrs_disable[MESSAGE] == false) {
        char message[4096];
        va_list args;

        va_start(args, fmt);
        vsprintf(message, fmt, args);
        va_end(args);

        j += sprintf(buffer + j, "%s%s%s", KNRM_b, message, KNRM);
    }

    return fprintf(dest, "%s\n", buffer);
}

/**
 *  Determine the memory "block" distance between two addresses,
 *  beg and end, by their "block" widths, determined by sizeof(TYPENAME)
 *
 *  @param[in]  beg     the beginning address
 *  @param[in]  end     the ending address
 *  @param[in]  width   sizeof(TYPENAME), or size in bytes of TYPENAME
 *
 *  @return     distance between beg and end
 *
 *  Precondition: beg and end must be non-NULL and pointing to memory
 *                of the same data type
 */
ptrdiff_t ptr_distance(const void *beg, const void *end, size_t width) {
    char *start = (((char *)(beg)) + (width));
    char *finish = (((char *)(end)) + (width));

    return ((finish - start) / width);
}

void lnb_swap(list_node_base *x, list_node_base *y) {
    if (x->next != x) {
        if (y->next != y) {
            /* Both x and y are not empty. */
            list_node_base *tmp_x_next = x->next;
            list_node_base *tmp_x_prev = x->prev;

            /* swap(x->next, y->next) */
            x->next = y->next;
            y->next = tmp_x_next;

            /* swap(x->prev, y->prev) */
            x->prev = y->prev;
            y->prev = tmp_x_prev;
        } else {
            /* x is not empty, y is empty. */
            y->next = x->next;
            y->prev = x->prev;

            y->next->prev = y;
            y->prev->next = y;

            x->next = x;
            x->prev = x;
        }
    } else if (y->next != y) {
        /* x is empty, y is not empty. */
        x->next = y->next;
        x->prev = y->prev;
        x->next->prev = x->prev->next = x;
        y->next = y->prev = y;
    }
}

void lnb_transfer(list_node_base *n, list_node_base *first, list_node_base *last) {
    if (n != last) {
        /* Remove [first, last) from its old position. */
        last->prev->next = n;
        first->prev->next = last;
        n->prev->next = first;

        /* Splice [first, last) into its new position. */
        {
            list_node_base *tmp_n_prev = n->prev;
            n->prev = last->prev;
            last->prev = first->prev;
            first->prev = tmp_n_prev;
        }
    }
}

void lnb_reverse(list_node_base *n) {
    list_node_base *tmp = n;
    
    do {
        /* swap(tmp->next, tmp->prev) */
        list_node_base *tmp_next = tmp->next;
        tmp->next = tmp->prev;
        tmp->prev = tmp_next;

        tmp = tmp->prev;
    } while (tmp != n);
}

void lnb_hook(list_node_base *n, list_node_base *position) {
    n->next = position;
    n->prev = position->prev;

    position->prev->next = n;
    position->prev = n;
}

void lnb_unhook(list_node_base *n) {
    list_node_base *next_node = n->next;
    list_node_base *prev_node = n->prev;

    prev_node->next = next_node;
    next_node->prev = prev_node;
}

size_t lnb_distance(list_node_base *pos, list_node_base *end) {
    size_t count = 0;

    while (pos != end) {
        ++count;
        pos = pos->next;
    }

    return count;
}
