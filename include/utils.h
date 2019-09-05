/**
 *  @file       utils.h
 *  @brief      Header file for utility functions, macros, etc.
 *
 *  @author     Gemuele Aludino
 *  @date       20 Jun 2019
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

#ifndef UTILS_H
#define UTILS_H

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
/**
 *  Any header/source file associated with gcslib must include utils.h.
 *  (Note that most container headers already include utils.h, so a client
 *   need not include this header directly, unless they wish to make their
 *   own container using the contents of utils.h/utils.c)
 *
 *  utils.h is required for struct typetable and related function signatures for
 *  comparators, copy functions, print functions, and destructors used by
 *  built-in types, and serves as an example for user-defined types.
 *
 *  *** Use the name of your custom type in place of foobar. ***
 *
 *  Note that compound word types (e.g. struct foobar must be typedef'ed
 *  into a single word.
 *
 *      (struct foobar) must be typedef'ed to (foo)
 *
 *  A user defined type, foobar, must declare the following function
 *  prototypes to be used by an instance of (struct typetable):
 *
 *          void *foobar_copy(void *dst, const void *src);
 *          void foobar_dtor(void *arg)
 *          void foobar_swap(void *s1, void *s2);
 *          int foobar_compare(const void *c1, const void *c2);
 *          void foobar_print(const void *arg, FILE *dest);
 *
 *  These prototypes may be in a public header if required to be,
 *  but are suggested to remain in a source file prior to their definitions
 *  only.
 *
 *  Within your public header file for foobar, after creating the following
 *  function prototypes for your user-defined type,
 *  use the following macro inside your header file within a global scope:
 *
 *      in something like foobar.h, after your declared function prototypes:
 *      TYPETABLE_DECLARE_PTR(foobar);
 *
 *  foobar will be prepended to each function prototype identifier,
 *  by means of token pasting and the preprocessor macros used to produce
 *  instances of (struct typetable) and (struct typetable *)
 *  require tokens with no spaces and/or asterisks, as stated above.
 *
 *  In foobar's source file,
 *  you will then create the definitions for the four prototypes listed above.
 *
 *  After the respective functions are defined,
 *  use the following macro inside your source file within a global scope:
 *
 *      in something like foobar.c, after your defined functions:
 *      TYPETABLE_DEFINE_PTR(foobar);
 *
 *  The handle (pointer), _foobar_ is a (struct typetable *)
 *  that will be used for the instantation of containers, such as vector,
 *  list, map, etc.
 *
 *  It should go without saying, but --
 *  if a user deviates from the naming convention described, i.e.
 *  doesn't use TYPENAME_compare, or TYPENAME_print, for example --
 *  the TYPETABLE macros will fail to work.
 *  You do not have to use them, and you can create your typetables
 *  manually if you would like to. (Just define an instance of
 *  struct typetable with a pointer that you can access at any time.)
 *
 *  Note that the _TYPENAME_ naming convention for the typetable
 *  pointer is used for tmpl ADTs -- so if you want to use the
 *  tmpld data structures, your struct typetable pointer
 *  will have to have this naming convention.
 *
 *  See this header file, and utils.c for examples.
 */

/**< optional typedefs for pointers-to-functions */
typedef bool (*unary_predicate_fn)(const void *);
typedef bool (*binary_predicate_fn)(const void *, const void *);
typedef void (*consumer_fn)(const void *);

typedef void *(*copy_fn)(void *, const void *);
typedef void (*dtor_fn)(void *);
typedef void (*swap_fn)(void *, void *);
typedef int (*compare_fn)(const void *, const void *);
typedef void (*print_fn)(const void *, FILE *);

/**
 *  @def        ADDR_AT
 *  @brief      Retrieves the address of an element within a (void *)
 *
 *  Used to retrieve the address of an element within a block of memory,
 *  particularly a void pointer to a dynamically allocated array.
 */
#define ADDR_AT(BASE, INDEX, WIDTH) (((char *)(BASE)) + ((INDEX) * (WIDTH)))

/**
 *  @struct     typetable
 *  @brief      a virtual function table that determines the behavior of
 *              a container ADT when acting with or upon its elements
 */
struct typetable {
    size_t width;                               /**< sizeof(T) */

    void *(*copy)(void *, const void *);        /**< intended for deep copies */
    void (*dtor)(void *);    /**< for release of dynamically allocated memory */
    void (*swap)(void *, void *);/**< for moving dynamically allocated memory */

