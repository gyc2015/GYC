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
 * stack_T_init
 */
int TEMPLATE_FUNCTION(stack,init)(TEMPLATE_TYPE(stack)* s, int size)
{
	assert(0 != s);
	assert(size > 0);

	s->stor_begin=Calloc(size, T);
	assert(0 != s->stor_begin);
	s->stor_end=s->stor_begin + size;
	s->end=s->stor_begin;
	
	return 0;
}

/*
 * stack_T_destroy
 */
void TEMPLATE_FUNCTION(stack,destroy)(TEMPLATE_TYPE(stack)* s)
{
	assert(0 != s);
	assert(0 != s->stor_begin);
	Free(s->stor_begin);
	s->stor_end = 0;
	s->end = 0;
}

/*
 * stack_T_reserve
 */
int TEMPLATE_FUNCTION(stack,reserve)(TEMPLATE_TYPE(stack) *s, int size)
{
	assert(0 != s);
	assert(0 != s->stor_begin);

	int actual_size = TEMPLATE_FUNCTION(stack,size)(s);
	if (size <= actual_size)
		return 0;
	T *tmp = Realloc(s->stor_begin, (size_t) size, T);
	assert(0 != tmp);

	s->stor_begin = tmp; 
	s->stor_end = s->stor_begin + size;
	s->end = s->stor_begin+actual_size;
	return 0;
}

/*
 * stack_T_empty
 */
int TEMPLATE_FUNCTION(stack,is_empty)(TEMPLATE_TYPE(stack)* s)
{
	assert (s != NULL);
	assert (s->stor_begin != NULL);
	assert (s->end != NULL);
	return s->stor_begin == s->end;
}

/*
 * stack_T_size
 */
int TEMPLATE_FUNCTION(stack,size)(const TEMPLATE_TYPE(stack) *s)
{
	assert (s != NULL);
	assert (s->stor_begin != NULL);
	return s->end - s->stor_begin;
}

/*
 * stack_T_clear
 */
void TEMPLATE_FUNCTION(stack,clear)(TEMPLATE_TYPE(stack)* s)
{
	assert (s != NULL);
	assert (s->stor_begin != NULL);
	s->end = s->stor_begin;
}

/*
 * stack_T_push
 */
int TEMPLATE_FUNCTION(stack,push)(TEMPLATE_TYPE(stack) *s, T elem)
{
	assert (s != NULL);
	assert (s->stor_begin != NULL);

	if (s->end == s->stor_end) {
		T *old = s->stor_begin;	
		T *bigger = Calloc(2 * TEMPLATE_FUNCTION(stack,size)(s), T);
		
		assert(0 != bigger);
		memcpy(bigger, s->stor_begin, (size_t) TEMPLATE_FUNCTION(stack,size)(s)*sizeof(T));

		s->end = bigger + (s->stor_end - s->stor_begin);
		s->stor_end = bigger + 2 * (s->stor_end - s->stor_begin);
		s->stor_begin = bigger;
		
		*(s->end) = elem;
		(s->end) += 1;

		Free(old);
	} else {
		*(s->end) = elem;
		(s->end) += 1;
	}
	return 0;
}


int TEMPLATE_FUNCTION(stack,append)(TEMPLATE_TYPE(stack) *s, const T *es, int n)
{
	assert (s != NULL);
	assert (s->stor_begin != NULL);
	
	for (int i = 0; i < n; i++)
		TEMPLATE_FUNCTION(stack,push)(s, es[i]);
	return 0;
}

/*
 * stack_T_pop
 */
T TEMPLATE_FUNCTION(stack,pop)(TEMPLATE_TYPE(stack)* s)
{
	assert (s != NULL);
	assert (s->stor_begin != NULL);
	assert (s->end != NULL);
	assert (s->end != s->stor_begin);
		
	(s->end)--;
	
	return *(s->end);
}

/*
 * stack_T_top
 */
T TEMPLATE_FUNCTION(stack,top)(const TEMPLATE_TYPE(stack)* s)
{
	assert (s != NULL);
	assert (s->stor_begin != NULL);
	assert (s->end != NULL);
	assert (s->end != s->stor_begin);

	return *(s->end-1);
}


