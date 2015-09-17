#include <assert.h>
#include <stdarg.h>

#ifndef T
#error "Specify the vector type"
#endif

#ifndef TEMPLATE_TYPE
#error "Specify the vector template type"
#endif

#ifndef TEMPLATE_FUNCTION
#error "Specify the vector template function"
#endif


/*
 * vector_T_init - construct a vector, if success return 0.
 *
 * e.g. vector_int_init(&a,3);
 */
int TEMPLATE_FUNCTION(vector,init)(TEMPLATE_TYPE(vector) *v, int size)
{
	int alloc_size = size > 0 ? size :1;

	v->stor_begin = Calloc(alloc_size, T);
	assert(0 != v->stor_begin);

	v->stor_end = v->stor_begin + alloc_size;
	v->end = v->stor_begin + size;
	return 0;
}

/*
 * vector_T_init_copy
 */
int TEMPLATE_FUNCTION(vector, init_copy)(TEMPLATE_TYPE(vector) *v, T *data, int length)
{
	v->stor_begin = Calloc(length, T);
	assert(0 != v->stor_begin);

	v->stor_end = v->stor_begin + length;
	v->end = v->stor_end;
	memcpy(v->stor_begin, data, (size_t)length * sizeof(T));

	return 0;
}

/*
 * vector_T_copy_to
 */
void TEMPLATE_FUNCTION(vector, copy_to)(const TEMPLATE_TYPE(vector) *v, T *to)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	if (v->end != v->stor_begin)
		memcpy(to, v->stor_begin, sizeof(T) * (size_t)(v->end - v->stor_begin));
}

/*
 * vector_T_copy
 */
int TEMPLATE_FUNCTION(vector, copy)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from)
{
	assert(NULL != from);
	assert(NULL != from->stor_begin);
	TEMPLATE_FUNCTION(vector, init)(to, TEMPLATE_FUNCTION(vector, size)(from));
	assert(0 != to->stor_begin);

	memcpy(to->stor_begin, from->stor_begin, (size_t)TEMPLATE_FUNCTION(vector, size)(from) * sizeof(T));
	return 0;
}

/*
 * vector_T_update
 */
int TEMPLATE_FUNCTION(vector,update)(TEMPLATE_TYPE(vector) *to, const TEMPLATE_TYPE(vector) *from)
{
	int n = TEMPLATE_FUNCTION(vector,size)(from);
	TEMPLATE_FUNCTION(vector,resize)(to, n);
	memcpy(to->stor_begin, from->stor_begin, sizeof(T)*n);
	return 0;
}

/*
 * vector_T_move_interval
 */
int TEMPLATE_FUNCTION(vector,move_interval)(TEMPLATE_TYPE(vector) *v, int begin, int end, int to)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	memcpy(v->stor_begin+to, v->stor_begin+begin, sizeof(T) * (size_t) (end-begin));
	return 0;
}
/*
 * vector_T_move_interval2
 */
int TEMPLATE_FUNCTION(vector,move_interval2)(TEMPLATE_TYPE(vector) *v, int begin, long int end, int to)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	memmove(v->stor_begin+to, v->stor_begin+begin, sizeof(T) * (size_t) (end-begin));

	return 0;
}

/*
 * vector_T_get_interval
 */
int TEMPLATE_FUNCTION(vector,get_interval)(const TEMPLATE_TYPE(vector) *v,TEMPLATE_TYPE(vector) *res, int from, int to)
{
	TEMPLATE_FUNCTION(vector, resize)(res, to-from);
	memcpy(res->stor_begin, v->stor_begin+from, (size_t) (to-from) * sizeof(T));
	return 0;
}
/*
 * vector_T_destroy
 */
void TEMPLATE_FUNCTION(vector, destroy)(TEMPLATE_TYPE(vector) *v)
{
	assert(v != 0);
	if (v->stor_begin != 0) {
		Free(v->stor_begin);
		v->stor_begin = NULL;
		v->stor_end = NULL;
		v->end = NULL;
	}
}

/*
 * vector_T_capacity
 */