    int (*compare)(const void *, const void *); /**< sorting/searching */
    void (*print)(const void *, FILE *dest);    /**< output to stream */
};

/**< Use these pointer variables to instantiate an ADT container (i.e. vector)
 where a formal parameter type of (struct typetable *) is required */

extern struct typetable *_char_;
extern struct typetable *_signed_char_;
extern struct typetable *_unsigned_char_;

extern struct typetable *_short_int_;
extern struct typetable *_signed_short_int_;
extern struct typetable *_unsigned_short_int_;

extern struct typetable *_int_;
extern struct typetable *_signed_int_;
extern struct typetable *_unsigned_int_;

extern struct typetable *_long_int_;
extern struct typetable *_signed_long_int_;
extern struct typetable *_unsigned_long_int_;

#if __STD_VERSION__ >= 199901L
extern struct typetable *_long_long_int_;
extern struct typetable *_signed_long_long_int_;
extern struct typetable *_unsigned_long_long_int_;
#endif

extern struct typetable *_float_;
extern struct typetable *_double_;
extern struct typetable *_long_double_;

extern struct typetable *_bool_;

extern struct typetable *_char_ptr_;

extern struct typetable *_str_;
extern struct typetable *_str_ignore_case_;

extern struct typetable *_cstr_;
extern struct typetable *_cstr_ignore_case_;
extern struct typetable *_cstr_strdup_;
extern struct typetable *_cstr_ignore_case_strdup_;

extern struct typetable *_void_ptr_;

extern struct typetable *_int8_;
extern struct typetable *_int8_t_;
extern struct typetable *_int16_;
extern struct typetable *_int16_t_;
extern struct typetable *_int32_;
extern struct typetable *_int32_t_;
extern struct typetable *_int64_;
extern struct typetable *_int64_t_;

extern struct typetable *_uint8_;
extern struct typetable *_uint8_t_;
extern struct typetable *_uint16_;
extern struct typetable *_uint16_t_;
extern struct typetable *_uint32_;
extern struct typetable *_uint32_t_;

#if __STD_VERSION__ >= 199901L
extern struct typetable *_uint64_;
extern struct typetable *_uint64_t_;
#endif

extern struct typetable *_pthread_t_;

/**
 *  @def        TYPETABLE_DECLARE_PTR
 *  @brief      For use by a client's user-defined type (in its header file),
 *              creates a handle for a struct typetable to be used for
 *              container instantiations
 */
#define TYPETABLE_DECLARE_PTR(TYPENAME) extern struct typetable *_##TYPENAME##_

/**
 *  @def        TYPETABLE_DEFINE_PTR
 *  @brief      For use by a client's user-defined type (in its source file),
 *              creates an instance struct typetable and defines the
 *              address for its handle for container instantiations
 *
 *  If at least one of the function pointers will be NULL,
 *  your struct typetable instance should be defined manually.
 */
#define TYPETABLE_DEFINE_PTR(TYPENAME)                                         \
struct typetable ttbl_##TYPENAME = {                                           \
    sizeof(TYPENAME),                                                          \
    TYPENAME##_copy,                                                           \
    TYPENAME##_dtor,                                                           \
    TYPENAME##_swap,                                                           \
    TYPENAME##_compare,                                                        \
    TYPENAME##_print                                                           \
};                                                                             \
                                                                               \
struct typetable *_##TYPENAME##_ = &ttbl_##TYPENAME                            \


#define TYPETABLE_DEFINE_PTR(TYPENAME)                                         \
struct typetable ttbl_##TYPENAME = {                                           \
    sizeof(TYPENAME), TYPENAME##_copy, TYPENAME##_dtor,                        \
    TYPENAME##_swap, TYPENAME##_compare, TYPENAME##_print                      \
};                                                                             \
                                                                               \
struct typetable *_##TYPENAME##_ = &ttbl_##TYPENAME                            \

