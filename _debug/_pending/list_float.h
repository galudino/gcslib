/**
 *  @file       list_float.h
 *  @brief      Header file for list template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef LIST_FLOAT_H
#define LIST_FLOAT_H

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
#include "list_template.h"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T double
#include "list_template.h"
#undef T
//*/

//*
#ifdef T
#undef T
#endif
#define T long_double
#include "list_template.h"
#undef T
//*/

#endif /* LIST_FLOAT_H */
