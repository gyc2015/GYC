#include "queue_int.h"
#include <assert.h>

int main()
{
	queue_int a;
	queue_int_init(&a, 1);
	assert(queue_int_is_empty(&a));

	queue_int_fprint(&a, stdout);

	for (int i = 1; i < 15; i++) {
		queue_int_enq(&a, i);
		assert(i == queue_int_size(&a));
	}
	queue_int_fprint(&a, stdout);

	for (int i = 1; i < 3; i++)
		printf("%d\n",queue_int_deq(&a));
	queue_int_fprint(&a, stdout);

	for (int i = 1; i < 15; i++) {
		queue_int_enq(&a, i);
	}
	queue_int_fprint(&a, stdout);

	queue_int_destroy(&a);
	return 0;
}
