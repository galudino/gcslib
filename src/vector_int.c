/**
 *  @file       vector_int.c
 *  @brief      Source file for vector template instantiations
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
 
#include "vector_int.h"

//*
#ifdef T
#undef T
#endif
#define T char
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T int
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T int16_t
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T int64_t
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T uint8_t
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T uint16_t
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T uint32_t
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T uint64_t
#include "vector_tmpl.c"
#undef T
//*/
