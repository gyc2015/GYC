#ifndef QUEUE_INT_H
#define QUEUE_INT_H

#include <stdio.h>
#include "mm.h"

#define QUEUE_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "queue_tmp.h"
//#include "vector_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "queue_tmp.h"
//#include "vector_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int queue_int_fprint(queue_int *q, FILE *file);

#undef QUEUE_NUMBER

#endif
