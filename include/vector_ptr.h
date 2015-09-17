#ifndef VECTOR_PTR_H
#define VECTOR_PTR_H

#include <stdio.h>
#include "mm.h"

/** 
 * Vector, storing pointers efficiently
 * \ingroup internal
 * 
 */
typedef struct vector_ptr
{
	void** stor_begin;
	void** stor_end;
	void** end;
    void (*item_destructor)(void *a);
} vector_ptr;

/*
 * allocation
 */
int vector_ptr_init(vector_ptr* v, int size);
int vector_ptr_alloc(vector_ptr* v, int size);
void vector_ptr_destroy(vector_ptr* v);
/*
int vector_ptr_init_copy (vector_ptr* v, void** data, long int length);
const vector_ptr *vector_ptr_view (const vector_ptr *v, 
				     void *const *data, long int length);
void vector_ptr_free_all   (vector_ptr* v);
void vector_ptr_destroy_all   (vector_ptr* v);
igraph_bool_t vector_ptr_empty     (const vector_ptr* v);
*/

/*
 * properties
 */
int vector_ptr_size(const vector_ptr* v);
int vector_ptr_capacity(const vector_ptr* v);
/*
void vector_ptr_clear     (vector_ptr* v);
void vector_ptr_null      (vector_ptr* v);
*/

/*
 * Exchange Element
 */
int vector_ptr_push_back(vector_ptr* v, void* e);
void vector_ptr_remove(vector_ptr *v, void *ele);
void vector_ptr_remove_at(vector_ptr *v, int pos);

/*
 * resize operation
 */
int vector_ptr_reserve(vector_ptr* v, int size);
int vector_ptr_resize(vector_ptr* v, int newsize);

/*
int vector_ptr_append(vector_ptr *to, const vector_ptr *from);
void *vector_ptr_pop_back (vector_ptr *v);
int vector_ptr_insert(vector_ptr *v, long int pos, void* e);
void* vector_ptr_e(const vector_ptr* v, long int pos);
void vector_ptr_set(vector_ptr* v, long int pos, void* value);
void vector_ptr_copy_to(const vector_ptr *v, void** to);
int vector_ptr_copy(vector_ptr *to, const vector_ptr *from);
*/

/*
 * sort
 */
void vector_ptr_sort(vector_ptr *v, int(*compar)(void**, int i, int j));
void vector_ptr_build_max_heap(vector_ptr *v, int bias, int length, int(*compar)(void** a, int i, int j));
/*
int vector_ptr_index_int(vector_ptr *v,
				const igraph_vector_int_t *idx);

igraph_finally_func_t* vector_ptr_get_item_destructor(const vector_ptr *v);
igraph_finally_func_t* vector_ptr_set_item_destructor(vector_ptr *v,
        igraph_finally_func_t *func);
*/


#endif
