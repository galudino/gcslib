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
#include "vector_string.h"
#include "vec2D.h"

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
    vector *v = v_new(_vec2D_);
    
    vec2D v1;
    vec2D v2;

    _vec2D_->copy = NULL;
    vec2D_init(&v1, 23.1, 34.2);
    vec2D_init(&v2, 45.3, 42.1);

    v_pushb(v, &v1);
    v_pushb(v, &v2);

    v_puts(v);
    v_delete(&v);
    */

    /*
    vector *v = v_new(_str_);
    char *str = NULL;

    _str_->copy = NULL;

    str = strdup("string1");
    v_pushb(v, &str);

    str = strdup("string2");
    v_pushb(v, &str);

    v_puts(v);

    v_delete(&v);
    */

    /*
    vector_str *v = vnew_str();
    char *str = NULL;

    _str_->copy = NULL;

    str = strdup("string1");
    vpushbptr_str(v, &str);

    str = strdup("string2");
    vpushbptr_str(v, &str);
    
    vputs_str(v);
    vdelete_str(&v);
    */

    /*
    vector_str *v = vnew_str();

    vpushb_str(v, "string1");
    vpushb_str(v, "string2");

    vputs_str(v);
    vdelete_str(&v);
    */

    return EXIT_SUCCESS;
}
