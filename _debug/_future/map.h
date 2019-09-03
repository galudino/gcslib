/**
 *  @file       map.h
 *  @brief      Header file for an associative array (key-value) ADT
 *
 *  @author     Gemuele Aludino
 *  @date       17 Feb 2019
 *  @copyright  Copyright © 2019 Gemuele Aludino
 */

#ifndef MAP_H
#define MAP_H

#include <stdarg.h>
#include <stdbool.h>

#include "utils.h"
#include "iterator.h"
#include "pair.h"

typedef struct map      map;
typedef struct map *    map_ptr;
typedef struct map **   map_dptr;

enum entry_element {
    KEY, VALUE, KEY_VALUE
};

/**< map: constructors */
map *           m_new           (struct typetable *ttbl_key,
                                 struct typetable *ttbl_val);

map *           m_newrnge       (struct typetable *ttbl_key,
                                 struct typetable *ttbl_val,
                                 iterator *first, iterator *last);

map *           m_newcopy       (map *m);

map *           m_newmove       (map **m);

/**< map: destructor */
void            m_delete        (void *arg);

/**< map: iterator functions */
iterator *      m_begin         (map *m);
iterator *      m_end           (map *m);

/**< map: capacity-based functions */
size_t          m_empty         (map *m);
size_t          m_size          (map *m);
size_t          m_maxsize       (map *m);

/**< map: element access functions */
void *          m_at            (map *m, const void *key);

/**< map: element access functions w/ const qualifier */
const void *    m_at_const      (map *m, const void *key);

/**< map: modifiers - insertion */
pair *          m_insert        (map *m, pair *kval);
iterator *      m_inserthint    (iterator *pos, pair *kval);
void            m_insertrnge    (iterator *first, iterator *last);

/**< map: modifiers - erasure */
void            m_erase         (iterator *pos);
void            m_erasebykey    (const void *key);
void            m_erasernge     (iterator *first, iterator *last);

/**< map: container swappage */
void            m_swap          (map **m, map **other);

/**< map: clear container */
void            m_clear         (map *m);

/**< map: observers - retrieve comparators */
compare_fn      m_key_comp      (map *m);
compare_fn      m_value_comp    (map *m);

/**< map: operations */
iterator *      m_find          (map *m, const void *key);
size_t          m_count         (map *m, const void *key);
iterator *      m_lowerbound    (map *m, const void *key);
iterator *      m_upperbound    (map *m, const void *key);
pair *          m_equalrange    (map *m, const void *key);

/**< map: custom print functions - output to FILE stream */
void           m_puts          (map *m);

void           m_putsf         (map *m,
                                const char *before,
                                const char *after,
                                const char *postelem,
                                const char *empty,
                                size_t breaklim);

void           m_fputs         (map *m, FILE *dest);

void           m_fputsf        (map *m, FILE *dest,
                                const char *before,
                                const char *after,
                                const char *postelem,
                                const char *empty,
                                size_t breaklim);

/**< map: required function prototypes for (struct typetable) */
int             map_compare    (const void *c1, const void *c2);
void *          map_copy       (const void *arg);
void            map_print      (const void *arg, FILE *dest);
void            map_delete     (void *arg);

/**< struct typetable/struct iterator_table: ptrs to vtables */
struct typetable   *_map_;
struct iterator_table   *_map_iterator_;

#endif /* MAP_H */
