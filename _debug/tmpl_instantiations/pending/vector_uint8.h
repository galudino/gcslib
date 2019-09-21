#ifndef VECTOR_UINT8_H
#define VECTOR_UINT8_H

ypedef struct vector_uint8_t vector_uint8_t;
typedef struct vector_uint8_t *vector_ptr_uint8_t;
typedef struct vector_uint8_t **vector_dptr_uint8_t;


vector_uint8_t *vnew_uint8_t(void);
vector_uint8_t *vnewr_uint8_t(size_t n);
vector_uint8_t *vnewfill_uint8_t(size_t n, uint8_t val);
vector_uint8_t *vnewfillptr_uint8_t(size_t n, uint8_t *valaddr);
vector_uint8_t *vnewrnge_uint8_t(iterator first, iterator last);
vector_uint8_t *vnewcopy_uint8_t(vector_uint8_t *v);
vector_uint8_t *vnewmove_uint8_t(vector_uint8_t **v);


void vdelete_uint8_t(vector_uint8_t **v);


iterator vbegin_uint8_t(vector_uint8_t *v);
iterator vend_uint8_t(vector_uint8_t *v);


size_t vsize_uint8_t(vector_uint8_t *v);
size_t vmaxsize_uint8_t(vector_uint8_t *v);


void vresize_uint8_t(vector_uint8_t *v, size_t n);
void vresizefill_uint8_t(vector_uint8_t *v, size_t n, uint8_t val);
void vresizefillptr_uint8_t(vector_uint8_t *v, size_t n, uint8_t *valaddr);


size_t vcapacity_uint8_t(vector_uint8_t *v);
bool vempty_uint8_t(vector_uint8_t *v);


void vreserve_uint8_t(vector_uint8_t *v, size_t n);
void vshrinktofit_uint8_t(vector_uint8_t *v);


uint8_t *vat_uint8_t(vector_uint8_t *v, size_t n);
uint8_t *vfront_uint8_t(vector_uint8_t *v);
uint8_t *vback_uint8_t(vector_uint8_t *v);
uint8_t **vdata_uint8_t(vector_uint8_t *v);


const uint8_t *vatconst_uint8_t(vector_uint8_t *v, size_t n);
const uint8_t *vfrontconst_uint8_t(vector_uint8_t *v);
const uint8_t *vbackconst_uint8_t(vector_uint8_t *v);
const uint8_t **vdataconst_uint8_t(vector_uint8_t *v);


void vassignrnge_uint8_t(vector_uint8_t *v, iterator first, iterator last);
void vassignfill_uint8_t(vector_uint8_t *v, size_t n, uint8_t val);
void vassignfillptr_uint8_t(vector_uint8_t *v, size_t n, uint8_t *valaddr);


void vpushb_uint8_t(vector_uint8_t *v, uint8_t val);
void vpushbptr_uint8_t(vector_uint8_t *v, uint8_t *valaddr);
void vpopb_uint8_t(vector_uint8_t *v);


iterator vinsert_uint8_t(vector_uint8_t *v, iterator pos, uint8_t val);
iterator vinsertptr_uint8_t(vector_uint8_t *v, iterator pos, uint8_t *val);
iterator vinsertfill_uint8_t(vector_uint8_t *v, iterator pos, size_t n, uint8_t val);
iterator vinsertfillptr_uint8_t(vector_uint8_t *v, iterator pos, size_t n, uint8_t *valaddr);
iterator vinsertrnge_uint8_t(vector_uint8_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint8_t(vector_uint8_t *v, iterator pos, uint8_t *valaddr);


iterator verase_uint8_t(vector_uint8_t *v, iterator pos);
iterator verasernge_uint8_t(vector_uint8_t *v, iterator pos, iterator last);


void vswap_uint8_t(vector_uint8_t **v, vector_uint8_t **other);


void vclear_uint8_t(vector_uint8_t *v);


void vinsertat_uint8_t(vector_uint8_t *v, size_t index, uint8_t val);
void vinsertatptr_uint8_t(vector_uint8_t *v, size_t index, uint8_t *valaddr);
void veraseat_uint8_t(vector_uint8_t *v, size_t index);
void vreplaceat_uint8_t(vector_uint8_t *v, size_t index, uint8_t val);
void vreplaceatptr_uint8_t(vector_uint8_t *v, size_t index, uint8_t *valaddr);


void vswapelem_uint8_t(vector_uint8_t *v, size_t n1, size_t n2);


void vremove_uint8_t(vector_uint8_t *v, uint8_t val);
void vremoveif_uint8_t(vector_uint8_t *v, bool (*unary_predicate)(const void *));


vector_uint8_t *vmerge_uint8_t(vector_uint8_t *v, vector_uint8_t * other);
void vreverse_uint8_t(vector_uint8_t *v);


vector_uint8_t *varrtov_uint8_t(uint8_t *base, size_t length);
vector_uint8_t *vptrtov_uint8_t(uint8_t *base, size_t length, size_t capacity);


int vsearch_uint8_t(vector_uint8_t *v, uint8_t val);
void vsort_uint8_t(vector_uint8_t *v);


void vputs_uint8_t(vector_uint8_t *v);
void vputsf_uint8_t(vector_uint8_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint8_t(vector_uint8_t *v, FILE *dest);
void vfputsf_uint8_t(vector_uint8_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);


void *tmpl_vector_copy_uint8_t(void *arg, const void *other);
void tmpl_vector_dtor_uint8_t(void *arg);
void tmpl_vector_swap_uint8_t(void *s1, void *s2);
int tmpl_vector_compare_uint8_t(const void *c1, const void *c2);
void tmpl_vector_print_uint8_t(const void *arg, FILE *dest);


void vsetttbl_uint8_t(vector_uint8_t *v, struct typetable *ttbl);

size_t vgetwidth_uint8_t(vector_uint8_t *v);
copy_fn vgetcopy_uint8_t(vector_uint8_t *v);
dtor_fn vgetdtor_uint8_t(vector_uint8_t *v);
swap_fn vgetswap_uint8_t(vector_uint8_t *v);
compare_fn vgetcompare_uint8_t(vector_uint8_t *v);
print_fn vgetprint_uint8_t(vector_uint8_t *v);
struct typetable *vgetttbl_uint8_t(vector_uint8_t *v);


struct typetable *_vector_uint8_t_;
struct iterator_table *_vector_iterator_uint8_t_;

#endif /* VECTOR_UINT8_H */