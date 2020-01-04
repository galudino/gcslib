/**
 *  @file       test.c
 *  @brief      Client source file to test struct vector_ptr (vptr_t)
 *
 *  @author     Gemuele Aludino
 *  @date       04 Jan 2020
 *  @copyright  Copyright © 2020 Gemuele Aludino
 */
/**
 *  Copyright © 2020 Gemuele Aludino
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
#include <errno.h>
#include <string.h>

#include "vector_ptr.h"

/* if older than C99, create this macro for strdup */
#if __STD_VERSION__ < 199901L
#define strdup(s)           strcpy(malloc(strlen(s) + 1), s)
#endif

/**< cmpfn__str - comparator for strings */
int cmpfn__str(const void *c0, const void *c1);

/**< cmpfn__int = comparator for int */
int cmpfn__int(const void *c0, const void *c1);

/**< test cases */
void test__str(void);
void test__int(void);

/**
 *  @brief  Program execution begins here
 *
 *  @param[in]  argc    argument count
 *  @param[in]  argv    command line arguments
 *
 *  @return     0 on success, else error (see errno)
 */
int main(int argc, const char *argv[]) {
    printf("\n");

    test__int();
    test__str();
    
    /* testing macros vptr_new and vptr_delete */
    {
        vptr_t *v = vptr_new(2);
        vptr_delete(v);
    }
    
    return 0;
}

/**
 *  @brief  Compares strings c0 and c1
 *
 *  @param[in]  c0  address of a character pointer, (char **)
 *  @param[in]  c1  address of a character pointer, (char **)
 *
 *  @return     0 if c0 and c1 match
 *              (see man pages for strcmp for specifics on return values)
 */
int cmpfn__str(const void *c0, const void *c1) {
    return strcmp(*(char **)(c0), *(char **)(c1));
}

/**
 *  @brief  Compares integers c0 and c1
 *
 *  @param[in]  c0  address of an integer, (int)
 *  @param[in]  c1  address of an integer, (int)
 *
 *  @return     c0 - c1
 */
int cmpfn__int(const void *c0, const void *c1) {
    return (*(int *)(c0) - *(int *)(c1));
}

/**
 *  @brief  Tests vptr_t with dynamically allocated strings (char *)
 */
