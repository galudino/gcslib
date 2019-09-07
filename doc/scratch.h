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




// end scratch
