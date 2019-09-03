/**
 *  @file       vector_float.h
 *  @brief      Header file for vector template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       20 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */
#include "vector_float.h"

//*
#ifdef T
#undef T
#endif
#define T float
#include "vector_tmpl.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T double
#include "vector_tmpl.c"
#undef T
//*/


//*
#ifdef T
#undef T
#endif
#define T long_double
#include "vector_tmpl.c"
#undef T
//*/
