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

void test_vectorvp_int(void);
void test_vectorvp_str(void);
void test_vectorvp_vec2D(void);

void test_vectortmpl_int(void);
void test_vectortmpl_str(void);

void test_listvp_int(void);
void test_listvp_str(void);
void test_listvp_vec2D(void);

void test_listtmpl_int(void);
void test_listtmpl_str(void);

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
    test_vectorvp_int();
    test_vectorvp_str();
    test_vectorvp_vec2D();

    test_vectortmpl_int();
    test_vectortmpl_str();
    */

    test_listvp_int();
    test_listvp_str();
    test_listvp_vec2D();

    test_listtmpl_int();
    test_listtmpl_str();

    return EXIT_SUCCESS;
}

/**
 *  @brief  Space to test container functionality for integral types
 */
void test_vectorvp_int() {

}

/**
 *  @brief  Space to test container functionality for (char *) types
 */
void test_vectorvp_str() {
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
void test_vectorvp_vec2D() {

}


void test_vectortmpl_int() {
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

void test_vectortmpl_str() {
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
    vdelete(str)(&vs);
}

void test_listvp_int() {
    list *l = l_new(_int_);
    int i = 0;
    int *curr = NULL;
    iterator it;
    void *sentinel = NULL;
    
    for (i = 0; i < 16; i++) {
        l_pushb(l, &i);
    }

    it = l_begin(l);
    sentinel = it_finish(it);

    while ((curr = it_curr(it)) != sentinel) {
        printf("iterator: %d\n", (*curr));
        it_incr(&it);
    }
    
    it = l_erase(l, it_next_n(l_begin(l), 5));
    it = l_erase(l, it);

    printf("\nfront: %d\n", *(int *)(l_front(l)));
    printf("back: %d\n", *(int *)(l_back(l)));

    l_puts(l);
    l_delete(&l);
}

void test_listvp_str() {
    list *l = l_new(_str_);
    
    list *lfill = NULL;
    list *lrnge = NULL;
    list *lcopy = NULL;
    list *lmove = NULL;

    char *curr = NULL;

    iterator it = { NULL, NULL, NULL };
    iterator end = { NULL, NULL, NULL };
    iterator pos = { NULL, NULL, NULL };

    int i = -1;
    void *sentinel = NULL;

    curr = "alpha";
    l_pushb(l, &curr);

    curr = "beta";
    l_pushb(l, &curr);

    curr = "charlie";
    l_pushb(l, &curr);

    curr = "delta";
    l_pushb(l, &curr);

    curr = "echo";
    l_pushb(l, &curr);

    curr = "foxtrot";
    l_pushb(l, &curr);

    curr = "georgia";
    l_pushb(l, &curr);
    
    l_puts(l);

    printf("\nfront: %s\n", *(char **)(l_front(l)));
    printf("back: %s\n\n", *(char **)(l_back(l)));

    curr = " -- fill me up -- ";
    lfill = l_newfill(_str_, 16, &curr);

    l_puts(lfill);

    it = it_next_n(l_begin(l), 2);
    end = it_next_n(l_begin(l), 5);
    sentinel = it_curr(end);
    lrnge = l_newrnge(it, end);

    l_puts(lrnge);

    lcopy = l_newcopy(lrnge);

    l_puts(lcopy);

    lmove = l_newmove(&lcopy);

    LOG(__FILE__, "old:");
    l_puts(lcopy);

    LOG(__FILE__, "move:");
    if (l_back(lmove)) {
        printf("move front: %s\n", *(char **)(l_back(lmove)));
    }
    l_puts(lmove);
}

void test_listvp_vec2D() {

}

void test_listtmpl_int() {

}

void test_listtmpl_str() {

}
