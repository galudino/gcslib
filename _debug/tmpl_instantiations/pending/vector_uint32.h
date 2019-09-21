#ifndef VECTOR_UINT32_H
#define VECTOR_UINT32_H

typedef struct vector_uint32_t vector_uint32_t;
typedef struct vector_uint32_t *vector_ptr_uint32_t;
typedef struct vector_uint32_t **vector_dptr_uint32_t;


vector_uint32_t *vnew_uint32_t(void);
vector_uint32_t *vnewr_uint32_t(size_t n);
vector_uint32_t *vnewfill_uint32_t(size_t n, uint32_t val);
vector_uint32_t *vnewfillptr_uint32_t(size_t n, uint32_t *valaddr);
vector_uint32_t *vnewrnge_uint32_t(iterator first, iterator last);
vector_uint32_t *vnewcopy_uint32_t(vector_uint32_t *v);
vector_uint32_t *vnewmove_uint32_t(vector_uint32_t **v);


void vdelete_uint32_t(vector_uint32_t **v);


iterator vbegin_uint32_t(vector_uint32_t *v);
iterator vend_uint32_t(vector_uint32_t *v);


size_t vsize_uint32_t(vector_uint32_t *v);
size_t vmaxsize_uint32_t(vector_uint32_t *v);


void vresize_uint32_t(vector_uint32_t *v, size_t n);
void vresizefill_uint32_t(vector_uint32_t *v, size_t n, uint32_t val);
void vresizefillptr_uint32_t(vector_uint32_t *v, size_t n, uint32_t *valaddr);


size_t vcapacity_uint32_t(vector_uint32_t *v);
bool vempty_uint32_t(vector_uint32_t *v);


void vreserve_uint32_t(vector_uint32_t *v, size_t n);
void vshrinktofit_uint32_t(vector_uint32_t *v);


uint32_t *vat_uint32_t(vector_uint32_t *v, size_t n);
uint32_t *vfront_uint32_t(vector_uint32_t *v);
uint32_t *vback_uint32_t(vector_uint32_t *v);
uint32_t **vdata_uint32_t(vector_uint32_t *v);


const uint32_t *vatconst_uint32_t(vector_uint32_t *v, size_t n);
const uint32_t *vfrontconst_uint32_t(vector_uint32_t *v);
const uint32_t *vbackconst_uint32_t(vector_uint32_t *v);
const uint32_t **vdataconst_uint32_t(vector_uint32_t *v);


void vassignrnge_uint32_t(vector_uint32_t *v, iterator first, iterator last);
void vassignfill_uint32_t(vector_uint32_t *v, size_t n, uint32_t val);
void vassignfillptr_uint32_t(vector_uint32_t *v, size_t n, uint32_t *valaddr);


void vpushb_uint32_t(vector_uint32_t *v, uint32_t val);
void vpushbptr_uint32_t(vector_uint32_t *v, uint32_t *valaddr);
void vpopb_uint32_t(vector_uint32_t *v);


iterator vinsert_uint32_t(vector_uint32_t *v, iterator pos, uint32_t val);
iterator vinsertptr_uint32_t(vector_uint32_t *v, iterator pos, uint32_t *val);
iterator vinsertfill_uint32_t(vector_uint32_t *v, iterator pos, size_t n, uint32_t val);
iterator vinsertfillptr_uint32_t(vector_uint32_t *v, iterator pos, size_t n, uint32_t *valaddr);
iterator vinsertrnge_uint32_t(vector_uint32_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint32_t(vector_uint32_t *v, iterator pos, uint32_t *valaddr);


iterator verase_uint32_t(vector_uint32_t *v, iterator pos);
iterator verasernge_uint32_t(vector_uint32_t *v, iterator pos, iterator last);


void vswap_uint32_t(vector_uint32_t **v, vector_uint32_t **other);


void vclear_uint32_t(vector_uint32_t *v);


void vinsertat_uint32_t(vector_uint32_t *v, size_t index, uint32_t val);
void vinsertatptr_uint32_t(vector_uint32_t *v, size_t index, uint32_t *valaddr);
void veraseat_uint32_t(vector_uint32_t *v, size_t index);
void vreplaceat_uint32_t(vector_uint32_t *v, size_t index, uint32_t val);
void vreplaceatptr_uint32_t(vector_uint32_t *v, size_t index, uint32_t *valaddr);


void vswapelem_uint32_t(vector_uint32_t *v, size_t n1, size_t n2);


void vremove_uint32_t(vector_uint32_t *v, uint32_t val);
void vremoveif_uint32_t(vector_uint32_t *v, bool (*unary_predicate)(const void *));


vector_uint32_t *vmerge_uint32_t(vector_uint32_t *v, vector_uint32_t * other);
void vreverse_uint32_t(vector_uint32_t *v);


vector_uint32_t *varrtov_uint32_t(uint32_t *base, size_t length);
vector_uint32_t *vptrtov_uint32_t(uint32_t *base, size_t length, size_t capacity);


int vsearch_uint32_t(vector_uint32_t *v, uint32_t val);
void vsort_uint32_t(vector_uint32_t *v);


void vputs_uint32_t(vector_uint32_t *v);
void vputsf_uint32_t(vector_uint32_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint32_t(vector_uint32_t *v, FILE *dest);
void vfputsf_uint32_t(vector_uint32_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);


void *tmpl_vector_copy_uint32_t(void *arg, const void *other);
void tmpl_vector_dtor_uint32_t(void *arg);
void tmpl_vector_swap_uint32_t(void *s1, void *s2);
int tmpl_vector_compare_uint32_t(const void *c1, const void *c2);
void tmpl_vector_print_uint32_t(const void *arg, FILE *dest);


void vsetttbl_uint32_t(vector_uint32_t *v, struct typetable *ttbl);

size_t vgetwidth_uint32_t(vector_uint32_t *v);
copy_fn vgetcopy_uint32_t(vector_uint32_t *v);
dtor_fn vgetdtor_uint32_t(vector_uint32_t *v);
swap_fn vgetswap_uint32_t(vector_uint32_t *v);
compare_fn vgetcompare_uint32_t(vector_uint32_t *v);
print_fn vgetprint_uint32_t(vector_uint32_t *v);
struct typetable *vgetttbl_uint32_t(vector_uint32_t *v);


struct typetable *_vector_uint32_t_;
struct iterator_table *_vector_iterator_uint32_t_;

#endif /* VECTOR_UINT32_H */