/**
 *  Stack allocated instances and heap allocated instances will need to be
 *  casted differently. You will need two different sets of
 *  typetables (with different functions to corresponding tables)
 *  to account for differences in typecasting. Consider the following:
 *
 *  struct mytype foo;
 *  foo.x = 1;
 *  foo.y = 2;
 *  mytype_print(&foo, stdout); // address-of (foo) is a (struct mytype *)
 *
 *  void mytype_print(const void *arg, FILE *dest) {
 *      struct mytype *t = (struct mytype *)(arg);      // casting a pointer
 *      fprintf(dest, "%d: %d", t->x, t->y);
 *  }
 *
 *  --- vs. ---
 *
 *  struct mytype *foo;
 *  foo->x = 1;
 *  foo->y = 2;
 *  mytype_print(&foo, stdout); // address-of (foo *) is a (struct mytype **)
 *
 *  void mytype_print(const void *arg, FILE *dest) {
 *      struct mytype *t = *(struct mytype **)(arg);    // casting a handle
 *      fprintf(dest, "%d: %d", t->x, t->y);
 *  }
 */

/**< Copy functions - casts arg to TYPE and copies the pointee */
void *str_copy(void *arg, const void *other);
void *cstr_copy(void *arg, const void *other);
void *void_ptr_copy(void *arg, const void *other);

/**< Destructor functions - casts arg to TYPE and frees the pointee */
void str_dtor(void *arg);
void cstr_dtor(void *arg);
void void_ptr_dtor(void *arg); /*<< empty body */

/**< Swap functions */
void str_swap(void *arg, void *other);
void cstr_swap(void *arg, void *other);

/**< Comparator functions - casts c1 and c2 to TYPE and compares the args */
int char_compare(const void *c1, const void *c2);
int signed_char_compare(const void *c1, const void *c2);
int unsigned_char_compare(const void *c1, const void *c2);

int short_int_compare(const void *c1, const void *c2);
int signed_short_int_compare(const void *c1, const void *c2);
int unsigned_short_int_compare(const void *c1, const void *c2);

int int_compare(const void *c1, const void *c2);
int signed_int_compare(const void *c1, const void *c2);
int unsigned_int_compare(const void *c1, const void *c2);

int long_int_compare(const void *c1, const void *c2);
int signed_long_int_compare(const void *c1, const void *c2);
int unsigned_long_int_compare(const void *c1, const void *c2);

#if __STDC_VERSION__ >= 199901L
int long_long_int_compare(const void *c1, const void *c2);
int signed_long_long_int_compare(const void *c1, const void *c2);
int unsigned_long_long_int_compare(const void *c1, const void *c2);
#endif

int float_compare(const void *c1, const void *c2);
int double_compare(const void *c1, const void *c2);
int long_double_compare(const void *c1, const void *c2);

int bool_compare(const void *c1, const void *c2);

int char_ptr_compare(const void *c1, const void *c2);
int char_arr_compare(const void *c1, const void *c2);
int char_arr_compare_ignore_case(const void *c1, const void *c2);

int str_compare(const void *c1, const void *c2);
int str_compare_ignore_case(const void *c1, const void *c2);
int cstr_compare(const void *c1, const void *c2);
int cstr_compare_ignore_case(const void *c1, const void *c2);

int void_ptr_compare(const void *c1, const void *c2);

int int8_compare(const void *c1, const void *c2);
int int16_compare(const void *c1, const void *c2);
int int32_compare(const void *c1, const void *c2);

#if __STD_VERSION__ >= 199901L
int int64_compare(const void *c1, const void *c2);
#endif

int uint8_compare(const void *c1, const void *c2);
int uint16_compare(const void *c1, const void *c2);
int uint32_compare(const void *c1, const void *c2);

#if __STD_VERSION__ >= 199901L
int uint64_compare(const void *c1, const void *c2);
#endif

/**< Print functions - casts arg to TYPE and prints output to dest */
void char_print(const void *arg, FILE *dest);
void signed_char_print(const void *arg, FILE *dest);
void unsigned_char_print(const void *arg, FILE *dest);

void short_int_print(const void *arg, FILE *dest);
void signed_short_int_print(const void *arg, FILE *dest);
void unsigned_short_int_print(const void *arg, FILE *dest);

void int_print(const void *arg, FILE *dest);
void signed_int_print(const void *arg, FILE *dest);
void unsigned_int_print(const void *arg, FILE *dest);

void long_int_print(const void *arg, FILE *dest);
void signed_long_int_print(const void *arg, FILE *dest);
void unsigned_long_int_print(const void *arg, FILE *dest);

#if __STD_VERSION__ >= 199901L
void long_long_int_print(const void *arg, FILE *dest);
void signed_long_long_int_print(const void *arg, FILE *dest);
void unsigned_long_long_int_print(const void *arg, FILE *dest);
#endif

