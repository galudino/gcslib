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

void test_int_vp(void);
void test_str_vp(void);
void test_vec2D_vp(void);

void test_int_tmpl(void);
void test_str_tmpl(void);

/**
 *  @brief  Program execution begins here
 *
 *  @param[in]  argc    argument count
 *  @param[in]  argv    command line arguments
 *
 *  @return     exit status
 */
int main(int argc, const char *argv[]) {
    test_int_vp();
    test_str_vp();
    test_vec2D_vp();


    test_int_tmpl();
    test_str_tmpl();

    return EXIT_SUCCESS;
}

/**
 *  @brief  Space to test container functionality for integral types
 */
void test_int_vp() {

}

/**
 *  @brief  Space to test container functionality for (char *) types
 */
void test_str_vp() {

}

/**
 *  @brief  Space to test container functionality for "object" types
 */
void test_vec2D_vp() {

}


void test_int_tmpl() {

}

void test_str_tmpl() {

}
