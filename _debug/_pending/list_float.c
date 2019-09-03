/**
 *  @file       list_float.c
 *  @brief      Source file for list template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#include "list_float.h"

//*
#ifdef T
#undef T
#endif
#define T float
#include "list_template.c"
#include "mergesort_template.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T double
#include "list_template.c"
#include "mergesort_template.c"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T long_double
#include "list_template.c"
#include "mergesort_template.c"
#undef T
//*/