int TEMPLATE_FUNCTION(vector, capacity)(const TEMPLATE_TYPE(vector) *v)
{
	return v->stor_end - v->stor_begin;
}

/*
 * vector_T_contains_op
 */
int TEMPLATE_FUNCTION(vector, contains_op)(const TEMPLATE_TYPE(vector) *v, T e, int (*op)(const void*, const void*))
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	T *p = v->stor_begin;
	while (p < v->end) {
		if (0 == op(p,&e))
			return 1;
		p++;
	}
	return 0;
}

/*
 * vector_T_empty - Verify whether the size of the vector is zero.
 *
 * return Non-zero number(true) if the size of the vector is zero and zero(false) otherwise.
 */
int TEMPLATE_FUNCTION(vector, empty)(const TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	return v->stor_begin == v->end;
}

/*
 * vector_T_size
 */
int TEMPLATE_FUNCTION(vector, size)(const TEMPLATE_TYPE(vector)* v)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return v->end - v->stor_begin;
}

/*
 * vector_T_reserve
 */
int TEMPLATE_FUNCTION(vector, reserve)(TEMPLATE_TYPE(vector) *v, int size)
{
	int actual_size = TEMPLATE_FUNCTION(vector, size)(v);
	T *tmp;
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	if (size <= TEMPLATE_FUNCTION(vector, size)(v)) return 0;

	tmp = Realloc(v->stor_begin, (size_t)size, T);
	assert(0 != v->stor_begin);
	v->stor_begin = tmp;
	v->stor_end = v->stor_begin + size;
	v->end = v->stor_begin + actual_size;

	return 0;
}

/*
 * vector_T_clear
 */
void TEMPLATE_FUNCTION(vector, clear)(TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	v->end = v->stor_begin;
}

/*
 * vector_T_resize
 */
void TEMPLATE_FUNCTION(vector, resize)(TEMPLATE_TYPE(vector) *v, int newsize)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	TEMPLATE_FUNCTION(vector, reserve)(v, newsize);
	v->end = v->stor_begin + newsize;
}

void TEMPLATE_FUNCTION(vector, adjust)(TEMPLATE_TYPE(vector) *v, int newsize)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	if (newsize > TEMPLATE_FUNCTION(vector,capacity)(v))
		TEMPLATE_FUNCTION(vector, reserve)(v, newsize * 2);

	v->end = v->stor_begin + newsize;
}

/*
 * vector_T_resize_min - deallocate the unused memory of a vector
 */
void TEMPLATE_FUNCTION(vector, resize_min)(TEMPLATE_TYPE(vector) *v)
{
	if (v->stor_end == v->end) return;

	size_t size = (size_t)(v->end - v->stor_begin);
	T *tmp = Realloc(v->stor_begin, size, T);
	assert(0 != tmp);

	v->stor_begin = tmp;
	v->stor_end = v->end = v->stor_begin + size;
}

/*
 * vector_T_push_back
 */
int TEMPLATE_FUNCTION(vector, push_back)(TEMPLATE_TYPE(vector) *v, T e)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	/* full, allocate more storage */
	if (v->stor_end == v->end) {
		int new_size = TEMPLATE_FUNCTION(vector, size)(v) * 2;
		if (new_size == 0) new_size = 1;
		TEMPLATE_FUNCTION(vector, reserve)(v, new_size);
	}

	*(v->end) = e;
	v->end += 1;

	return 0;
}

/*
 * vector_T_pop_back - return and remove the last element of the vector
 */
T TEMPLATE_FUNCTION(vector, pop_back)(TEMPLATE_TYPE(vector) *v)
{
	T tmp;
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	assert(v->end != v->stor_begin);
	tmp = TEMPLATE_FUNCTION(vector, tail)(v);
	v->end -= 1;
	return tmp;
}

/*
 * vector_T_remove_section - NOT include to
 */
void TEMPLATE_FUNCTION(vector, remove_section)(TEMPLATE_TYPE(vector) *v, int from, int to)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	assert(to < TEMPLATE_FUNCTION(vector, size)(v) + 1);
	assert(from <= to);
	memmove(v->stor_begin + from, v->stor_begin + to, sizeof(T) * (size_t)(v->end - v->stor_begin - to));
	v->end -= (to - from);
}

