#include <assert.h>
#include <stdarg.h>

/*
 * vector_T_init_value
 *
 * e.g. vector_int_init_value(&b, 3, 1,2,3);
 */
int TEMPLATE_FUNCTION(vector,init_value)(TEMPLATE_TYPE(vector) *v, int no, ...)
{
	va_list ap;
	TEMPLATE_FUNCTION(vector,init)(v, no);

	va_start(ap, no);
	for (int i = 0; i < no; i++)
		VECTOR(*v)[i] = (T)va_arg(ap, T);
	va_end(ap);

	return 0;
}

/*
 * vector_T_init_value_end - this constructor is similar to vector_T_init_value(), the
 * only difference is that instead of giving the number of elements in the vector, a special
 * marker element follows the last vector element.
 */
int TEMPLATE_FUNCTION(vector, init_value_end)(TEMPLATE_TYPE(vector) *v, T endmark, ...)
{
	int i = 0, n = 0;
	va_list ap;

	va_start(ap, endmark);
	while(1) {
		T num = (T) va_arg(ap, T);
		if (num == endmark)
			break;
		n++;
	}
	va_end(ap);

	TEMPLATE_FUNCTION(vector, init)(v, n);

	va_start(ap, endmark);
	for (i = 0; i < n; i++)
		VECTOR(*v)[i] = (T)va_arg(ap, T);
	va_end(ap);

	return 0;
}

/*
 * vector_T_max - find the maximum of the vector
 */
T TEMPLATE_FUNCTION(vector, max)(const TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	int n = TEMPLATE_FUNCTION(vector, size)(v);
	T max = VECTOR(*v)[0];
	for (int i = 1; i < n; i++) {
		if (VECTOR(*v)[i] > max)
			max = VECTOR(*v)[i];
	}
	return max;
}

/*
 * vector_T_which_max - gives the position of the maximum element of the vector
 */
int TEMPLATE_FUNCTION(vector, which_max)(const TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	int n = TEMPLATE_FUNCTION(vector, size)(v);
	int which = 0;
	for (int i = 1; i < n; i++) {
		T tmp = VECTOR(*v)[i];
		if (tmp > VECTOR(*v)[which])
			which = i;
	}
	return which;
}

/*
 * vector_T_min - find the minimum of the vector
 */
T TEMPLATE_FUNCTION(vector, min)(const TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	int n = TEMPLATE_FUNCTION(vector, size)(v);
	T min = VECTOR(*v)[0];
	for (int i = 1; i < n; i++) {
		if (VECTOR(*v)[i] < min)
			min = VECTOR(*v)[i];
	}
	return min;
}

/*
 * vector_T_which_min - gives the position of the minimum element of the vector
 */
int TEMPLATE_FUNCTION(vector, which_min)(const TEMPLATE_TYPE(vector) *v)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	int n = TEMPLATE_FUNCTION(vector, size)(v);
	int which = 0;
	for (int i = 1; i < n; i++) {
		T tmp = VECTOR(*v)[i];
		if (tmp < VECTOR(*v)[which])
			which = i;
	}
	return which;
}

/*
 * vector_T_minmax
 */
int TEMPLATE_FUNCTION(vector, minmax)(const TEMPLATE_TYPE(vector) *v, T *min, T *max)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	int n = TEMPLATE_FUNCTION(vector, size)(v);
	*min = *max = VECTOR(*v)[0];
	for (int i = 1; i < n; i++) {
		T tmp = VECTOR(*v)[i];
		if (tmp > *max)
			*max = tmp;
		else if (tmp < *min)
			*min = tmp;
	}
	return 0;
}
/*
 * vector_T_which_minmax
 */
int TEMPLATE_FUNCTION(vector, which_minmax)(const TEMPLATE_TYPE(vector) *v, int *which_min, int *which_max)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	int n = TEMPLATE_FUNCTION(vector, size)(v);
	*which_min = *which_max = 0;
	for (int i = 1; i < n; i++) {
		T tmp = VECTOR(*v)[i];
		if (tmp > VECTOR(*v)[*which_max])
			*which_max = i;
		else if (tmp < VECTOR(*v)[*which_min])
			*which_min = i;
	}
	return 0;
}

