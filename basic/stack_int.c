#include "stack_int.h"
#include <stdlib.h>
#include <assert.h>

#define STACK_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "stack.tmp.h"
//#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T


#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "stack.tmp.h"
//#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

/*
 * stack_int_fprint
 */
int stack_int_fprint(stack_int *s, FILE *file)
{
	int n = stack_int_size(s);
	if (0 == n) {
		fprintf(file, "the stack is empty\n");
		return 0;
	}
	for (int i = 0; i < n; i++)
		fprintf(file, " %d", s->stor_begin[i]);
	fprintf(file, "\n");
	return 0;
}

#undef STACK_NUMBER
