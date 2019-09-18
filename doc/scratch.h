/**
 *  Scratch notes go here.
 *  It's named with a .h extension so syntax highlighting turns on by default.
 */

// v_remove revision:
    //
    //  Need to come up with something more efficient than this.
    //  Running v_erase_at in a loop means
    //      When element to erase is found at index i
    //      Erase element at i
    //      Then shift elements from [i + 1, v_size(v))
    //          one block left
    //      Repeat for all elements found.
    //      (that's more shifting than necessary)
    //
    //  An alternative idea would be:
    //      In one pass, delete and/or set NULL
    //      any element that matches that of valaddr
    //      Do not do any shifting of elements yet.
    //
    //      In the next pass,
    //          For each block
    //              Find a NULL block
    //                  When NULL block found,
    //                      Advance from NULL block n times
    //                      until a non-null block is found.
    //                          When non-NULL block found,
    //                          do v_swap_addr with NULL block
    //
    //  Another idea would be:
    //      Keep a bool del_index[v_size(v)] array.
    //      Destroy the elements that are to be removed in one pass.
    //      Mark the indices in the del_index array.
    //
    //      In the next pass,
    //          Go through the vector and specifically visit
    //          the NULL indices, and v_swap_addr that NULL
    //          element with the first non-NULL element found.
    //          Repeat until there are no more NULLs to swap with
    //          Decrement the finish pointer based on how many
    //          elements were removed.


void v_assignrnge(vector *v, iterator first, iterator last) {
    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    void *sentinel = it_curr(last);
    void *curr = NULL;

    size_t old_size = v_size(v);
    size_t old_capacity = v_capacity(v);

    if (first.itbl != last.itbl) {
        /**
         *  Iterators first and last must refer to the same container,
         *  or else the cursor pointer will never meet the sentinel pointer
         *  that will end the copy loop.
         * 
         *  This also means that iterators first and last cannot refer to
         *  different container types (this is determiend by what itbls
         *  they each point to)
         */
        ERROR(__FILE__, "first and last must matching container types and refer to the same container.");
        return;
    }

    if (delta >= old_capacity) {
        /**
         *  If range [first, last) exceeds that of old_capacity,
         *  resize the vector to accomodate the new elements
         */
        v_resize(v, delta);
    }

    /* move the finish pointer to the beginning of the vector */
    v->impl.finish = v->impl.start;

    if (ttbl_first->copy && ttbl_first->dtor) {
        /**
         *  If items were deep copied, existing memory
         *  will be released before copying new data
         */
        while ((curr = it_curr(first)) != sentinel)  {
            ttbl_first->dtor(v->impl.finish);
            ttbl_first->copy(v->impl.finish, curr);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            it_incr(&first);
        }
    } else {
        /**
         *  If items were shallow copied,
         *  blocks will be overwritten immediately
         */
        while ((curr = it_curr(first)) != sentinel)  {
            memcpy(v->impl.finish, curr, v->ttbl->width);

            v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            it_incr(&first);
        };
    }

    /* restoring finish pointer to appropriate location */
    v->impl.finish = delta < old_size
    ? (char *)(v->impl.start) + (old_size * v->ttbl->width) : v->impl.finish;
}

