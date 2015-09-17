#include "queue_int.h"
#include <stdlib.h>
#include <assert.h>

#define QUEUE_NUMBER

#define T int
#define TEMPLATE_TYPE(a) a ##_int
#define TEMPLATE_FUNCTION(a,b) a ##_int_## b
#include "queue.tmp.h"
//#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T


#define T double 
#define TEMPLATE_TYPE(a) a ##_double
#define TEMPLATE_FUNCTION(a,b) a ##_double_## b
#include "queue.tmp.h"
//#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

/*
 * queue_int_fprint
 */
int queue_int_fprint(queue_int *q, FILE *file)
{
	assert(0 != q);
	assert(0 != q->stor_begin);

	if (q->end == NULL) {
		fprintf(file, "The queue is empty\n");
		return 0;
	}
	if (q->begin < q->end) {
		for (int *p = q->begin; p < q->end; p++)
			fprintf(file, " %d", *p);
	} else {
		for (int *p = q->begin; p < q->stor_end; p++)
			fprintf(file, " %d", *p);
		for (int *p = q->stor_begin; p < q->end; p++)
			fprintf(file, " %d", *p);
	}

	fprintf(file, "\n");
	return 0;
}

#undef QUEUE_NUMBER
