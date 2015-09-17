#ifndef STACK_INT_H
#define STACK_INT_H

#include <stdio.h>
#include "mm.h"

#define STACK_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "stack_tmp.h"
//#include "vector_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "stack_tmp.h"
//#include "vector_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int stack_int_fprint(stack_int *s, FILE *file);

#undef STACK_NUMBER

#endif