/*
 * vector_T_contains (0:not contain, 1:contains)
 */
int TEMPLATE_FUNCTION(vector, contains)(const TEMPLATE_TYPE(vector) *v, T e)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	T *p = v->stor_begin;
	while (p < v->end) {
		if (*p == e)
			return 1;
		p++;
	}
	return 0;
}

/*
 * vector_T_whereis - get the e's index
 */
int TEMPLATE_FUNCTION(vector, whereis)(const TEMPLATE_TYPE(vector) *v, T e)
{
	assert(v != NULL);
	assert(v->stor_begin != NULL);

	T *p = v->stor_begin;
	while (p < v->end) {
		if (*p == e)
			return (p - v->stor_begin);
		p++;
	}

	return -1;
}

/*
 * vector_T_isininterval - check if all the elements of a vector are in the given interval.
 *
 * @v: the vector object
 * @low: the lower limit of the interval (inclusive)
 * @high: the higher limit of the interval (inclusive)
 */
int TEMPLATE_FUNCTION(vector, isininterval)(const TEMPLATE_TYPE(vector) *v, T low, T high)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	for(T* ptr = v->stor_begin; ptr < v->end; ptr++) {
		if (*ptr < low || *ptr > high)
			return 0;
	}
	return 1;
}

/*
 * vector_T_add_constant
 */
int TEMPLATE_FUNCTION(vector,add_constant)(TEMPLATE_TYPE(vector) *v, T plus)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	for (int i = 0; i < TEMPLATE_FUNCTION(vector,size)(v); i++)
		VECTOR(*v)[i] += plus;
	return 0;
}

/*
 * vector_T_add
 */
int TEMPLATE_FUNCTION(vector,add)(TEMPLATE_TYPE(vector) *v1, TEMPLATE_TYPE(vector) *v2)
{
	assert(NULL != v1);
	assert(NULL != v1->stor_begin);
	assert(NULL != v2);
	assert(NULL != v2->stor_begin);

	int n1 = TEMPLATE_FUNCTION(vector, size)(v1);
	int n2 = TEMPLATE_FUNCTION(vector, size)(v2);
	assert(n1 == n2);

	for (int i = 0; i < n1; i++)
		VECTOR(*v1)[i] += VECTOR(*v2)[i];
	return 0;
}

/*
 * vector_T_sub
 */
int TEMPLATE_FUNCTION(vector,sub)(TEMPLATE_TYPE(vector) *v1, TEMPLATE_TYPE(vector) *v2)
{
	assert(NULL != v1);
	assert(NULL != v1->stor_begin);
	assert(NULL != v2);
	assert(NULL != v2->stor_begin);

	int n1 = TEMPLATE_FUNCTION(vector, size)(v1);
	int n2 = TEMPLATE_FUNCTION(vector, size)(v2);
	assert(n1 == n2);

	for (int i = 0; i < n1; i++)
		VECTOR(*v1)[i] -= VECTOR(*v2)[i];
	return 0;
}

/*
 * vector_T_sum
 */
T TEMPLATE_FUNCTION(vector,sum)(TEMPLATE_TYPE(vector) *v)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	T res = 0;
	for (int i =0; i < TEMPLATE_FUNCTION(vector,size)(v); i++)
		res += VECTOR(*v)[i];
	return res;
}

/*
 * vector_T_cumsum
 *
 * @to: An initialized vector, the result will be stored here.
 */
int TEMPLATE_FUNCTION(vector,cumsum)(TEMPLATE_TYPE(vector) *to, TEMPLATE_TYPE(vector) *from)
{
	assert(NULL != to);
	assert(NULL != to->stor_begin);
	assert(NULL != from);
	assert(NULL != from->stor_begin);
	int n = TEMPLATE_FUNCTION(vector, size)(from);

	TEMPLATE_FUNCTION(vector,resize)(to,n);
	T res = 0;
	for (int i = 0; i < n; i++) {
		res += VECTOR(*from)[i];
		VECTOR(*to)[i] = res;
	}
	return 0;
}