void float_print(const void *arg, FILE *dest);
void double_print(const void *arg, FILE *dest);
void long_double_print(const void *arg, FILE *dest);

void bool_print(const void *arg, FILE *dest);

void char_ptr_print(const void *arg, FILE *dest);
void char_arr_print(const void *arg, FILE *dest);

void str_print(const void *arg, FILE *dest);
void cstr_print(const void *arg, FILE *dest);

void void_ptr_print(const void *arg, FILE *dest);

void int8_print(const void *arg, FILE *dest);
void int16_print(const void *arg, FILE *dest);
void int32_print(const void *arg, FILE *dest);

#if __STD_VERSION__ >= 199901L
void int64_print(const void *arg, FILE *dest);
#endif

void uint8_print(const void *arg, FILE *dest);
void uint16_print(const void *arg, FILE *dest);
void uint32_print(const void *arg, FILE *dest);

#if __STD_VERSION__ >= 199901L
void uint64_print(const void *arg, FILE *dest);
#endif

/**< Parse functions - casts arg to TYPE and returns a (char *)  */
char *char_parse(const void *arg);
char *signed_char_parse(const void *arg);
char *unsigned_char_parse(const void *arg);

char *short_int_parse(const void *arg);
char *signed_short_int_parse(const void *arg);
char *unsigned_short_int_parse(const void *arg);

char *int_parse(const void *arg);
char *signed_int_parse(const void *arg);
char *unsigned_int_parse(const void *arg);

char *long_int_parse(const void *arg);
char *signed_long_int_parse(const void *arg);
char *unsigned_long_int_parse(const void *arg);

#if __STDC_VERSION__ >= 199901L
char *long_long_int_parse(const void *arg);
char *signed_long_long_int_parse(const void *arg);
char *unsigned_long_long_int_parse(const void *arg);
#endif

char *float_parse(const void *arg);
char *double_parse(const void *arg);
char *long_double_parse(const void *arg);

char *bool_parse(const void *arg);

char *char_ptr_parse(const void *arg);
char *char_arr_parse(const void *arg);

char *str_parse(const void *arg);
char *cstr_parse(const void *arg);

char *void_ptr_parse(const void *arg);

char *int8_parse(const void *arg);
char *int16_parse(const void *arg);
char *int32_parse(const void *arg);

#if __STD_VERSION__ >= 199901L
char *int64_parse(const void *arg);
#endif

char *uint8_parse(const void *arg);
char *uint16_parse(const void *arg);
char *uint32_parse(const void *arg);

#if __STD_VERSION__ >= 199901L
char *uint64_parse(const void *arg);
#endif

/**< C-String trim functions */
char *str_trim_left(char *to_trim, const char *charset);
char *str_trim_right(char *to_trim, const char *charset);
char *str_trim(char *to_trim, const char *charset);

#define streql(s1, s2) strcmp(s1, s2) == 0
#define strneql(s1, s2, n) strncmp(s1, s2, n) == 0

/**< void ptr swappage */
void void_ptr_swap(void **n1, void **n2);

#define ESC_CHARS "\t\n\v\f\r\" "
#define NULL_TERMINATOR '\0'
#define QUOTATION_MARK '\"'
#define COMMA ','
#define PERIOD '.'
#define DASH '-'

#define APPLE_LOGO ""
#define APPROX_EQ_SYMBOL "≈"
#define BETA_SYMBOL "ß"
#define CENTS_SYMBOL "¢"
#define COPYRIGHT "©"
#define DELTA_SYMBOL "∆"
#define DEGREE_SYMBOL "°"
#define DIAMOND_SYMBOL "◊"
#define DIVISION_SYMBOL "÷"
#define EMPTY_SET_SYMBOL "ø"
#define EURO_CURRENCY_SYMBOL "€"
#define GTEQUAL_SYMBOL "≥"
#define INFINITY_SYMBOL "∞"
#define LTEQUAL_SYMBOL "≤"
#define MU "µ"
#define OMEGA_CAP "Ω"
#define PARAGRAPH_SYMBOL "¶"
#define PI_CAP_SYMBOL "∏"
#define PI_SYMBOL "π"
#define POUND_CURRENCY_SYMBOL "£"
#define RIGHTS_RESERVED_SYMBOL "®"
#define SIGMA_CAP "∑"
#define THETA_LOW_SYMBOL "θ"
#define TRADEMARK_SYMBOL "™"
#define UPSIDEDOWN_EXCPT "¡"
#define UPSIDEDOWN_QUEST "¿"

