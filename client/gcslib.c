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
    /* 
    test_int_vp();
    test_str_vp();

    test_vec2D_vp();

    test_int_tmpl();
    */
    LOG(__FILE__, "void ptr");
    test_str_vp();

    LOG(__FILE__, "tmpl");
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
    vector *vstr = NULL;
    vector *vs = NULL;

    iterator first;
    iterator last;
    
    char *temp = NULL;

    vstr = v_newr(_str_, 1);

    temp = "----filler----";
    vs = v_newfill(_str_, 16, &temp);

    first = v_begin(vstr);
    last = v_end(vstr);

    temp = "barbara";
    v_pushb(vstr, &temp);

    temp = "kasey";
    v_pushb(vstr, &temp);

    temp = "aliese";
    v_pushb(vstr, &temp);

    temp = "serah";
    v_pushb(vstr, &temp);

    temp = "caterina";
    v_pushb(vstr, &temp);

    temp = "rose";
    v_pushb(vstr, &temp);

    v_puts(vstr);
    v_sort(vstr);
    v_shrink_to_fit(vstr);
    v_puts(vstr);

    first = it_next_n(v_begin(vstr), 2);
    last = it_next_n(v_begin(vstr), 4);

    v_assignrnge(vs, first, last);

    temp = "changed";
    v_assignfill(vs, 1, &temp);

    v_puts(vs);
    
    v_delete(&vstr);
    v_delete(&vs);
}

/**
 *  @brief  Space to test container functionality for "object" types
 */
void test_vec2D_vp() {

}


void test_int_tmpl() {
    vector(int) *v = NULL;
    vector(int) *vr = NULL;
    vector(int) *vfill = NULL;
    vector(int) *vrnge = NULL;
    vector(int) *vmove = NULL;

    int i = 0;
    int *curr = NULL;
    size_t size = 0;
    size_t capacity = 0;

    iterator first;
    iterator last;

    v = vnew(int)();
    vputs(int)(v);

    vr = vnewr(int)(128);
    vputs(int)(vr);

    vfill = vnewfill(int)(32, 4709);
    vputs(int)(vfill);

    capacity = 128;
    for (i = 0; i < capacity; i++) {
        vpushb(int)(vr, i);
    }

    first = it_next_n(vbegin(int)(vr), 25);
    last = it_next_n(vbegin(int)(vr), 50);

    size = vsize(int)(vr);
    vrnge = vnewrnge(int)(first, last);
    vputs(int)(vrnge);

    vmove = vnewmove(int)(&vrnge);

    vputs(int)(vrnge);
    vputs(int)(vmove);

    vdelete(int)(&vrnge);
    vdelete(int)(&vfill);
    vdelete(int)(&vr);
    vdelete(int)(&v);

    first = vbegin(int)(vmove);
    last = vend(int)(vmove);

    while ((curr = it_curr(first)) != it_curr(last)) {
        char str[16];
        sprintf(str, "iterator: %d", (*curr));
        LOG(__FILE__, str);
        it_incr(&first);
    }

    vdelete(int)(&vmove);


    vr = vnewr(int)(1);

    for (i = 0; i < 32; i++) {
        vpushb(int)(vr, rand() % 50);
    }

    vsort(int)(vr);

    vputs(int)(vr);
}

void test_str_tmpl() {
    vector(str) *vstr = vnewr(str)(1);
    iterator first;
    iterator last;

    vector(str) *vs = vnewfill(str)(16, "----filler----");

    vpushb(str)(vstr, "barbara");
    vpushb(str)(vstr, "kasey");
    vpushb(str)(vstr, "aliese");
    vpushb(str)(vstr, "serah");
    vpushb(str)(vstr, "caterina");
    vpushb(str)(vstr, "rose");

    while (vsize(str)(vstr) > 4) {
        vpopb(str)(vstr);
    }

    vputs(str)(vstr);
    
    vsort(str)(vstr);
    vshrinktofit(str)(vstr);
    vputs(str)(vstr);

    first = it_next_n(vbegin(str)(vstr), 2);
    last = it_next_n(vbegin(str)(vstr), 4);

    vassignrnge(str)(vs, first, last);

    vassignfill(str)(vs, 2, "changed");
    vputs(str)(vs);

    veraseat(str)(vstr, 2);
    vputs(str)(vstr);

    vdelete(str)(&vstr);
}
