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


#include "gcslib.h"

#include "vec2D.h" /**< vec2D API, a test "object" */

#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/**
 *  @brief  Program execution begins here
 *
 *  @param[in]  argc    argument count
 *  @param[in]  argv    command line arguments
 *
 *  @return     exit status
 */
int main(int argc, char *argv[]) {


    /*
    rbtree *t = rbt_new(_int_);
    int i = 0;
    int temp = 0;

    for (i = 0; i < 20; i++) {
        temp = rand() % 100;
        rbt_insert(t, &temp);
    }

    rbt_puts(t);

    rbt_delete(&t);

    {
        vector *v = v_new(_str_);

        iterator it;
        char **curr = NULL;
        char *str = NULL;

        str = "first";
        v_pushb(v, &str);

        str = "second";
        v_pushb(v, &str);

        str = "third";
        v_pushb(v, &str);

        it = v_begin(v);
        v_erasernge(v, v_begin(v), v_end(v));

        v_puts(v);

        v_delete(&v);

    }
    */

    return EXIT_SUCCESS;
}
