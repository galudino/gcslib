#ifndef VECTOR_UINT64_H
#define VECTOR_UINT64_H

typedef struct vector_uint64_t vector_uint64_t;
typedef struct vector_uint64_t *vector_ptr_uint64_t;
typedef struct vector_uint64_t **vector_dptr_uint64_t;

vector_uint64_t *vnew_uint64_t(void);
vector_uint64_t *vnewr_uint64_t(size_t n);
vector_uint64_t *vnewfill_uint64_t(size_t n, uint64_t val);
vector_uint64_t *vnewfillptr_uint64_t(size_t n, uint64_t *valaddr);
vector_uint64_t *vnewrnge_uint64_t(iterator first, iterator last);
vector_uint64_t *vnewcopy_uint64_t(vector_uint64_t *v);
vector_uint64_t *vnewmove_uint64_t(vector_uint64_t **v);


void vdelete_uint64_t(vector_uint64_t **v);


iterator vbegin_uint64_t(vector_uint64_t *v);
iterator vend_uint64_t(vector_uint64_t *v);


size_t vsize_uint64_t(vector_uint64_t *v);
size_t vmaxsize_uint64_t(vector_uint64_t *v);


void vresize_uint64_t(vector_uint64_t *v, size_t n);
void vresizefill_uint64_t(vector_uint64_t *v, size_t n, uint64_t val);
void vresizefillptr_uint64_t(vector_uint64_t *v, size_t n, uint64_t *valaddr);


size_t vcapacity_uint64_t(vector_uint64_t *v);
bool vempty_uint64_t(vector_uint64_t *v);


void vreserve_uint64_t(vector_uint64_t *v, size_t n);
void vshrinktofit_uint64_t(vector_uint64_t *v);


uint64_t *vat_uint64_t(vector_uint64_t *v, size_t n);
uint64_t *vfront_uint64_t(vector_uint64_t *v);
uint64_t *vback_uint64_t(vector_uint64_t *v);
uint64_t **vdata_uint64_t(vector_uint64_t *v);


const uint64_t *vatconst_uint64_t(vector_uint64_t *v, size_t n);
const uint64_t *vfrontconst_uint64_t(vector_uint64_t *v);
const uint64_t *vbackconst_uint64_t(vector_uint64_t *v);
const uint64_t **vdataconst_uint64_t(vector_uint64_t *v);


void vassignrnge_uint64_t(vector_uint64_t *v, iterator first, iterator last);
void vassignfill_uint64_t(vector_uint64_t *v, size_t n, uint64_t val);
void vassignfillptr_uint64_t(vector_uint64_t *v, size_t n, uint64_t *valaddr);


void vpushb_uint64_t(vector_uint64_t *v, uint64_t val);
void vpushbptr_uint64_t(vector_uint64_t *v, uint64_t *valaddr);
void vpopb_uint64_t(vector_uint64_t *v);


iterator vinsert_uint64_t(vector_uint64_t *v, iterator pos, uint64_t val);
iterator vinsertptr_uint64_t(vector_uint64_t *v, iterator pos, uint64_t *val);
iterator vinsertfill_uint64_t(vector_uint64_t *v, iterator pos, size_t n, uint64_t val);
iterator vinsertfillptr_uint64_t(vector_uint64_t *v, iterator pos, size_t n, uint64_t *valaddr);
iterator vinsertrnge_uint64_t(vector_uint64_t *v, iterator pos, iterator first,
                         iterator last);
iterator vinsertmove_uint64_t(vector_uint64_t *v, iterator pos, uint64_t *valaddr);


iterator verase_uint64_t(vector_uint64_t *v, iterator pos);
iterator verasernge_uint64_t(vector_uint64_t *v, iterator pos, iterator last);


void vswap_uint64_t(vector_uint64_t **v, vector_uint64_t **other);


void vclear_uint64_t(vector_uint64_t *v);


void vinsertat_uint64_t(vector_uint64_t *v, size_t index, uint64_t val);
void vinsertatptr_uint64_t(vector_uint64_t *v, size_t index, uint64_t *valaddr);
void veraseat_uint64_t(vector_uint64_t *v, size_t index);
void vreplaceat_uint64_t(vector_uint64_t *v, size_t index, uint64_t val);
void vreplaceatptr_uint64_t(vector_uint64_t *v, size_t index, uint64_t *valaddr);


void vswapelem_uint64_t(vector_uint64_t *v, size_t n1, size_t n2);


void vremove_uint64_t(vector_uint64_t *v, uint64_t val);
void vremoveif_uint64_t(vector_uint64_t *v, bool (*unary_predicate)(const void *));


vector_uint64_t *vmerge_uint64_t(vector_uint64_t *v, vector_uint64_t * other);
void vreverse_uint64_t(vector_uint64_t *v);


vector_uint64_t *varrtov_uint64_t(uint64_t *base, size_t length);
vector_uint64_t *vptrtov_uint64_t(uint64_t *base, size_t length, size_t capacity);


int vsearch_uint64_t(vector_uint64_t *v, uint64_t val);
void vsort_uint64_t(vector_uint64_t *v);


void vputs_uint64_t(vector_uint64_t *v);
void vputsf_uint64_t(vector_uint64_t *v, const char *before, const char *after,
               const char *postelem, const char *empty, size_t breaklim);
void vfputs_uint64_t(vector_uint64_t *v, FILE *dest);
void vfputsf_uint64_t(vector_uint64_t *v, FILE *dest, const char *before,
                const char *after, const char *postelem, const char *empty,
                size_t breaklim);


void *tmpl_vector_copy_uint64_t(void *arg, const void *other);
void tmpl_vector_dtor_uint64_t(void *arg);
void tmpl_vector_swap_uint64_t(void *s1, void *s2);
int tmpl_vector_compare_uint64_t(const void *c1, const void *c2);
void tmpl_vector_print_uint64_t(const void *arg, FILE *dest);


void vsetttbl_uint64_t(vector_uint64_t *v, struct typetable *ttbl);

size_t vgetwidth_uint64_t(vector_uint64_t *v);
copy_fn vgetcopy_uint64_t(vector_uint64_t *v);
dtor_fn vgetdtor_uint64_t(vector_uint64_t *v);
swap_fn vgetswap_uint64_t(vector_uint64_t *v);
compare_fn vgetcompare_uint64_t(vector_uint64_t *v);
print_fn vgetprint_uint64_t(vector_uint64_t *v);
struct typetable *vgetttbl_uint64_t(vector_uint64_t *v);


struct typetable *_vector_uint64_t_;
struct iterator_table *_vector_iterator_uint64_t_;


#endif /* VECTOR_UINT64_H */