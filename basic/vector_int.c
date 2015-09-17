#include "vector_int.h"
#include <stdlib.h>
#include <assert.h>

#define VECTOR_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "vector.tmp.h"
#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T


#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "vector.tmp.h"
#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#undef VECTOR_NUMBER

#define T char 
#define TEMPLATE_TYPE(a) a ##_char
#define TEMPLATE_FUNCTION(a,b) a ##_char_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T


void print_vector_int(const vector_int *v, FILE *f) {
  int i;
  for (i = 0; i < vector_int_size(v); i++) {
    fprintf(f, " %d", VECTOR(*v)[i]);
  }
  fprintf(f, "\n");
}

void print_vector_int3(const vector_int *v, char c, FILE *f)
{
	int i;
	fprintf(f,"%d", VECTOR(*v)[0]);
	for (i = 1; i < vector_int_size(v); i++) {
		fprintf(f, "%c%d",c, VECTOR(*v)[i]);
	}
	fprintf(f, "\n");

}
void print_vector_double(const vector_double *v, FILE *f) {
	int i;
	for (i = 0; i < vector_double_size(v); i++) {
		fprintf(f, " %f", VECTOR(*v)[i]);
	}
	fprintf(f, "\n");
}

void print_vector_double3(const vector_double *v, char c, FILE *f)
{
	int i;
	fprintf(f,"%f", VECTOR(*v)[0]);
	for (i = 1; i < vector_double_size(v); i++) {
		fprintf(f, "%c%f",c, VECTOR(*v)[i]);
	}
	fprintf(f, "\n");

}
/*
 * vector_int_scan_tie - scan the vector @el 
 * according to its orderlist @il to identify the start of each tie
 *
 * @res: the vector contains tie start point
 * @el: the object list
 * @il: the order list of @el
 * @max: hint, the largest element in el
 */
int vector_int_scan_tie(vector_int *res, vector_int *el, vector_int *il, int max)
{
	int i,j;
	int nel = vector_int_size(el);
	vector_int_resize(res, max + 1);
	vector_int_fill(res, nel);
	#define ELIL(i) (VECTOR(*el)[ VECTOR(*il)[(i)] ])
	if (vector_int_size(el) == 0) {
		vector_int_null(res);
	} else {

		int idx = -1;
		for (i = 0; i <= ELIL(0) ; i++) {
			idx++;
			VECTOR(*res)[idx] = 0;
		}

		for (i = 0; i < nel; i++) {
			int n = ELIL(i) - ELIL(VECTOR(*res)[idx]);
			for (j = 0; j < n; j++) {
				idx++;
				VECTOR(*res)[idx] = i;
			}
		}
	}
	#undef ELIL
	return 0;
}


int vector_int_order_dec(const vector_int *v, vector_int *ret, int max)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	int i, j;
	vector_int ptr, rad;
	int n = vector_int_size(v);
	vector_int_init(&ptr, max + 1);
	vector_int_init(&rad, n);
	vector_int_resize(ret, n);

	for (i = 0; i < n; i++) {
		int radix = max - v->stor_begin[i];
		if (VECTOR(ptr)[radix] != 0)
			VECTOR(rad)[i] = VECTOR(ptr)[radix];
		VECTOR(ptr)[radix] = i+1;
	}

	j = 0;
	for (i = 0; i < max + 1; i++) {
		if (VECTOR(ptr)[i] != 0) {
			int next = VECTOR(ptr)[i] - 1;
			ret->stor_begin[j++] = next;
			while (VECTOR(rad)[next] != 0) {
				next = VECTOR(rad)[next] - 1;
				ret->stor_begin[j++] = next;
			}
		}
	}

	vector_int_destroy(&ptr);
	vector_int_destroy(&rad);
	return 0;
}

int vector_int_order_inc(const vector_int *v, vector_int *ret, int max)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	int i, j;
	vector_int ptr, rad;
	int n = vector_int_size(v);
	vector_int_init(&ptr, max + 1);
	vector_int_init(&rad, n);
	vector_int_resize(ret, n);

	for (i = 0; i < n; i++) {
		int radix = v->stor_begin[i];
		if (VECTOR(ptr)[radix] != 0)
			VECTOR(rad)[i] = VECTOR(ptr)[radix];
		VECTOR(ptr)[radix] = i+1;
	}

	j = 0;
	for (i = 0; i < max + 1; i++) {
		if (VECTOR(ptr)[i] != 0) {
			int next = VECTOR(ptr)[i] - 1;
			ret->stor_begin[j++] = next;
			while (VECTOR(rad)[next] != 0) {
				next = VECTOR(rad)[next] - 1;
				ret->stor_begin[j++] = next;
			}
		}
	}

	vector_int_destroy(&ptr);
	vector_int_destroy(&rad);
	return 0;
}
/*
 * vector_int_order_inc2 - calculate the order of the elements in the vector
 *
 * @v1: the original vector object
 * @v2: the second key
 * @ret: An initialized vector_int object, it will be resized to match the size of v1.
 *       The result of the computation will be stored here.
 * @max: hint, the largest element in v1
 */