void v_assignfill(vector *v, size_t n, const void *valaddr) {
    size_t old_size = v_size(v);
    size_t old_capacity = v_capacity(v);

    void *newstart = NULL;
    void *sentinel = NULL;

    assert(valaddr);

    if (n >= old_size || n >= old_capacity) {
        /**
         *  elements from [0, size) are completely overwritten and replaced with
         *  valaddr. If elements were deep copied, their memory must be released first.
         */
        if (v->ttbl->dtor) {
            v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);

            while (v->impl.finish != v->impl.start) {
                v->ttbl->dtor(v->impl.finish);
                v->impl.finish = (char *)(v->impl.finish) - (v->ttbl->width);
            }

            v->ttbl->dtor(v->impl.start);
        }

        free(v->impl.start);
        v->impl.start = NULL;

        newstart = calloc(n, v->ttbl->width);
        assert(newstart);

        v->impl.start = newstart;
        v->impl.finish = v->impl.start;
        v->impl.end_of_storage = (char *)(v->impl.start) + (n * v->ttbl->width);

        if (v->ttbl->copy) {
            /* deep copy */
            while (v->impl.finish != v->impl.end_of_storage) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            /* shallow copy */
            while (v->impl.finish != v->impl.end_of_storage) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }
    } else {
        if (v->ttbl->dtor) {
            /**
             *  Elements from [0, n) are completely overwritten and replaced with
             *  valaddr. If elements were deep copied, their memory must be released first.
             */
            sentinel = (char *)(v->impl.start) + ((n + 1) * v->ttbl->width);
            v->impl.finish = v->impl.start;

            while (v->impl.finish != sentinel) {
                v->ttbl->dtor(v->impl.finish);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }

            v->ttbl->dtor(v->impl.start);
        } else {
            /**
             *  If elements are shallow copied,
             *  we skip memory deallocation and move the finish pointer
             *  to the address of the start pointer
             */
            v->impl.finish = v->impl.start;
        }

        v->impl.finish = v->impl.start;
        sentinel = (char *)(v->impl.start) + ((n + 1) * v->ttbl->width);
        
        if (v->ttbl->copy) {
            /* deep copy */
            while (v->impl.finish != sentinel) {
                v->ttbl->copy(v->impl.finish, valaddr);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        } else {
            /* shallow copy */
            while (v->impl.finish != sentinel) {
                memcpy(v->impl.finish, valaddr, v->ttbl->width);
                v->impl.finish = (char *)(v->impl.finish) + (v->ttbl->width);
            }
        }

        /* restoring the finish pointer to its former address */
        v->impl.finish = (char *)(v->impl.start) + (old_size * v->ttbl->width);
    }
}


static void v_initialize_dispatch_fill(vector *v, size_t n, const void *valaddr);
static void v_initialize_dispatch_rnge(vector *v, iterator first, iterator last);
static void v_range_initialize(vector *v, iterator first, iterator last);

static void v_range_check(vector *v, size_t n);

static void v_assign_aux(vector *v, iterator first, iterator last);
static void v_assign_dispatch_fill(vector *v, size_t n, const void *valaddr);
static void v_assign_dispatch_rnge(vector *v, iterator first, iterator last);

static void v_insert_aux(vector *v, iterator pos, const void *valaddr);
static void v_insert_dispatch_fill(vector *v, iterator pos, size_t n, const void *valaddr);
static void v_insert_dispatch_rnge(vector *v, iterator first, iterator last);

static void v_fill_assign(vector *v, size_t n, const void *valaddr);
static void v_fill_insert(vector *v, iterator pos, size_t n, const void *valaddr);

static void v_range_insert(vector *v, iterator first, iterator last);

static void v_erase_at_end(vector *v, void *pos);

    /*
    ALTERNATE CODE for v_assignrnge
    int delta = it_distance(&first, &last);
    struct typetable *ttbl_first = it_get_ttbl(first);

    if (first.itbl != last.itbl) {
        ERROR(__FILE__, "first and last must match container types and refer to the same container.");
        return;
    }

    v_clear(v);
    free(v->impl.start);
    v->impl.start = NULL;

    v->impl.start = allocate_and_copy(v->ttbl, delta, first.curr, last.curr);
    v->impl.finish = (char *)(v->impl.start) + (delta + v->ttbl->width);
    v->impl.end_of_storage = v->impl.finish;
    */


   /*
    ALTERNATE CODE for v_assignrnge
    vector *tmp = v_newrnge(first, last);
    v_swap(&v, &tmp);
    v_delete(&tmp);
   */



    /*
    ALTERNATE CODE for v_assignfill
    just make a new vector tmp, filled with n elements,
    swap with v, and delete tmp.
    vector *tmp = v_newfill(ttbl, n, valaddr);
    v_swap(&v, &tmp);
    v_delete(&tmp);
    */

   static void v_range_check(vector *v, size_t n) {
    assert(v);
    size_t size = v_size(v);
    if (n >= v_size(v)) {
        char str[256];
        sprintf(str, "Index out of bounds - %lu is greater than or equal to %lu.", n, size);
        ERROR(__FILE__, str);
    }
}



// end scratch