#define BUFFER_SIZE 256
#define BUFFER_SIZE_4K 4096
#define MAXIMUM_STACK_BUFFER_SIZE 16384

/**
 *  Redefine a macro of interest by using a preprocessor directive
 *  before the inclusion of "utils.h"
 *      For example, to redefine CSTR_ARR_DEFAULT_SIZE (for cstr_arr)
 *      
 *      #ifdef CSTR_ARR_DEFAULT_SIZE
 *      #undef CSTR_ARR_DEFAULT_SIZE
 *      #endif
 *      #define CSTR_ARR_DEFAULT_SIZE   <nonzero integer of positive magnitude>
 * 
 *      #include "utils.h"
 */
#ifndef CSTR_ARR_DEFAULT_SIZE
#define CSTR_ARR_DEFAULT_SIZE 256
#endif /* CSTR_ARR_DEFAULT_SIZE */

#ifndef CHAR_ARR_DEFAULT_SIZE
#define CHAR_ARR_DEFAULT_SIZE 256
#endif /* CHAR_ARR_DEFAULT_STR */

#ifndef CHAR_PTR_ARR_DEFAULT_SIZE
#define CHAR_PTR_ARR_DEFAULT_SIZE 256
#endif /* CHAR_PTR_ARR_DEFAULT_SIZE */

#ifndef CCHAR_ARR_DEFAULT_SIZE
#define CCHAR_ARR_DEFAULT_SIZE 256
#endif /* CCHAR_ARR_DEFAULT_SIZE */

#ifndef CCHAR_PTR_ARR_DEFAULT_SIZE
#define CCHAR_PTR_ARR_DEFAULT_SIZE 256
#endif /* CCHAR_PTR_ARR_DEFAULT_SIZE */

#define KNRM "\x1B[0;0m"   /**< reset to standard color/weight */
#define KNRM_b "\x1B[0;1m" /**< standard color bold */

#define KGRY "\x1B[0;2m" /**< dark grey */

#define KITL "\x1B[0;3m" /**< italics */
#define KULN "\x1B[0;4m" /**< underline */

#define KBNK "\x1B[0;5m" /**< blink every second */
#define KHIL "\x1B[0;7m" /**< grey highlight */

#define KRED "\x1B[0;31m" /**< red */
#define KGRN "\x1B[0;32m" /**< green */
#define KYEL "\x1B[0;33m" /**< yellow */
#define KBLU "\x1B[0;34m" /**< blue */
#define KMAG "\x1B[0;35m" /**< magenta */
#define KCYN "\x1B[0;36m" /**< cyan */
#define KWHT "\x1B[0;37m" /**< white */

#define KRED_b "\x1B[1;31m" /**< red bold */
#define KGRN_b "\x1B[1;32m" /**< green bold */
#define KYEL_b "\x1B[1;33m" /**< yellow bold */
#define KBLU_b "\x1B[1;34m" /**< blue bold */
#define KMAG_b "\x1B[1;35m" /**< magenta bold */
#define KCYN_b "\x1B[1;36m" /**< cyan bold */
#define KWHT_b "\x1B[1;37m" /**< white bold */

/**< actual arguments within a macro that involve pointer types
 must have their asterisk(s) 'typedefed' the asterisk away.
 this is not ideal, but is needed for token pasting. */

/**
 *  char_ptr        is a pointer to a char
 *  char_dptr       is a pointer to a char_ptr (ptr that pts to a char)
 *  char_tptr       is a pointer to char_dptr  (ptr to a ptr that pts to char)
 */
typedef char *char_ptr;
typedef char **char_dptr;
typedef char ***char_tptr;
/**
 *  char_arr[CHAR_ARR_DEFAULT_SIZE]
 *      is an array of char, size CHAR_ARR_DEFAULT_SIZE
 *  char_ptr_arr[CHAR_PTR_ARR_DEFAULT_SIZE]
 *      is a pointer to an array of char, size CHAR_PTR_ARR_DEFAULT_SIZE
 */
typedef char char_arr[CHAR_ARR_DEFAULT_SIZE];
typedef char *char_ptr_arr[CHAR_PTR_ARR_DEFAULT_SIZE];