int vector_int_order_inc2(vector_int *v1, vector_int *v2, vector_int *ret, int max)
{
	assert(NULL != v1);
	assert(NULL != v1->stor_begin);
	assert(NULL != v2);
	assert(NULL != v2->stor_begin);

	int i, j;
	vector_int ptr, rad;
	int n = vector_int_size(v1);
	vector_int_init(&ptr, max + 1);
	vector_int_init(&rad, n);
	vector_int_resize(ret, n);

	for (i = 0; i < n; i++) {
		int radix = v2->stor_begin[i];
		if (VECTOR(ptr)[radix] != 0)
			VECTOR(rad)[i] = VECTOR(ptr)[radix];
		VECTOR(ptr)[radix] = i+1;
	}

	j = 0;
	for (i = 0; i < max + 1; i++) {
		if (VECTOR(ptr)[i] != 0) {
			int next = VECTOR(ptr)[i] - 1;
			ret->stor_begin[j++] = next;
			while (VECTOR(rad)[next] != 0) {
				next = VECTOR(rad)[next] - 1;
				ret->stor_begin[j++] = next;
			}
		}
	}

	vector_int_null(&ptr);
	vector_int_null(&rad);

	for (int i = 0; i < n; i++) {
		int e = VECTOR(*ret)[n-i-1];
		int radix = VECTOR(*v1)[e];
		if (VECTOR(ptr)[radix] != 0)
			VECTOR(rad)[e] = VECTOR(ptr)[radix];
		VECTOR(ptr)[radix] = e + 1;
	}

	j = 0;
	for (int i = 0; i < max + 1; i++) {
		if (VECTOR(ptr)[i] != 0) {
			int next = VECTOR(ptr)[i] - 1;
			ret->stor_begin[j++] = next;
			while (VECTOR(rad)[next] != 0) {
				next = VECTOR(rad)[next] -1;
				VECTOR(*ret)[j++] = next;
			}
		}
	}

	vector_int_destroy(&ptr);
	vector_int_destroy(&rad);
	return 0;
}

/*
 * vector_int_element_distribution
 *
 * @dis: an initialized vector
 * @v: the vector
 */
int vector_int_ele_freq_min_max(vector_int *dis, const vector_int *v, int *min, int *max)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);
	assert(NULL != dis);
	assert(NULL != dis->stor_begin);

	int n = vector_int_size(v);
	vector_int_minmax(v, min, max);
	int ndis = *max - *min + 1;
	vector_int_resize(dis, ndis);
	vector_int_null(dis);

	for (int i = 0; i < n; i++) {
		int tmp = VECTOR(*v)[i] - *min;
		VECTOR(*dis)[tmp]++;
	}
	return 0;
}

/*
 * vector_int_rank
 */
int vector_int_rank(vector_int *v)
{
	int min, max;
	vector_int fre;
	vector_int_init(&fre,0);

	int rank = 0;
	vector_int_ele_freq_min_max(&fre, v, &min, &max);
	for (int i = 0; i < vector_int_size(&fre); i++) {
		if (0 < VECTOR(fre)[i]) {
			VECTOR(fre)[i] = rank;
			rank++;
		}
	}

	for (int i = 0; i < vector_int_size(v); i++)
		VECTOR(*v)[i] = VECTOR(fre)[VECTOR(*v)[i]-min];

	vector_int_destroy(&fre);
	return 0;
}

/*
 * vector_double_scale
 */
void vector_double_scale(vector_double *v, double term)
{
	for (int i = 0; i < vector_double_size(v); i++)
		VECTOR(*v)[i] *= term;
}

/*
 * vector_scale_int
 */
void vector_scale_int(vector_double *re,const vector_int *v, double term)
{
	vector_double_resize(re, vector_int_size(v));
	for (int i = 0; i < vector_int_size(v); i++)
		VECTOR(*re)[i] = term * VECTOR(*v)[i];
}