void test__str() {
    /* popped is a pointer that will store an element to free after a pop/erase function is called */
    char *popped = NULL;

    /* val is a pointer that will store an element that will be used for a push/insert function */
    char *val = NULL;
    
    /**
     *  vec is a struct of type struct vector_ptr residing in stack memory.
     *  (it will persist until main() returns)
     *
     *  the elements of vec's internal buffer are pointers-to-void (void *).
     */
    vptr_t vec = { { NULL, NULL, NULL } };
     
     /* v is convenience pointer to vec. */
    vptr_t *v = &vec;
    
    /* initialize vptr_t ("constructor" or "initializer" fn, explicitly called) */
    vptr_init(v, 1);
    
    /**
     *  vptr_pushb test (push back)
     *      dynamically allocate memory for (char *) in client space
     *      provide address of (char *), which is a (char **) for vptr_pushb
     *      dereferenced address will be assigned to vptr_t's internal buffer.
     */
    val = strdup("charlie");
    vptr_pushb(v, &val);

    val = strdup("alpha");
    vptr_pushb(v, &val);

    val = strdup("delta");
    vptr_pushb(v, &val);

    val = strdup("beta");
    vptr_pushb(v, &val);

    val = strdup("echo");
    vptr_pushb(v, &val);

    val = strdup("golf");
    vptr_pushb(v, &val);

    val = strdup("foxtrot");
    vptr_pushb(v, &val);

    /**
     *  vptr_insert test (insertion in middle)
     *      uses a vptr_iter_t (vector iterator) to determine element insertion.
     *          vptr_begin(v) returns address of v[0]
     *          vptr_end(v) returns address of v[size] (one past last element)
     *         
     *      so vptr_begin(v) + n returns the address of v[n].
     */
    {
        val = strdup("vptr_insert test insertion");
        vptr_insert(v, vptr_begin(v) + 3, &val);
    }
    
    /**
     *  vptr_pushf test (push front)
     *      just like vptr_pushb, but appends to front as opposed to back
     */
    {
        val = strdup("vptr_pushf test string");
        vptr_pushf(v, &val);
    
        popped = vptr_front(v);
        printf("vptr_front: %s\n", *(char **)(popped));
        popped = NULL;
    }
    
    puts("");

    /**
     *  vptr_iter_t test (iterator)
     */
    {
        vptr_iter_t it = vptr_begin(v);

        while (it != vptr_end(v)) {
            printf("it: %s\n", *(char **)(it++));
        }
    } 

    puts("");

    /**
     *  vptr_popf test (pop front)
     *      use a temp variable to capture the address of the element to pop,
     *      and pass the address of temp to vptr_popf when calling it.
     *      then, you may free temp, which holds the address of the memory
     *      to destroy.
     */
    {
        vptr_popf(v, &popped);
        printf("freeing %s from vptr_popf\n", popped);
        
        free(popped);
        popped = NULL;
    }

    puts("");
    
    /**
     *  another iterator test
     */
    {
        vptr_iter_t it = vptr_begin(v);

        while (it != vptr_end(v)) {
            printf("it: %s\n", *(char **)(it++));
        }
    }

    puts("");
    
    /**
     *  vptr_erase test (erasure from middle)
     *      just like vptr_insert, but erases instead.
     *      also uses a temp variable to capture the address of the element to pop.
     */
    {
        vptr_erase(v, vptr_begin(v) + 3, &popped);
        printf("freeing '%s' using vstr_erase\n", popped);
        free(popped);
        popped = NULL;
    }
    
    /**
     *  vptr_qsort test
     */
    vptr_qsort(v, cmpfn__str);
    
    /**
     *  vptr_search test
     */
    val = "echo";
    printf("vstr_search for %s: %d\n", val, vptr_search(vptr_begin(v), cmpfn__str, &val));
    
    puts("");

    /**
     *  another iterator test
     */
    {
        vptr_iter_t it = vptr_begin(v);

        while (it != vptr_end(v)) {
            printf("it: %s\n", *(char **)(it));
            free(*(it++));
        }
    } 

    puts("");

    /**
     *  vptr_deinit - explicit call to 'destructor' function
     */
    vptr_deinit(v);
}

void test__int() {
    vptr_t *v = vptr_new(2);
    int push = 0;
    int pop = 0;
        
    push = 8;
    vptr_pushb(v, &push);

    push = 32;
    vptr_pushb(v, &push);

    push = 16;
    vptr_pushb(v, &push);

    push = 4;
    vptr_pushb(v, &push);

    push = 64;
    vptr_pushb(v, &push);

    push = 0;
    vptr_pushb(v, &push);

    push = 2;
    vptr_pushb(v, &push);

    puts("");

    {
        vptr_iter_t it = vptr_begin(v);
        vptr_iter_t end = vptr_end(v);

        while (it != end) {
            printf("it: %d\n", *(int *)(it++));
        }
    }

    puts("");

    vptr_qsort(v, cmpfn__int);

    puts("");

    {
        vptr_iter_t it = vptr_begin(v);
        vptr_iter_t end = vptr_end(v);

        while (it != end) {
            printf("it: %d\n", *(int *)(it++));
        }
    }
    
    puts("");

    vptr_popb(v, &pop);
    printf("popped: %d\n", pop);

    puts("");

    {
        vptr_iter_t it = vptr_begin(v);
        vptr_iter_t end = vptr_end(v);

        while (it != end) {
            printf("it: %d\n", *(int *)(it++));
        }
    }
    
    puts("");

    vptr_delete(v);
}

