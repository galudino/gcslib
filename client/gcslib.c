/**
 *  @file       gcslib.c
 *  @brief      Testbench for gcslib containers
 *
 *  @author     Gemuele Aludino
 *  @date       1 Sep 2019
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

#include "gcslib.h"             /**< Entire gcslib_c library */
#include "vec2D.h"              /**< vec2D API, a test "object" */

#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void test_int(void);
void test_str(void);
void test_vec2D(void);

/**
 *  @brief  Program execution begins here
 *
 *  @param[in]  argc    argument count
 *  @param[in]  argv    command line arguments
 *
 *  @return     exit status
 */
int main(int argc, const char *argv[]) {
    test_int();
    test_str();
    test_vec2D();

    return EXIT_SUCCESS;
}

/**
 *  @brief  Space to test container functionality for integral types
 */
void test_int() {
    vector *v = v_new(_int_);
    for (int i = 0; i < 25; i++) {
        int r = rand() % 100;
        v_pushb(v, &r);
    }

    v_shrink_to_fit(v);

    // EXPERMENTAL MACRO: for-each macro
    // for each int i in iterator it, print it.
    // Just supply the target type,
    // an identifier (name) you'll give
    // to identify an element of the target type,
    // and the iterator that you will traverse.
    // note: rvalue iterators will not work.
    // make it addressable - store it in a variable first.
    iterator it = v_begin(v);
    foreach(int, i, it) {
        LOG(__FILE__, "iterator: %d", *i);
    }

    v_delete(&v);
}

/**
 *  @brief  Space to test container functionality for (char *) types
 */
void test_str() {
    const char arr[][64] = { "alpha", "beta", "charlie", "delta",
    "echo", "foxtrot", "golf", "hottub", "icecream", "jerseygirl" };

    vector *v1 = v_new(_cstr_);
    for (int i = 0; i < 10; i++) {
        const char *cstr = arr[i];
        v_pushb(v1, &cstr);
    }

    v_erase_at(v1, 3);
    v_puts(v1);

    // using iterators to generate a range from within v1 at indices [2, 6),
    // and using that range to create a new vector
    vector *v2 =
    v_newrnge(it_next_n(v_begin(v1), 2), it_next_n(v_begin(v1), 6));

    v_puts(v2);

    v_delete(&v2);
    v_delete(&v1);
}

/**
 *  @brief  Space to test container functionality for "object" types
 */
void test_vec2D() {
    // If you intend to use your user-defined composite type (struct)
    // as the struct itself, or a pointer-to-struct,
    // note that each will require their own typetable, as demonstrated below.
    // usage with the containers remains the same.

    vector *v1 = v_new(_vec2D_);

    for (int i = 0; i < 8; i++) {
        double r1 = rand() % 90;
        double r2 = rand() % 90;

        vec2D vect;
        vec2D_init(&vect, r1, r2);
        v_pushb(v1, &vect);
    }

    v_puts(v1);
    v_delete(&v1);

    vector *v2 = v_new(_vec2D_ptr_);

    for (int i = 0; i < 8; i++) {
        double r1 = rand() % 90;
        double r2 = rand() % 90;

        vec2D *vect = vec2D_ptr_new(r1, r2);
        v_pushb(v2, &vect);
    }

    v_puts(v2);
    v_delete(&v2);
}
