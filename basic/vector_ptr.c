#include "vector_ptr.h"
#include "maxheap.h"
#include <assert.h>

/*
 * vector_ptr_init
 */
int vector_ptr_init(vector_ptr* v, int size)
{
	int alloc_size = size > 0 ? size :1;

	v->stor_begin = Calloc(alloc_size, void*);
	assert(0 != v->stor_begin);

	v->stor_end = v->stor_begin + alloc_size;
	v->end = v->stor_begin + size;
	v->item_destructor = 0;

	return 0;
}

/*
 * vector_ptr_alloc
 */
int vector_ptr_alloc(vector_ptr* v, int size)
{
	int alloc_size = size > 0 ? size :1;

	v->stor_begin = Calloc(alloc_size, void*);
	assert(0 != v->stor_begin);

	v->stor_end = v->stor_begin + alloc_size;
	v->end = v->stor_begin;
	v->item_destructor = 0;

	return 0;
}

void vector_ptr_call_item_destructor_all(vector_ptr* v)
{
	void **ptr;
	if (v->item_destructor != 0) {
		for (ptr=v->stor_begin; ptr<v->end; ptr++) {
		if (*ptr != 0)
			v->item_destructor(*ptr);
		}
	}
}

/*
 * vector_ptr_destroy
 */
void vector_ptr_destroy(vector_ptr* v)
{
	assert(v != 0);
	if (v->stor_begin != 0) {
		Free(v->stor_begin);
		v->stor_begin = NULL;
	}
}

/*
 * vector_ptr_push_back
 */
int vector_ptr_push_back(vector_ptr* v, void* e)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);	

	/* full, allocate more storage */
	if (v->stor_end == v->end) {
		int new_size = vector_ptr_size(v) * 2;
		if (new_size == 0) { new_size = 1; }
		vector_ptr_reserve(v, new_size);
	}
	
	*(v->end) = e;
	v->end += 1;
	
	return 0;

}

/*
 * vector_ptr_remove_at
 */
void vector_ptr_remove_at(vector_ptr *v, int pos)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	assert(pos < vector_ptr_size(v));
	if (pos + 1 < vector_ptr_size(v))
		memmove(v->stor_begin+pos, v->stor_begin+pos+1,sizeof(void*) * (size_t) (vector_ptr_size(v)-pos-1));
	v->end--;
}

void vector_ptr_remove(vector_ptr *v, void *ele)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	assert(ele != NULL);
	for (int i = 0; i < vector_ptr_size(v); i++) {
		void *tmp = (v->stor_begin)[i];
		if (tmp == ele)
			return vector_ptr_remove_at(v, i);
	}
}

/*
 * vector_ptr_size
 */
int vector_ptr_size(const vector_ptr* v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	return v->end - v->stor_begin;
}

int vector_ptr_capacity(const vector_ptr* v)
{
	return v->stor_end - v->stor_begin;
}
/*
 * vector_ptr_reserve
 */
int vector_ptr_reserve(vector_ptr* v, int size)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	
	int actual_size = vector_ptr_size(v);
	if (size <= actual_size) { return 0; }

	void** tmp = Realloc(v->stor_begin, (size_t) size, void*);
	assert(0 != tmp);

	v->stor_begin=tmp;
	v->stor_end=v->stor_begin + size;
	v->end=v->stor_begin+actual_size;
	
	return 0;

}

/*
 * vector_ptr_resize
 */
int vector_ptr_resize(vector_ptr* v, int newsize)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);
	vector_ptr_reserve(v, newsize);
	v->end = v->stor_begin + newsize;
	return 0;
}

/*
 * vector_ptr_sort
 */
void _swap(void **a, int i, int j)
{
	void *tmp = a[i];
	a[i] = a[j];
	a[j] = tmp;
}

void vector_ptr_sort(vector_ptr *v, int(*compar)(void** a, int i, int j))
{
	heap_sort(v->stor_begin, vector_ptr_size(v), compar,_swap);
}
/*
 * vector_ptr_build_max_heap
 */
void vector_ptr_build_max_heap(vector_ptr *v, int bias, int length, int(*compar)(void** a, int i, int j))
{
	assert(0 != v);
	assert(bias >= 0);
	assert(bias + length <= vector_ptr_size(v));

	build_max_heap(v->stor_begin + bias, length, compar, _swap);
}
