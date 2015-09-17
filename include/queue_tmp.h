
#ifndef T
#error "Specify the vector type"
#endif

#ifndef TEMPLATE_TYPE
#error "Specify the vector template type"
#endif

#ifndef TEMPLATE_FUNCTION
#error "Specify the vector template function"
#endif

typedef struct TEMPLATE_TYPE(queue)
{
	T *begin;
	T *end;
	T *stor_begin;
	T *stor_end;
} TEMPLATE_TYPE(queue);

/*
 * allocation
 */
int TEMPLATE_FUNCTION(queue,init)(TEMPLATE_TYPE(queue) *q, int size);
void TEMPLATE_FUNCTION(queue,destroy)(TEMPLATE_TYPE(queue) *q);


/*
 * accessing element
 */
#ifndef QUEUE 
#define QUEUE(a) ((a).begin)
#endif

/*
 * properties
 */
int TEMPLATE_FUNCTION(queue,is_empty)(const TEMPLATE_TYPE(queue) *q);
int TEMPLATE_FUNCTION(queue,is_full)(const TEMPLATE_TYPE(queue) *q);
int TEMPLATE_FUNCTION(queue,size)(const TEMPLATE_TYPE(queue) *q);
T TEMPLATE_FUNCTION(queue,head)(const TEMPLATE_TYPE(queue) *q);
T TEMPLATE_FUNCTION(queue,tail)(const TEMPLATE_TYPE(queue) *q);

/*
 * operations
 */
void TEMPLATE_FUNCTION(queue,clear)(TEMPLATE_TYPE(queue) *q);
T TEMPLATE_FUNCTION(queue,deq)(TEMPLATE_TYPE(queue) *q);
T TEMPLATE_FUNCTION(queue,pop)(TEMPLATE_TYPE(queue) *q);
int TEMPLATE_FUNCTION(queue,enq)(TEMPLATE_TYPE(queue) *q, T elem);

