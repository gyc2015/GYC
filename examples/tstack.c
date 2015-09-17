#include "stack_int.h"
#include <assert.h>

int main()
{
	stack_int a;
	stack_int_init(&a, 1);
	assert(stack_int_is_empty(&a));
	printf("stack length = %d\n", stack_int_size(&a));

	stack_int_push(&a, 1);
	assert(1 == stack_int_size(&a));
	printf("stack length = %d\n", stack_int_size(&a));

	stack_int_push(&a, 3);
	assert(2 == stack_int_size(&a));
	printf("stack length = %d\n", stack_int_size(&a));
	stack_int_fprint(&a, stdout);

	printf("top %d\n", stack_int_top(&a));
	assert(2 == stack_int_size(&a));
	printf("pop %d\n", stack_int_pop(&a));
	assert(1 == stack_int_size(&a));
	printf("stack length = %d\n", stack_int_size(&a));

	stack_int_destroy(&a);
	return 0;
}
