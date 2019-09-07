/**
 *  @file       list.c
 *  @brief      Source file for a doubly-linked list ADT
 *
 *  @author     Gemuele Aludino
 *  @date       07 Sep 2019
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

#include "list.h"
#include "iterator.h"
#include "utils.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 *  @struct     list
 *  @brief      Represents a doubly-linked list ADT
 * 
 *  Note that struct list is opaque -- 
 *  its fields cannot be accessed directly,
 *  nor can instances of struct list be created on the stack. 
 *  This is done to enforce encapsulation.
 */
struct list {
    struct list_node impl;
    struct typetable *ttbl;
};

/**
 *  static func prototypes
 */


/**
 *  ttbl_list and _list_ ttbl pointer
 */


/**
 *  static list iterator func prototypes
 */


/**
 *  list iterator table and _list_iterator_ pointer
 */


/**
 *  public list func defs
 */


/**
 *  public list typetable func defs
 */


/**
 *  static list func defs
 */


/**
 *  static list iterator func defs
 */
