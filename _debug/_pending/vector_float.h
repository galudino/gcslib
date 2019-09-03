/**
 *  @file       vector_float.h
 *  @brief      Header file for vector template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       20 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef VECTOR_FLOAT_H
#define VECTOR_FLOAT_H

#include <stdio.h>

/**
 *  @file       utils.h
 *  @brief      Required for typetable
 */
#include "utils.h"

//*
#ifdef T
#undef T
#endif
#define T float
#include "vector_tmpl.h"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T double
#include "vector_tmpl.h"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T long_double
#include "vector_tmpl.h"
#undef T
//*/

#endif /* VECTOR_FLOAT_H */