/*
 * vector_T_remove
 */
void TEMPLATE_FUNCTION(vector, remove)(TEMPLATE_TYPE(vector) *v, int elem)
{
	TEMPLATE_FUNCTION(vector, remove_section)(v, elem, elem + 1);
}

/*
 * vector_T_swap_elements
 */
int TEMPLATE_FUNCTION(vector, swap_elements)(TEMPLATE_TYPE(vector) *v, int i, int j)
{
	T tmp = VECTOR(*v)[i];
	VECTOR(*v)[i] = VECTOR(*v)[j];
	VECTOR(*v)[j] = tmp;
	return 0;
}

/*
 * vector_T_new_item
 */
T* TEMPLATE_FUNCTION(vector, new_item)(TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	/* full, allocate more storage */
	if (v->stor_end == v->end) {
		int new_size = TEMPLATE_FUNCTION(vector, size)(v) * 2;
		if (new_size == 0) new_size = 1;
		TEMPLATE_FUNCTION(vector, reserve)(v, new_size);
	}

	v->end += 1;
	return (v->end - 1);
}

/*
 * vector_T_insert
 */
int TEMPLATE_FUNCTION(vector, insert)(TEMPLATE_TYPE(vector) *v, int pos, T value)
{
	size_t size = (size_t)TEMPLATE_FUNCTION(vector, size)(v);
	TEMPLATE_FUNCTION(vector, resize)(v, (long)size+1);
	if (pos < size)
		memmove(v->stor_begin+pos+1, v->stor_begin+pos, sizeof(T)*(size - (size_t)pos));
	v->stor_begin[pos] = value;
	return 0;
}

/*
 * vector_T_e - access to an element of the vector
 */
T TEMPLATE_FUNCTION(vector, e)(TEMPLATE_TYPE(vector) *v, int pos)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return *(v->stor_begin + pos);
}

/*
 * vector_T_tail - access to the last element of the vector
 */
T TEMPLATE_FUNCTION(vector, tail)(TEMPLATE_TYPE(vector) *v)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return *((v->end) - 1);
}

/*
 * vector_T_e_ptr - get the address of an element in the vector
 */
T* TEMPLATE_FUNCTION(vector, e_ptr)(TEMPLATE_TYPE(vector) *v, int pos)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	return v->stor_begin + pos;
}

/*
 * vector_T_set - assignment to an element of a vector
 */
void TEMPLATE_FUNCTION(vector, set)(TEMPLATE_TYPE(vector) *v, int pos, T value)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	*(v->stor_begin + pos) = value;
}


/*
 * vector_T_null - set the value of each element in the vector to zero
 */
void TEMPLATE_FUNCTION(vector, null)(TEMPLATE_TYPE(vector) *v)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	if (TEMPLATE_FUNCTION(vector, size)(v) > 0)
		memset(v->stor_begin, 0, sizeof(T)*(size_t)TEMPLATE_FUNCTION(vector, size)(v));
}

/*
 * vector_T_fill - fill the vector with a constant value
 */
void TEMPLATE_FUNCTION(vector, fill)(TEMPLATE_TYPE(vector) *v, T e)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	for (T* ptr = v->stor_begin; ptr < v->stor_end; ptr++)
		*ptr = e;
}

/*
 * vector_T_append
 */
int TEMPLATE_FUNCTION(vector, append)(TEMPLATE_TYPE(vector) *from, TEMPLATE_TYPE(vector) *to)
{
	int fromsize = TEMPLATE_FUNCTION(vector, size)(from);
	int tosize = TEMPLATE_FUNCTION(vector, size)(to);

	TEMPLATE_FUNCTION(vector, resize)(to, tosize + fromsize);
	memcpy(to->stor_begin + tosize, from->stor_begin, sizeof(T) * (size_t)fromsize);
	to->end = to->stor_begin+tosize+fromsize;
	return 0;
}
