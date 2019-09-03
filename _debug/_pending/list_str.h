/**
 *  @file       list_str.h
 *  @brief      Header file for list template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef LIST_STR_H
#define LIST_STR_H

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
#include "list_template.h"
#undef T
//*/

//* typedef char * str, for dynamically allocated char *
// destructor enabled
#ifdef T
#undef T
#endif
#define T str
#include "list_template.h"
#undef T
//*/

//* typedef const char *cstr, for const char * literals
// no destructor
#ifdef T
#undef T
#endif
#define T cstr
#include "list_template.h"
#undef T
//*/

#endif /* LIST_STR_H */
