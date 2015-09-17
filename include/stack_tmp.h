#ifndef T
#error "Specify the vector type"
#endif

#ifndef TEMPLATE_TYPE
#error "Specify the vector template type"
#endif

#ifndef TEMPLATE_FUNCTION
#error "Specify the vector template function"
#endif

typedef struct TEMPLATE_TYPE(stack)
{
	T *stor_begin;
	T *stor_end;
	T *end;
} TEMPLATE_TYPE(stack);

/*
 * allocation
 */
int TEMPLATE_FUNCTION(stack,init)(TEMPLATE_TYPE(stack) *s, int size);
void TEMPLATE_FUNCTION(stack,destroy)(TEMPLATE_TYPE(stack) *s);
int TEMPLATE_FUNCTION(stack,reserve)(TEMPLATE_TYPE(stack) *s, int size);

/*
 * properties
 */
int TEMPLATE_FUNCTION(stack,is_empty)(TEMPLATE_TYPE(stack) *s);
int TEMPLATE_FUNCTION(stack,size)(const TEMPLATE_TYPE(stack) *s);
T TEMPLATE_FUNCTION(stack,top)(const TEMPLATE_TYPE(stack)* s);

/*
 * operations
 */
void TEMPLATE_FUNCTION(stack,clear)(TEMPLATE_TYPE(stack)* s);
int TEMPLATE_FUNCTION(stack,push)(TEMPLATE_TYPE(stack) *s, T elem);
int TEMPLATE_FUNCTION(stack,append)(TEMPLATE_TYPE(stack) *s, const T *es, int n);
T TEMPLATE_FUNCTION(stack,pop)(TEMPLATE_TYPE(stack)* s);
