/**
 *  @file       mergesort_template.h
 *  @brief      Template header file for a mergesort algorithm
 *
 *  @author     Gemuele Aludino
 *  @date       21 Jul 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifdef T

#define dlmergesortrecursive(T) template(dlmergesortrecursive, T)
#define dlmergesortiterative(T) template(dlmergesortiterative, T)

void dlmergesortrecursive(T)(void *h, void *t,
                             int (*compare)(const void *, const void *));

#endif /* T */
