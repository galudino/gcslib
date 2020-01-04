/**
 *  @file       vector_ptr.c
 *  @brief      Source file for a vector ADT (elements of any pointer type)
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
#include "vector_ptr.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  capacity
 *
 *  @return
 */
vptr_t *vptr_init(vptr_t *self, size_t capacity) {
    void **start = NULL;
    
    self->impl.start = NULL;
    self->impl.finish = NULL;
    self->impl.end_of_storage = NULL;
    
    start = calloc(capacity, sizeof *start);
    assert(start);

    self->impl.start = start;
    self->impl.finish = self->impl.start;
    self->impl.end_of_storage = self->impl.start + capacity;

    return self;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *
 *  @return
 */
vptr_t *vptr_deinit(vptr_t *self) {
    free(self->impl.start);
    self->impl.start = NULL;

    self->impl.finish = NULL;
    self->impl.end_of_storage = NULL;

    return self;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *
 *  @return
 */
void *vptr_front(vptr_t *self) {
    if (self->impl.start == self->impl.finish) {
        return NULL;
    } else {
        return self->impl.start;
    }
}

/**
 *  @brief  TODO
 * 
 *  @param[in]  self
 *
 *  @return
 */
void *vptr_back(vptr_t *self) {
    if (self->impl.start == self->impl.finish) {
        return NULL;
    } else {
        return self->impl.finish - 1;
    }
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  n
 *
 *  @return
 */
void *vptr_at(vptr_t *self, int n) {
    int size = self->impl.finish - self->impl.start;
    
    if (n < 0 || n >= size) {
        return NULL;
    } else {
        return self->impl.start + n;
    }
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *
 *  @return
 */
bool vptr_empty(vptr_t *self) {
    return self->impl.start == self->impl.finish;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  
 *  @return
 */
size_t vptr_size(vptr_t *self) {
    return self->impl.finish - self->impl.start;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *
 *  @return
 */
size_t vptr_capacity(vptr_t *self) {
    return self->impl.end_of_storage - self->impl.start;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  n
 *
 *  @return
 */
bool vptr_resize(vptr_t *self, size_t n) {
    void **start = NULL;
    size_t capacity = self->impl.end_of_storage - self->impl.start;

    if (n <= capacity) {
        return false;
    } else {
        size_t size = self->impl.finish - self->impl.start;

        start = realloc(self->impl.start, sizeof *start * n);
        assert(start);

        self->impl.start = start;
        self->impl.finish = self->impl.start + size;
        self->impl.end_of_storage = self->impl.start + n;
        
        return true;
    }
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *
 *  @return
 */
bool vptr_shrink_to_fit(vptr_t *self) {
    size_t capacity = self->impl.end_of_storage - self->impl.start;
    size_t size = self->impl.finish - self->impl.start;

    if (capacity <= size) {
        return false;
    } else {
        return vptr_resize(self, size);
    }
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *
 *  @return
 */
vptr_iter_t vptr_begin(vptr_t *self) {
    return self->impl.start;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *
 *  @return
 */
vptr_iter_t vptr_end(vptr_t *self) {
    return self->impl.finish;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  it
 *  @param[in]  valaddr
 *
 *  @return
 */
vptr_iter_t vptr_insert(vptr_t *self, vptr_iter_t it, void *valaddr) {
    void **val = valaddr;

    if (self->impl.finish == self->impl.end_of_storage) {
        size_t capacity = self->impl.finish - self->impl.start;
        vptr_resize(self, capacity * 2);
    }
    
    memmove(it + 1, it, sizeof *it * (self->impl.finish - it));
    *(it) = *(val);
    
    ++self->impl.finish;
   
    return it;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  it
 *  @param[out] tmp
 *
 *  @return
 */
vptr_iter_t vptr_erase(vptr_t *self, vptr_iter_t it, void *tmp) {
    void **temp = tmp;

    if (self->impl.start == self->impl.finish) {
        *(temp) = NULL;
    } else {
        (*temp) = (*it);
        
        --self->impl.finish;
        memmove(it, it + 1, sizeof *it * (self->impl.finish - it));
    }

    return it;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  valaddr
 *
 *  @return
 */
void vptr_pushb(vptr_t *self, void *valaddr) {
    void **val = valaddr;

    if (self->impl.finish == self->impl.end_of_storage) {
        size_t capacity = self->impl.finish - self->impl.start;
        vptr_resize(self, capacity * 2);
    }

    *(self->impl.finish++) = (*val);
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[out]  tmp
 */
void vptr_popb(vptr_t *self, void *tmp) {
    void **temp = tmp;
    
    if (self->impl.start == self->impl.finish) {
        *(temp) = NULL;
    } else {
        *(temp) = *(--self->impl.finish);
    }
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  valaddr
 */
void vptr_pushf(vptr_t *self, void *valaddr) {
    void **val = valaddr;
    size_t capacity = self->impl.finish - self->impl.start;

    if (self->impl.finish == self->impl.end_of_storage) {
        vptr_resize(self, capacity * 2);
        capacity = self->impl.finish - self->impl.start;
    }

    memmove(self->impl.start + 1, 
            self->impl.start, 
            sizeof *self->impl.start * (capacity));

    *(self->impl.start) = *(val);

    ++self->impl.finish;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[out] tmp
 */
void vptr_popf(vptr_t *self, void *tmp) {
    void **temp = tmp;
    size_t capacity = self->impl.finish - self->impl.start;

    if (self->impl.start == self->impl.finish) {
        *(temp) = NULL;
    } else {
        *(temp) = *(self->impl.start);

        memmove(self->impl.start, 
                self->impl.start + 1, 
                sizeof *self->impl.start * (capacity));

        --self->impl.finish;
    }
}

/**
 *  @brief  TODO
 *
 *  @param[in]  base
 *  @param[in]  cmpfn
 *  @param[in]  valaddr
 *
 *  @return
 */
int vptr_search(vptr_iter_t base, int (*cmpfn)(const void *, const void *), void *valaddr) {
    int i = 0;

    while (base + (i++)) {
        if (cmpfn(base + i, valaddr) == 0) {
            break;
        }
    }

    return i >= 0 ? i : -1;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  base
 *  @param[in]  cmpfn
 *  @param[in]  valaddr
 *
 *  @return
 */
vptr_iter_t vptr_find(vptr_iter_t base, int (*cmpfn)(const void *, const void *), void *valaddr) {
    int i = 0;

    while (base + (i++)) {
        if (cmpfn(base + i, valaddr) == 0) {
            break;
        }
    }

    return base + i ? base + i : NULL;
}

/**
 *  @brief  TODO
 *
 *  @param[in]  self
 *  @param[in]  cmpfn
 *
 *  @return
 */
vptr_iter_t vptr_qsort(vptr_t *self, int (*cmpfn)(const void *, const void *)) {
    qsort(self->impl.start, 
          self->impl.finish - self->impl.start, 
          sizeof *self->impl.start, cmpfn);

    return vptr_begin(self);
}

