
#ifndef T
#error "Specify the vector type"
#endif

#ifndef TEMPLATE_TYPE
#error "Specify the vector template type"
#endif

#ifndef TEMPLATE_FUNCTION
#error "Specify the vector template function"
#endif

typedef struct TEMPLATE_TYPE(vector)
{
	T* stor_begin;
	T* stor_end;
	T* end;
} TEMPLATE_TYPE(vector);

/*
 * allocation
 */
int TEMPLATE_FUNCTION(vector, init)(TEMPLATE_TYPE(vector) *v, int size);
int TEMPLATE_FUNCTION(vector, init_copy)(TEMPLATE_TYPE(vector) *v, T *data, int length);
void TEMPLATE_FUNCTION(vector, destroy)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, copy_to)(const TEMPLATE_TYPE(vector) *v, T *to);
int TEMPLATE_FUNCTION(vector, copy)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from);
int TEMPLATE_FUNCTION(vector,update)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from);

/*
 * accessing element
 */
#ifndef VECTOR
#define VECTOR(a) ((a).stor_begin)
#endif
T TEMPLATE_FUNCTION(vector, e)(TEMPLATE_TYPE(vector) *v, int pos);
T TEMPLATE_FUNCTION(vector, tail)(TEMPLATE_TYPE(vector) *v);
T* TEMPLATE_FUNCTION(vector, e_ptr)(TEMPLATE_TYPE(vector) *v, int pos);
void TEMPLATE_FUNCTION(vector, set)(TEMPLATE_TYPE(vector) *v, int pos, T value);
int TEMPLATE_FUNCTION(vector,get_interval)(const TEMPLATE_TYPE(vector) *v,TEMPLATE_TYPE(vector) *res, int from, int to);

/*
 * initializing elements
 */
void TEMPLATE_FUNCTION(vector, null)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, fill)(TEMPLATE_TYPE(vector) *v, T e);

/*
 * Exchange Element
 */
T* TEMPLATE_FUNCTION(vector, new_item)(TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, insert)(TEMPLATE_TYPE(vector) *v, int pos, T value);
int TEMPLATE_FUNCTION(vector, swap_elements)(TEMPLATE_TYPE(vector) *v, int i, int j);
int TEMPLATE_FUNCTION(vector, push_back)(TEMPLATE_TYPE(vector) *v, T e);
T TEMPLATE_FUNCTION(vector, pop_back)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, remove)(TEMPLATE_TYPE(vector) *v, int elem);

/*
 * properties
 */
int TEMPLATE_FUNCTION(vector, contains_op)(const TEMPLATE_TYPE(vector) *v, T e, int (*op)(const void*, const void*));
int TEMPLATE_FUNCTION(vector, empty)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, size)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, capacity)(const TEMPLATE_TYPE(vector) *v);

/*
 * Resize Operations
 */
int TEMPLATE_FUNCTION(vector, reserve)(TEMPLATE_TYPE(vector) *v, int size);
void TEMPLATE_FUNCTION(vector, clear)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, resize)(TEMPLATE_TYPE(vector) *v, int newsize);
void TEMPLATE_FUNCTION(vector, resize_min)(TEMPLATE_TYPE(vector) *v);
void TEMPLATE_FUNCTION(vector, remove_section)(TEMPLATE_TYPE(vector) *v, int from, int to);
int TEMPLATE_FUNCTION(vector, append)(TEMPLATE_TYPE(vector) *from, TEMPLATE_TYPE(vector) *to);
int TEMPLATE_FUNCTION(vector,move_interval)(TEMPLATE_TYPE(vector) *v, int begin, int end, int to);
int TEMPLATE_FUNCTION(vector,move_interval2)(TEMPLATE_TYPE(vector) *v, int begin, long int end, int to);
void TEMPLATE_FUNCTION(vector, adjust)(TEMPLATE_TYPE(vector) *v, int newsize);