/**
 *  cchar_ptr       is a pointer to an immutable char (address is mutable)
 *  cchar_dptr      is a pointer to a cchar_ptr
 *  cchar_tptr      is a pointer to a cchar_dptr
 */
typedef const char *cchar_ptr;
typedef const char **cchar_dptr;
typedef const char ***cchar_tptr;

/**
 *  cchar_arr[CHAR_ARR_DEFAULT_SIZE]
 *      is an array of const char, size CCHAR_ARR_DEFAULT_SIZE
 *  cchar_ptr_arr[CCHAR_PTR_ARR_DEFAULT_SIZE]
 *      is a pointer to an array of cchar_ptr, size CCHAR_PTR_ARR_DEFAULT_SIZE
 */
typedef const char cchar_arr[CCHAR_ARR_DEFAULT_SIZE];
typedef const char *cchar_ptr_arr[CCHAR_PTR_ARR_DEFAULT_SIZE];

/**
 *  str         is a "synonym" for char_ptr
 *  str_ptr     is a "synonym" for char_dptr
 *  str_dptr    is a "synonym" for char_tptr
 */
typedef char_ptr str;
typedef char_dptr str_ptr;
typedef char_tptr str_dptr;

/**
 *  str_arr     is a "synonym" for char_ptr_arr
 */
typedef char_ptr_arr str_arr;

/**
 *  cstr        is a "synonym" for cchar_ptr, recommended for string literals
 *  cstr_ptr    is a "synonym" for cchar_dptr
 *  cstr_dptr   is a "synonym" for a cchar_tptr
 */
typedef cchar_ptr cstr;
typedef cchar_dptr cstr_ptr;
typedef cchar_tptr cstr_dptr;

/**
 *  cstr_arr    is a "synonym" for cchar_ptr_arr[CCHAR_PTR_ARR_DEFAULT_SIZE]
 */
typedef cchar_ptr_arr cstr_arr;

/**
 *  use char_arr
 *      for statically allocated array of char (null-terminated, or not)
 *      char_arr (statically allocated array of char) have mutable characters.
 * 
 *  use char_ptr or str
 *      for dynamically allocated strings or string literals
 *      char_ptr/str have mutable addresses, but immutable characters.
 *      (compiler will not protect against character mutations of stack
 *       allocated string literals)
 *      
 *      note: the pointer-to-typetable, '_str_', include a copy function
 *      for creating dynamically allocated string duplicates, as well as a dtor
 *      function for releasing memory allocated by the copy function.
 *      the '_char_ptr_' pointer-to-typetable does not define a copy or dtor.
 * 
 *  use char_dptr or str_ptr
 *      for dynamically allocated arrays of (char_ptr), aka (str)
 * 
 *  use cchar_dptr or cstr_ptr
 *      for dynamically allocated arrays of (ccstr_ptr), aka (cstr)
 * 
 *  use char_ptr_arr or str_arr
 *      for statically allocated arrays of (char_ptr), aka (str)
 * 
 *  use cchar_ptr_arr or cstr_arr
 *      for statically allocated arrays of (cchar_ptr), aka (cstr)
 */

typedef signed char *signed_char_ptr;
typedef signed char **signed_char_dptr;
typedef unsigned char *unsigned_char_ptr;
typedef unsigned char **unsigned_char_dptr;

typedef short int *short_int_ptr;
typedef short int **short_int_dptr;
typedef signed short int *signed_short_int_ptr;
typedef signed short int **signed_short_int_dptr;

typedef int *int_ptr;
typedef int **int_dptr;

typedef signed int *signed_int_ptr;
typedef signed int **signed_int_dptr;
typedef unsigned int *unsigned_int_ptr;
typedef unsigned int **unsigned_int_dptr;

typedef long int *long_int_ptr;
typedef long int **long_int_dptr;

typedef signed long int *signed_long_int_ptr;
typedef signed long int **signed_long_int_dptr;
typedef unsigned long int *unsigned_long_int_ptr;
typedef unsigned long int **unsigned_long_int_dptr;

#if __STDC_VERSION__ >= 199901L
typedef long long int *long_long_int_ptr;
typedef long long int **long_long_int_dptr;

typedef signed long long int *signed_long_long_int_ptr;
typedef signed long long int **signed_long_long_int_dptr;
typedef unsigned long long int *unsigned_long_long_int_ptr;
typedef unsigned long long int **unsigned_long_long_int_dptr;
#endif

