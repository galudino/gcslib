/**
 *  @file       vector_str.c
 *  @brief      Header file for vector template instantiations
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
 
#include "vector_str.h"

/* typedef char * char_ptr, for char[] and char * literals */
/* no destructor */
#ifdef T
#undef T
#endif
#define T char_ptr
#include "vector_tmpl.c"
#undef T

/* typedef char * str, for dynamically allocated char * */
/* destructor enabled */
#ifdef T
#undef T
#endif
#define T str
#include "vector_tmpl.c"
#undef T

/* typedef const char *cstr, for const char * literals */
/* no destructor */
#ifdef T
#undef T
#endif
#define T cstr
#include "vector_tmpl.c"
#undef T
