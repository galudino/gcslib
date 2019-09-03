/**
 *  @file       vector_int.h
 *  @brief      Header file for vector template instantiations
 *
 *  @author     Gemuele Aludino
 *  @date       20 Jun 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef VECTOR_INT
#define VECTOR_INT

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
#include "vector_tmpl.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T int
#include "vector_tmpl.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T int16_t
#include "vector_tmpl.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T int64_t
#include "vector_tmpl.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint8_t
#include "vector_tmpl.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint16_t
#include "vector_tmpl.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint32_t
#include "vector_tmpl.h"
#undef T
//*/

//**
#ifdef T
#undef T
#endif
#define T uint64_t
#include "vector_tmpl.h"
#undef T
//*/

#endif /* VECTOR_INT */
