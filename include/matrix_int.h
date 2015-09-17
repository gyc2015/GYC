#ifndef MATRIX_INT_H
#define MATRIX_INT_H

#include <stdio.h>
#include "vector_int.h"
#include "mm.h"

#define MATRIX_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "matrix_tmp.h"
//#include "matrix_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "matrix_tmp.h"
//#include "matrix_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

void print_matrix_int(matrix_int *v, FILE *f);
void print_matrix_double(matrix_double *v, FILE *f);

#undef MATRIX_NUMBER

#endif
