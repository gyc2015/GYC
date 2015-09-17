#include "matrix_int.h"
#include <stdlib.h>
#include <assert.h>

#define MATRIX_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "matrix.tmp.h"
//#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T


#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "matrix.tmp.h"
//#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

void print_matrix_int(matrix_int *v, FILE *f)
{
	for (int i = 0; i < v->nrow; i++) {
		fprintf(f, "|");
		for (int j = 0; j < v->ncol; j++) {
			(0 == j) ? 0 : fprintf(f, "\t");
			fprintf(f, "%d", MATRIX(*v,i,j));
		}
		fprintf(f, "|\n");
	}
}


void print_matrix_double(matrix_double *v, FILE *f)
{
	for (int i = 0; i < v->nrow; i++) {
		fprintf(f, "|");
		for (int j = 0; j < v->ncol; j++) {
			(0 == j) ? 0 : fprintf(f, "\t");
			fprintf(f, "%f", MATRIX(*v,i,j));
		}
		fprintf(f, "|\n");
	}
}
#undef MATRIX_NUMBER
