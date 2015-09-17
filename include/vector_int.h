#ifndef VECTOR_INT_H
#define VECTOR_INT_H

#include <stdio.h>
#include "mm.h"

#define VECTOR_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "vector_tmp.h"
#include "vector_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "vector_tmp.h"
#include "vector_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

/*
 * vector_int
 */
void print_vector_int(const vector_int *v, FILE *f);
void print_vector_int3(const vector_int *v, char c, FILE *f);
int vector_int_scan_tie(vector_int *res, vector_int *el, vector_int *il, int max);
int vector_int_order_inc(const vector_int *v, vector_int *ret, int max);
int vector_int_order_dec(const vector_int *v, vector_int *ret, int max);
int vector_int_order_inc2(vector_int *v1, vector_int *v2, vector_int *ret, int max);
int vector_int_ele_freq_min_max(vector_int *dis, const vector_int *v, int *min, int *max);
int vector_int_rank(vector_int *v);

/*
 * vector_double
 */
void print_vector_double(const vector_double *v, FILE *f);
void print_vector_double3(const vector_double *v, char c, FILE *f);
void vector_double_scale(vector_double *v, double term);
void vector_scale_int(vector_double *re,const vector_int *v, double term);

#undef VECTOR_NUMBER

#define T char 
#define TEMPLATE_TYPE(a) a ##_char
#define TEMPLATE_FUNCTION(a,b) a ##_char_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T



#endif
