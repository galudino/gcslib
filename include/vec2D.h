/**
 *  @file       vec2D.h
 *  @brief      Header file for a two-dimensional vector (mathematics)
 *
 *  @author     Gemuele Aludino
 *  @date       26 Jun 2019
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

#ifndef VEC2D_H
#define VEC2D_H

#include "utils.h"

typedef struct vec2D vec2D;
typedef struct vec2D *vec2D_ptr;
typedef struct vec2D **vec2D_dptr;

/**
 *  @struct     vec2D
 *  @brief      contains a pointer intended for dynamically allocated double
 */
struct vec2D {
    double *attrs;
};

/**< vec2D_ptr: new/delete */
vec2D *vec2D_ptr_new(double vx, double vy);
void vec2D_ptr_delete(void *arg);

/**< vec2D: constructor/destructor */
void vec2D_init(vec2D *v, double vx, double vy);
void vec2D_deinit(vec2D *v);

/**< vec2D: accessors */
double vec2D_x(vec2D *v);
double vec2D_y(vec2D *v);

/*<< vec2D: mutators */
void vec2D_set_x(vec2D *v, double vx);
void vec2D_set_y(vec2D *v, double vy);

/*<< vec2D: magnitude/angle in degrees */
double vec2D_magnitude(vec2D *v);
double vec2D_angle_deg(vec2D *v);

/*<< vec2D: mutators - scale/invert */
void vec2D_scale(vec2D *v, double scalar);
void vec2D_invertdir(vec2D *v);

/*<< vec2D: add/subtract two vec2Ds */
vec2D *vec2D_add(vec2D *v2, vec2D *v1);
vec2D *vec2D_subtract(vec2D *v2, vec2D *v1);

/**< vec2D: typetable functions */
void *vec2D_copy(void *arg, const void *other);
void vec2D_dtor(void *arg);
void vec2D_swap(void *c1, void *c2);
int vec2D_compare(const void *c1, const void *c2);
void vec2D_print(const void *arg, FILE *dest);
void vec2D_print_override_simple(const void *arg, FILE *dest);

/**< vec2D_ptr: typetable functions */
void *vec2D_ptr_copy(void *arg, const void *other);
void vec2D_ptr_dtor(void *arg);
void vec2D_ptr_swap(void *c1, void *c2);
int vec2D_ptr_compare(const void *c1, const void *c2);
void vec2D_ptr_print(const void *arg, FILE *dest);
void vec2D_ptr_print_override_simple(const void *arg, FILE *dest);

/*<< Since all typetable functions are defined,
     this macro can be used. */
TYPETABLE_DECLARE_PTR(vec2D);
TYPETABLE_DECLARE_PTR(vec2D_ptr);

#endif /* vec2D_H */
