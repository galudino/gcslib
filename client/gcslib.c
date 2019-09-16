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

void test_vectorvp_str_assign(void);
void test_vectortmpl_str_assign(void);

void test_listvp_int(void);
void test_listvp_int_at(void);
void test_listvp_int_splice(void);
void test_listvp_str(void);
void test_listvp_str_splice(void);
void test_listvp_vec2D(void);

void test_listtmpl_int(void);
void test_listtmpl_str(void);

bool remove_if_even(const void *arg) {
    assert(arg);
    return (*(int *)(arg)) % 2 == 0;
}

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

    test_vectorvp_str_assign();
    test_vectortmpl_str_assign();
    */

    test_listvp_int_splice();
    test_listvp_str_splice();

    /*
    test_listvp_int();
    test_listvp_int_at();

    test_listvp_str();
    test_listvp_vec2D();

    test_listtmpl_int();
    test_listtmpl_str();
    */

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

void test_vectorvp_str_assign() {
    vector *v1 = v_new(_str_);
    const char *arr1[] = { "alpha", "beta", "charlie", "delta", "echo", "foxtrot", "kilo" };
    
    vector *v2 = v_new(_str_);
    const char *arr2[] = { "j", "k", "l", "m", "n", "o", "p", "q", "r" };
    
    int i = 0;
    for (i = 0; i < sizeof(arr1) / sizeof(char *); i++) {
        v_pushb(v1, &arr1[i]);
    }

    for (i = 0; i < sizeof(arr2) / sizeof(char *); i++) {
        v_pushb(v2, &arr2[i]);
    }

    v_puts(v1);
    v_puts(v2);

    {
        iterator it = { NULL, NULL, NULL };
        iterator pos = { NULL, NULL, NULL };
        iterator beg = { NULL, NULL, NULL };
        iterator end = { NULL, NULL, NULL };
        char *filler = "filler";

        pos = it_next_n(v_begin(v1), 3);
        beg = it_next_n(v_begin(v2), 2);
        end = it_next_n(v_begin(v2), 6);

        v_assignrnge(v1, beg, end);
        v_puts(v1);

        v_assignfill(v1, 16, &filler);
        v_puts(v1);
    }

    v_delete(&v1);
    v_delete(&v2);
}

