#ifndef VECTOR_UINT16_H
#define VECTOR_UINT16_H

typedef struct vector_uint16_t vector_uint16_t;
typedef struct vector_uint16_t *vector_ptr_uint16_t;
typedef struct vector_uint16_t **vector_dptr_uint16_t;


vector_uint16_t *vnew_uint16_t(void);
vector_uint16_t *vnewr_uint16_t(size_t n);
vector_uint16_t *vnewfill_uint16_t(size_t n, uint16_t val);
vector_uint16_t *vnewfillptr_uint16_t(size_t n, uint16_t *valaddr);
vector_uint16_t *vnewrnge_uint16_t(iterator first, iterator last);
vector_uint16_t *vnewcopy_uint16_t(vector_uint16_t *v);
vector_uint16_t *vnewmove_uint16_t(vector_uint16_t **v);


void vdelete_uint16_t(vector_uint16_t **v);


iterator vbegin_uint16_t(vector_uint16_t *v);
iterator vend_uint16_t(vector_uint16_t *v);


size_t vsize_uint16_t(vector_uint16_t *v);
size_t vmaxsize_uint16_t(vector_uint16_t *v);


void vresize_uint16_t(vector_uint16_t *v, size_t n);
void vresizefill_uint16_t(vector_uint16_t *v, size_t n, uint16_t val);
void vresizefillptr_uint16_t(vector_uint16_t *v, size_t n, uint16_t *valaddr);


size_t vcapacity_uint16_t(vector_uint16_t *v);
bool vempty_uint16_t(vector_uint16_t *v);


void vreserve_uint16_t(vector_uint16_t *v, size_t n);
void vshrinktofit_uint16_t(vector_uint16_t *v);


uint16_t *vat_uint16_t(vector_uint16_t *v, size_t n);
uint16_t *vfront_uint16_t(vector_uint16_t *v);
uint16_t *vback_uint16_t(vector_uint16_t *v);
uint16_t **vdata_uint16_t(vector_uint16_t *v);


const uint16_t *vatconst_uint16_t(vector_uint16_t *v, size_t n);
const uint16_t *vfrontconst_uint16_t(vector_uint16_t *v);
const uint16_t *vbackconst_uint16_t(vector_uint16_t *v);
const uint16_t **vdataconst_uint16_t(vector_uint16_t *v);


void vassignrnge_uint16_t(vector_uint16_t *v, iterator first, iterator last);
void vassignfill_uint16_t(vector_uint16_t *v, size_t n, uint16_t val);
void vassignfillptr_uint16_t(vector_uint16_t *v, size_t n, uint16_t *valaddr);


void vpushb_uint16_t(vector_uint16_t *v, uint16_t val);
void vpushbptr_uint16_t(vector_uint16_t *v, uint16_t *valaddr);
void vpopb_uint16_t(vector_uint16_t *v);


iterator vinsert_uint16_t(vector_uint16_t *v, iterator pos, uint16_t val);
iterator vinsertptr_uint16_t(vector_uint16_t *v, iterator pos, uint16_t *val);
iterator vinsertfill_uint16_t(vector_uint16_t *v, iterator pos, size_t n, uint16_t val);
iterator vinsertfillptr_uint16_t(vector_uint16_t *v, iterator pos, size_t n, uint16_t *valaddr);
iterator vinsertrnge_uint16_t(vector_uint16_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint16_t(vector_uint16_t *v, iterator pos, uint16_t *valaddr);


iterator verase_uint16_t(vector_uint16_t *v, iterator pos);
iterator verasernge_uint16_t(vector_uint16_t *v, iterator pos, iterator last);


void vswap_uint16_t(vector_uint16_t **v, vector_uint16_t **other);


void vclear_uint16_t(vector_uint16_t *v);


void vinsertat_uint16_t(vector_uint16_t *v, size_t index, uint16_t val);
void vinsertatptr_uint16_t(vector_uint16_t *v, size_t index, uint16_t *valaddr);
void veraseat_uint16_t(vector_uint16_t *v, size_t index);
void vreplaceat_uint16_t(vector_uint16_t *v, size_t index, uint16_t val);
void vreplaceatptr_uint16_t(vector_uint16_t *v, size_t index, uint16_t *valaddr);


void vswapelem_uint16_t(vector_uint16_t *v, size_t n1, size_t n2);


void vremove_uint16_t(vector_uint16_t *v, uint16_t val);
void vremoveif_uint16_t(vector_uint16_t *v, bool (*unary_predicate)(const void *));


vector_uint16_t *vmerge_uint16_t(vector_uint16_t *v, vector_uint16_t * other);
void vreverse_uint16_t(vector_uint16_t *v);


vector_uint16_t *varrtov_uint16_t(uint16_t *base, size_t length);
vector_uint16_t *vptrtov_uint16_t(uint16_t *base, size_t length, size_t capacity);


int vsearch_uint16_t(vector_uint16_t *v, uint16_t val);
void vsort_uint16_t(vector_uint16_t *v);


void vputs_uint16_t(vector_uint16_t *v);
void vputsf_uint16_t(vector_uint16_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint16_t(vector_uint16_t *v, FILE *dest);
void vfputsf_uint16_t(vector_uint16_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);


void *tmpl_vector_copy_uint16_t(void *arg, const void *other);
void tmpl_vector_dtor_uint16_t(void *arg);
void tmpl_vector_swap_uint16_t(void *s1, void *s2);
int tmpl_vector_compare_uint16_t(const void *c1, const void *c2);
void tmpl_vector_print_uint16_t(const void *arg, FILE *dest);


void vsetttbl_uint16_t(vector_uint16_t *v, struct typetable *ttbl);

size_t vgetwidth_uint16_t(vector_uint16_t *v);
copy_fn vgetcopy_uint16_t(vector_uint16_t *v);
dtor_fn vgetdtor_uint16_t(vector_uint16_t *v);
swap_fn vgetswap_uint16_t(vector_uint16_t *v);
compare_fn vgetcompare_uint16_t(vector_uint16_t *v);
print_fn vgetprint_uint16_t(vector_uint16_t *v);
struct typetable *vgetttbl_uint16_t(vector_uint16_t *v);


struct typetable *_vector_uint16_t_;
struct iterator_table *_vector_iterator_uint16_t_;

#endif /* VECTOR_UINT16_H */