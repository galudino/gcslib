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
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <assert.h>

#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE 1
#include <string.h>

#ifdef __STRICT_ANSI__
int n = 9999;
#else
int n = 2;
#endif

/*

strdup():
_SVID_SOURCE || _BSD_SOURCE || _XOPEN_SOURCE >= 500 || _XOPEN_SOURCE && _XOPEN_SOURCE_EXTENDED || / _POSIX_C_SOURCE >= 200809L
strndup():
POSIX_C_SOURCE >= 200809L || _XOPEN_SOURCE >= 700
_GNU_SOURCE
strdupa(), strndupa(): _GNU_SOURCE

*/

void *str_copy(void *arg, const void *other) {
    char **target = (char **)(arg);
    char **source = (char **)(other);

    (*target) = strdup((*source));

    return (*target);
}

/**
 *  @brief  Program execution begins here
 *
 *  @param[in]  argc    argument count
 *  @param[in]  argv    command line arguments
 *
 *  @return     exit status
 */
int main(int argc, const char * argv[]) {
    char *str = NULL;
    char *copyme = "you've copied me!!!!";

    str_copy(&str, &copyme);

    printf("result: %s\n", str);
    printf("n = %d\n", n);

    return EXIT_SUCCESS;
}
