/**
 *  @file       vector_str.h
 *  @brief      Header file for vector template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       20 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef VECTOR_STR_H
#define VECTOR_STR_H

/**
 *  @file       utils.h
 *  @brief      Required for typetable
 */
#include "utils.h"

//* typedef char * char_ptr, for char[] and char * literals
// no destructor
#ifdef T
#undef T
#endif
#define T char_ptr
#include "vector_tmpl.h"
#undef T
//*/

//* typedef char * str, for dynamically allocated char *
// destructor enabled
#ifdef T
#undef T
#endif
#define T str
#include "vector_tmpl.h"
#undef T
//*/

//* typedef const char *cstr, for const char * literals
// no destructor
#ifdef T
#undef T
#endif
#define T cstr
#include "vector_tmpl.h"
#undef T
//*/

#endif /* VECTOR_STR_H */
