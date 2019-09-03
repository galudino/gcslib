/**
 *  @file       map_template.h
 *  @brief      Template header file for a type-safe associative array (key-value) ADT
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifdef T1
#ifdef T2
#ifdef K
#ifdef V
#ifdef T

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 *  @file       utils.h
 *  @brief      Required for (struct typetable) and related functions
 */
#include "utils.h"

/**
 *  @file       iterator.h
 *  @brief      Required for iterator (struct iterator) and related functions
 */
#include "iterator.h"

#define map(K, V)       template2t(map, K, V)
#define map_ptr(K, V)   template2t(map_ptr, K, V)
#define map_dptr(K, V)  template2t(map_dptr, K, V)



#endif /* T */
#endif /* V */
#endif /* K */
#endif /* T2 */
#endif /* T1 */
