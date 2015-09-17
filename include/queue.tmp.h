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
 * queue_T_init
 */
int TEMPLATE_FUNCTION(queue,init)(TEMPLATE_TYPE(queue) *q, int size)
{
	assert(0 != q);
	assert(size > 0);

	q->stor_begin = Calloc(size, T);
	assert(0 != q->stor_begin);

	q->stor_end = q->stor_begin + size;
	q->begin = q->stor_begin;
	q->end = NULL;
	
	return 0;
}

/*
 * queue_T_destroy
 */
void TEMPLATE_FUNCTION(queue,destroy)(TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->stor_begin);

	Free(q->stor_begin);
	q->stor_begin = 0;
	q->stor_end = 0;
	q->begin = 0;
	q->end = 0;
}

/*
 * queue_T_is_empty
 */
int TEMPLATE_FUNCTION(queue,is_empty)(const TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->stor_begin);
	return NULL == q->end;
}

/*
 * queue_T_is_full
 */
int TEMPLATE_FUNCTION(queue,is_full)(const TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->stor_begin);
	return q->begin == q->end;
}

/*
 * queue_T_size
 */
int TEMPLATE_FUNCTION(queue,size)(const TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->stor_begin);

	if (NULL == q->end)
		return 0;
	else if (q->begin < q->end)
		return q->end - q->begin;
	else
		return (q->stor_end - q->begin) + (q->end - q->stor_begin);
}

/*
 * queue_T_head
 */
T TEMPLATE_FUNCTION(queue,head)(const TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->stor_begin);
	return *(q->begin);
}

/*
 * queue_T_tail
 */
T TEMPLATE_FUNCTION(queue,tail)(const TEMPLATE_TYPE(queue) *q)
{
	assert(q != 0);
	assert(q->stor_begin != 0);
	if (q->end == q->stor_begin)
		return *(q->stor_end-1);
	return *(q->end-1);
}

/*
 * queue_T_clear
 */
void TEMPLATE_FUNCTION(queue, clear)(TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->stor_begin);
	q->begin=q->stor_begin;
	q->end=NULL;
}

/*
 * queue_T_enq - enqueue append an element to the queue
 */
int TEMPLATE_FUNCTION(queue,enq)(TEMPLATE_TYPE(queue) *q, T elem)
{
	assert(q != 0);
	assert(q->stor_begin != 0);

	/* not full */
	if (q->begin != q->end) {
		if (q->end == NULL)
			q->end = q->begin;
		*(q->end) = elem;
		(q->end)++;
		if (q->end == q->stor_end)
			q->end = q->stor_begin;
	} else {
		/* full, allocate more storage */
		T *old = q->stor_begin;
		T *bigger = Calloc(2 * (q->stor_end - q->stor_begin), T);
		assert(0 != bigger);

		if (q->begin != q->stor_end)
			memcpy(bigger, q->begin,  (size_t)(q->stor_end - q->begin) * sizeof(T));

		if (q->end != q->stor_begin)
			memcpy(bigger+(q->stor_end-q->begin), q->stor_begin, (size_t)(q->end - q->stor_begin) * sizeof(T));

		q->end = bigger + (q->stor_end - q->stor_begin);
		q->stor_end  = bigger + 2*(q->stor_end - q->stor_begin);
		q->stor_begin = bigger;
		q->begin = bigger;

		*(q->end) = elem;
		(q->end)++;
		if (q->end==q->stor_end)
			q->end=q->stor_begin;

		Free(old);
	}
	return 0;
}

/*
 * queue_T_deq - dequeue return and remove the head of the queue
 */
T TEMPLATE_FUNCTION(queue,deq)(TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->begin);

	T re = *(q->begin);
	q->begin++;
	
	if (q->begin == q->stor_end)
		q->begin = q->stor_begin;
	if (q->begin == q->end)
		q->end = NULL;

	return re;
}

/*
 * queue_T_pop - pop return and remove the tail of the queue
 */
T TEMPLATE_FUNCTION(queue,pop)(TEMPLATE_TYPE(queue) *q)
{
	assert(0 != q);
	assert(0 != q->begin);

	T re;
	if (q->end == q->stor_begin) {
		re = *((q->stor_end) - 1);
		q->end = (q->stor_end) - 1;
	} else {
		re = *((q->end) - 1);
		q->end = (q->end) - 1;
	}

	if (q->begin == q->end)
		q->end=NULL;
	return re;
}