void test_vectortmpl_str_assign() {
    vector(str) *v1 = vnew(str)();
    char *arr1[] = { "alpha", "beta", "charlie", "delta", "echo", "foxtrot", "kilo" };
    
    vector(str) *v2 = vnew(str)();  
    char *arr2[] = { "j", "k", "l", "m", "n", "o", "p", "q", "r" };
    
    int i = 0;

    iterator it;
    iterator pos;
    iterator beg;
    iterator end;

    for (i = 0 ; i < sizeof(arr1) / sizeof(char *); i++) {
        vpushb(str)(v1, arr1[i]);
    }

    for (i = 0; i < sizeof(arr2) / sizeof(char *); i++) {
        vpushb(str)(v2, arr2[i]);
    }

    vputs(str)(v1);
    vputs(str)(v2);

    beg = it_next_n(vbegin(str)(v2), 2);
    end = it_next_n(vbegin(str)(v2), 6);

    vassignfill(str)(v1, 3, arr2[4]);
    vassignrnge(str)(v1, beg, end);
    vputs(str)(v1);

    vdelete(str)(&v1);
    vdelete(str)(&v2);
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

void test_listvp_int_at() {
    list *l = l_new(_int_);
    int i = 0;
    for (i = 0; i < 32; i++) {
        l_pushb(l, &i);
    }

    l_puts(l);

    LOG(__FILE__, "insert 9999 at 5");
    i = 9999;
    l_insert_at(l, 5, &i);
    l_puts(l);

    LOG(__FILE__, "erase 9999 at 5");
    l_erase_at(l, 5);
    l_puts(l);

    LOG(__FILE__, "replace element at 24 with 48");
    i = 48;
    l_replace_at(l, 24, &i);
    l_puts(l);

    LOG(__FILE__, "swap 10 and 20");
    l_swap_elem(l, 10, 20);
    l_puts(l);

    l_delete(&l);
}

void test_listvp_str() {
    list *l = l_new(_str_);
    
    list *lfill = NULL;
    list *lrnge = NULL;
    list *lcopy = NULL;
    list *lmove = NULL;
    list *dummy = NULL;

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

    dummy = l_new(_str_);

    curr = "b";
    l_pushb(dummy, &curr);

    curr = "c";
    l_pushb(dummy, &curr);

    curr = "d";
    l_pushb(dummy, &curr);

    curr = "e";
    l_pushb(dummy, &curr);

    curr = "f";
    l_pushb(dummy, &curr);

    curr = "g";
    l_pushb(dummy, &curr);

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
    
    curr = " -- insertion -- ";
    it = l_insert(l, it, &curr);
    l_puts(l);

    printf("it: %s\n", *(char **)(it_curr(it)));

    curr = " -- fill me up -- ";
    it = l_insertfill(l, it, 8, &curr);

    l_puts(l);

    printf("it: %s\n", *(char **)(it_curr(it)));

    it = l_insertrnge(l, it, l_begin(lmove), l_end(lmove));
    l_puts(l);

    printf("it: %s\n", *(char **)(it_curr(it)));

    it = l_erase(l, it);
    l_puts(l);

    printf("it: %s\n", *(char **)(it_curr(it)));

    it = l_erasernge(l, it_next_n(l_begin(l), 3), it_next_n(l_begin(l), 11));
    l_puts(l);

    LOG(__FILE__, "l_assignrnge:");
    l_assignrnge(l, l_begin(dummy), l_end(dummy));
    l_puts(l);

    l_assignfill(l, 16, &curr);
    l_puts(l);
}

void test_listvp_int_splice() {
    list *l1 = l_new(_int_);
    list *l2 = l_new(_int_);
    int i = 0;
    int index = -1;
    iterator it;
    iterator pos;
    iterator opos;
    iterator first;
    iterator last;

    for (i = 0; i < 10; i++) {
        l_pushb(l1, &i);
    }

    for (i = 10; i < 21; i++) {
        l_pushb(l2, &i);
    }

    l_puts(l1);
    l_puts(l2);

    LOG(__FILE__, "l_splice for l1 at pos 4, l2 from opos 2");
    pos = it_next_n(l_begin(l1), 4);
    opos = it_next_n(l_begin(l2), 2);
    it = l_splice(l1, pos, l2, opos);
    l_puts(l1);
    l_puts(l2);

    LOG(__FILE__, "l_splicelist for l1 at pos 4, all of l2");
    it = l_splicelist(l1, it, l2);
    l_puts(l1);
    l_puts(l2);

    LOG(__FILE__, "l_splicernge for l2 at begin");
    first = l_begin(l1);
    last = it_next_n(l_begin(l1), 4);
    it = l_splicernge(l2, l_begin(l2), l1, first, last);
    l_puts(l1);
    l_puts(l2);

    LOG(__FILE__, "l_splicernge for l2 at it");
    first = it_next_n(l_begin(l1), 20);
    last = it_next_n(l_begin(l1), 25);
    l_splicernge(l2, it, l1, first, last);
    l_puts(l1);
    l_puts(l2);

    LOG(__FILE__, "l_remove for l2, 13");
    i = 13;
    l_remove(l2, &i);
    l_puts(l2);

    LOG(__FILE__, "l_remove_if, even numbers");
    l_remove_if(l2, remove_if_even);
    l_puts(l2);

    LOG(__FILE__, "l_unique, get rid of 23's (except one");
    i = 23;
    l_pushf(l2, &i);
    l_insert_at(l2, 3, &i);
    l_insert_at(l2, 4, &i);
    l_puts(l2);

    LOG(__FILE__, "l_reverse");
    l_reverse(l2);
    l_puts(l2);

    /*
    LOG(__FILE__, "l_sort");
    l_sort(l2);
    l_puts(l2);
    */

    i = 1;
    index = l_search(l2, &i);
    printf("%d is at index %d\n", i, index);

    l_delete(&l1);
    l_delete(&l2);
}

void test_listvp_str_splice() {

}

void test_listvp_vec2D() {

}

void test_listtmpl_int() {

}

void test_listtmpl_str() {

}