typedef float *float_ptr;
typedef float **float_dptr;
typedef double *double_ptr;
typedef double **double_dptr;
typedef long double *long_double_ptr;
typedef long double **long_double_dptr;

typedef bool *bool_ptr;
typedef bool **bool_dptr;

typedef void *void_ptr;
typedef void **void_dptr;

typedef char *int8_ptr;
typedef char **int8_dptr;

typedef short int *int16_ptr;
typedef short int **int16_dptr;

typedef int *int32_ptr;
typedef int **int32_dptr;

#if __STD_VERSION__ >= 199901L
typedef long long int *int64_ptr;
typedef long long int **int64_dptr;
#endif

typedef unsigned char *uint8_ptr;
typedef unsigned char **uint8_dptr;

typedef unsigned short int *uint16_ptr;
typedef unsigned short int **uint16_dptr;

typedef unsigned int *uint32_ptr;
typedef unsigned int **uint32_dptr;

#if __STD_VERSION__ >= 199901L
typedef unsigned long long int *uint64_ptr;
typedef unsigned long long int **uint64_dptr;
#endif

typedef float float32_t;
typedef double float64_t;
typedef long double long_double;

/**< utils: debugging */
int ulog(FILE *dest,
         const char *level,     /**< meant for "BUG", "LOG", "ERROR", or "WARNING" */
         const char *file,      /**< meant for use with the __FILE__ macro */
         const char *func,      /**< meant for use with the __func__ macro */
         long double line,      /**< meant for use with the __LINE__ macro */
         const char *fmt, ...); /**< user's custom message */

/**
 *  Unless you would like to create a customized
 *  debugging message, please use the following preprocessor directives.
 *
 *  BUG is suggested for documenting bugs at certain points in your program.
 *  LOG is a general-purpose messaging tool.
 *  ERROR is used for displaying error messages (i.e. something failed, etc)
 *  WARNING is used to notify the client of an impending issue.
 *
 *  The first argument in BUG, LOG, ERROR, and WARNING is FILEMACRO,
 *  which refers to the following macro:
 *          __FILE__
 *  This is just a string, so if you are building a custom ulog message,
 *  with BUG, LOG, ERROR, or WARNING, and would like to put a different
 *  string in place of __FILE__, you may do so.
 */

/**< Designated default streams for BUG, LOG, ERROR, and WARNING */
#define ULOG_STREAM_BUG stderr
#define ULOG_STREAM_LOG stdout
#define ULOG_STREAM_ERROR stderr
#define ULOG_STREAM_WARNING stderr

/**
 *  @def        BUG
 *  @brief      Shorthand macro for ulog to note bugs in a program
 */
#if __STDC_VERSION__ >= 199901L
#define BUG(FILEMACRO, ...)                                                    \
    ulog(ULOG_STREAM_BUG, "[BUG]", FILEMACRO, __func__,                        \
         (long int)__LINE__, __VA_ARGS__)
#else
#define BUG(FILEMACRO, MSG)                                                    \
    ulog(ULOG_STREAM_BUG, "[BUG]", FILEMACRO, __func__,                        \
         (long int)__LINE__, MSG)
#endif

/**
 *  @def        LOG
 *  @brief      Shorthand macro for ulog to create messages for a program
 */
#if __STDC_VERSION__ >= 199901L
#define LOG(FILEMACRO, ...)                                                    \
    ulog(ULOG_STREAM_LOG, "[LOG]", FILEMACRO, __func__,                        \
         (long int)__LINE__, __VA_ARGS__)
#else
#define LOG(FILEMACRO, MSG)                                                    \
    ulog(ULOG_STREAM_LOG, "[LOG]", FILEMACRO, __func__,                        \
         (long int)__LINE__, MSG)
#endif

/**
 *  @def        ERROR
 *  @brief      Shorthand macro for ulog to display errors for a program
 */
#if __STDC_VERSION__ >= 199901L
#define ERROR(FILEMACRO, ...)                                                  \
    ulog(ULOG_STREAM_ERROR, "[ERROR]", FILEMACRO, __func__,                    \
         (long int)__LINE__, __VA_ARGS__)
#else
#define ERROR(FILEMACRO, MSG)                                                  \
    ulog(ULOG_STREAM_ERROR, "[ERROR]", FILEMACRO, __func__,                    \
         (long int)__LINE__, MSG)
