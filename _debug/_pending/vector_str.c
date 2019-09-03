/**
 *  @file       vector_str.c
 *  @brief      Header file for vector template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       20 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#include "vector_str.h"

//* typedef char * char_ptr, for char[] and char * literals
// no destructor
#ifdef T
#undef T
#endif
#define T char_ptr
#include "vector_tmpl.c"
#undef T
//*/

//* typedef char * str, for dynamically allocated char *
// destructor enabled
#ifdef T
#undef T
#endif
#define T str
#include "vector_tmpl.c"
#undef T
//*/

//* typedef const char *cstr, for const char * literals
// no destructor
#ifdef T
#undef T
#endif
#define T cstr
#include "vector_tmpl.c"
#undef T
//*/
