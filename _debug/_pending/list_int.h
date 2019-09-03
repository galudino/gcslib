/**
 *  @file       list_int.h
 *  @brief      Header file for list template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef LIST_INT
#define LIST_INT

/**
 *  @file       utils.h
 *  @brief      Required for typetable
 */
#include "utils.h"

//*
#ifdef T
#undef T
#endif
#define T char
#include "list_template.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T int
#include "list_template.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T int16_t
#include "list_template.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T int64_t
#include "list_template.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint8_t
#include "list_template.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint16_t
#include "list_template.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint32_t
#include "list_template.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint64_t
#include "list_template.h"
#undef T
//*/

#endif /* LIST_INT */
