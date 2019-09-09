/**
 *  @file       test.c
 *  @brief      Testbench for C code
 *
 *  @author     Gemuele Aludino
 *  @date       2 Sep 2019
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#include <string.h>

#include "gcslib.h"
#include "vec2D.h"

/* represents list_node_base */
typedef struct parent parent;
struct parent {
    parent *next;
    parent *prev;
};

/* represents list_node */
typedef struct child child;
struct child {
    struct parent base;
    const char *name;
};

/**
 *  @brief  Program execution begins here
 *
 *  @param[in]  argc    argument count
 *  @param[in]  argv    command line arguments
 *
 *  @return     exit status
 */
int main(int argc, const char * argv[]) {
    /*
    child first = { { NULL, NULL }, "john doe" };
    child second = { { NULL, NULL }, "jane doe" };
    child third = { { NULL, NULL }, "jessica doe" };

    parent *p = NULL;

    first.base.next = (parent *)(&second);

    second.base.prev = (parent *)(&first);
    second.base.next = (parent *)(&third);

    third.base.prev = (parent *)(&second);
    
    p = (parent *)(&first);

    while (p != NULL) {
        child c = *(child *)(p);
        printf("name: %s\n", c.name);
        p = c.base.next;
    }
    */

    /* allocate memory for child (list_node) objects */
    child *first = malloc(sizeof *first);
    child *second = malloc(sizeof *second);
    child *third = malloc(sizeof *third);
    
    /* cursor pointers for parent and child */
    parent *p = NULL;
    child *c = first;

    /* handling list_node by base type only */
    parent *pf = NULL;
    parent *ps = NULL;
    parent *pt = NULL;

    /* initializing links to all NULL */
    first->base.prev = first->base.next = NULL;
    second->base.prev = second->base.next = NULL;
    third->base.prev = third->base.next = NULL;

    /* initializing values for first, second, third */
    first->name = "John Doe";
    second->name = "Jane Doe";
    third->name = "Jessica Doe";

    /* assigning base-type pointers to derived type instances */
    pf = *(parent **)(&first);
    ps = *(parent **)(&second);
    pt = *(parent **)(&third);

    pf->next = ps;

    ps->prev = pf;
    ps->next = pt;

    pt->prev = ps;
    
    /*
    first->base.next = *(parent **)(&second);

    second->base.prev = *(parent **)(&first);
    second->base.next = *(parent **)(&third);

    third->base.prev = *(parent **)(&second);
    */

    p = *(parent **)(&c);
    while (p != NULL) {
        c = (child *)(p);
        printf("name: %s\n", c->name);
        p = p->next;
    }

    return EXIT_SUCCESS;
}