#endif

/**
 *  @def        WARNING
 *  @brief      Shorthand macro for ulog to display warning for a program
 */
#if __STDC_VERSION__ >= 199901L
#define WARNING(FILEMACRO, ...)                                                \
    ulog(ULOG_STREAM_WARNING, "[WARNING]", FILEMACRO, __func__,                \
         (long int)__LINE__, __VA_ARGS__)
#else
#define WARNING(FILEMACRO, MSG)                                                \
    ulog(ULOG_STREAM_WARNING, "[WARNING]", FILEMACRO, __func__,                \
         (long int)__LINE__, MSG)
#endif

#define UTILS_LOG_COUNT 5
extern bool ulog_disable[UTILS_LOG_COUNT];
enum ULOG_TYPE { ALL, BUG, LOG, ERROR, WARNING };

#define ULOG_TOGGLE_TYPE(ULOG_TYPE)                                            \
    (ulog_disable[ULOG_TYPE]) = (ulog_disable[ULOG_TYPE]) ? (false) : (true)

#define UTILS_LOG_ATTRS_COUNT 7
enum ULOG_ATTRS { DATE, TIME, LEVEL, FILENAME, LINE, FUNCTION, MESSAGE };
extern bool ulog_attrs_disable[UTILS_LOG_ATTRS_COUNT];

#define ULOG_TOGGLE_ATTR(ULOG_ATTR)                                            \
    ulog_attrs_disable[ULOG_ATTR] =                                            \
        (ulog_attrs_disable[ULOG_ATTR]) ? (false) : (true);

/**
 *  When using the ULOG toggle macros below:
 *
 *  Copy and paste these directives into a function (like main(), or in a
 *  function where you would like the toggle to occur, for example)
 *  and uncomment to toggle any of the ulog types and/or attributes on/off.
 *
 *  (Tip: instead of toggling all of these off to turn off
 *        all utils_log appearances, use the
 *              ULOG_TOGGLE_TYPE(ALL)
 *         directive instead, to disable all utils_log appearances.)
 *
 *  All toggles are OFF (false) by default.
 *  Uncommenting the macro will set the respective type/attribute to true.
 *  Invoking the directive again will undo the previous action.
 */

/**
ULOG_TOGGLE_TYPE(ALL);
ULOG_TOGGLE_TYPE(BUG);
ULOG_TOGGLE_TYPE(LOG);
ULOG_TOGGLE_TYPE(ERROR);
ULOG_TOGGLE_TYPE(WARNING);

ULOG_TOGGLE_ATTR(DATE);
ULOG_TOGGLE_ATTR(TIME);
ULOG_TOGGLE_ATTR(LEVEL);
ULOG_TOGGLE_ATTR(FILENAME);
ULOG_TOGGLE_ATTR(LINE);
ULOG_TOGGLE_ATTR(FUNCTION);
ULOG_TOGGLE_ATTR(MESSAGE);
*/

/**
 *  Token-pasting utilities for "templates" (tmpl)
 */

#define CAT(X, Y) X##_##Y
#define tmpl(ARG, TYPE) CAT(ARG, TYPE)

#define DCAT(X, Y, Z) X##_##Y##_##Z
#define tmpl2t(ARG, TYPE1, TYPE2) DCAT(ARG, TYPE1, TYPE2)

#define TCAT(X, Y, Z, ZZ) X##_##Y##_##Z_##ZZ
#define tmpl3t(ARG, TYPE1, TYPE2, TYPE3) TCAT(ARG, TYPE1, TYPE2, TYPE3)

#define QCAT(W, X, Y, Z, ZZ) WW##_##X##_##Y##_##Z##_##ZZ
#define tmpl4t(ARG, TYPE1, TYPE2, TYPE3, TYPE4) QCAT(ARG, TYPE1, TYPE2, TYPE3, TYPE4)

#define table_id(TBL_TYPE, T) tmpl(TBL_TYPE, T)
#define table_ptr_id(TYPE, T) _##TYPE##_##T##_
#define type_table_ptr_id(T) _##T##_

/**
 *  Node utility enumerations
 */
enum node_color { RED, BLACK };
enum node_traversal { INORDER, PREORDER, POSTORDER, LEVELORDER };

/**
 *  Pointer utility functions
 */
ptrdiff_t ptr_distance(const void *beg, const void *end, size_t width);

#endif /* UTILS_H */
