#include "vector_int.h"
#include <assert.h>
#include <stdio.h>
#include "mm.h"

typedef struct test_t
{
	int a;
	int b;
} test_t;

#define T test_t
#define TEMPLATE_TYPE(a) a ##_test_t
#define TEMPLATE_FUNCTION(a,b) a ##_test_t_## b
#include "vector_tmp.h"
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int test_t_op(const void* a, const void* b)
{
	test_t* atest = (test_t*)a;
	test_t* btest = (test_t*)b;
	
	if (atest->a < btest->a)
		return -1;
	else if (atest->a == btest->a)
		if (atest->b < btest->b)
			return -1;
		else if (atest->b == btest->b)
			return 0;
		else
			return 1;
	else
		return 1;
}

void print_vector_test_t(vector_test_t *v, FILE *f) {
	int i;
	for (i = 0; i < vector_test_t_size(v); i++) {
		fprintf(f, "v[%d].a=%d,v[%d].b=%d\n", i, VECTOR(*v)[i].a, i, VECTOR(*v)[i].b);
	}
	fprintf(f, "\n");
}

int main()
{
	vector_int a;
	vector_int_init(&a,3);
	printf("<<<init &a 3>>>\n");
	print_vector_int(&a, stdout);
	assert(3 == vector_int_size(&a));
	assert(3 == vector_int_capacity(&a));
	assert(0 == vector_int_e(&a, 0));

	vector_int_insert(&a, 2, 10);
	printf("<<<insert &a, 2, 10>>>\n");
	assert(4 == vector_int_capacity(&a));
	assert(4 == vector_int_size(&a));
	assert(10 == vector_int_e(&a, 2));
	assert(10 == *vector_int_e_ptr(&a, 2));
	print_vector_int(&a, stdout);

	vector_int_push_back(&a, 60);
	printf("<<<push back &a, 60>>>\n");
	assert(60 == vector_int_tail(&a));
	assert(8 == vector_int_capacity(&a));
	assert(5 == vector_int_size(&a));
	assert(60 == vector_int_max(&a));
	assert(4 == vector_int_which_max(&a));
	assert(1 == vector_int_contains(&a, 60));
	assert(0 == vector_int_contains(&a, 100));
	print_vector_int(&a, stdout);

	vector_int_resize_min(&a);
	printf("<<<resize min &a>>>\n");
	assert(60 == vector_int_tail(&a));
	assert(5 == vector_int_capacity(&a));
	assert(5 == vector_int_size(&a));
	print_vector_int(&a, stdout);

	printf("<<<pop_back &a>>>\n");
	assert(60 == vector_int_pop_back(&a));
	assert(5 == vector_int_capacity(&a));
	assert(4 == vector_int_size(&a));
	print_vector_int(&a, stdout);

	vector_int_set(&a, 2, -12);
	printf("<<<set &a 2 12>>>\n");
	assert(-12 == VECTOR(a)[2]);
	assert(5 == vector_int_capacity(&a));
	assert(4 == vector_int_size(&a));
	assert(-12 == vector_int_min(&a));
	assert(2 == vector_int_which_min(&a));
	print_vector_int(&a, stdout);


	vector_int_set(&a, 1, 12);
	int min, max;
	int which_min, which_max;
	vector_int_minmax(&a,&min, &max);
	vector_int_which_minmax(&a, &which_min, &which_max);
	assert(-12 == min);
	assert(12 == max);
	assert(2 == which_min);
	assert(1 == which_max);

	vector_int_reserve(&a, 10);
	printf("<<<reserve &a 10>>>\n");
	assert(10 == vector_int_capacity(&a));
	assert(4 == vector_int_size(&a));
	print_vector_int(&a, stdout);

	vector_int_null(&a);
	printf("<<<null &a>>>\n");
	assert(10 == vector_int_capacity(&a));
	assert(4 == vector_int_size(&a));
	assert(0 == VECTOR(a)[2]);
	print_vector_int(&a, stdout);

	vector_int_fill(&a, 15);
	printf("<<<fill &a 15>>>\n");
	assert(10 == vector_int_capacity(&a));
	assert(4 == vector_int_size(&a));
	assert(15 == VECTOR(a)[2]);
	print_vector_int(&a, stdout);

	vector_int_clear(&a);
	printf("<<<clear &a>>>\n");
	assert(10 == vector_int_capacity(&a));
	assert(0 == vector_int_size(&a));
	print_vector_int(&a, stdout);

	vector_int_destroy(&a);
	printf("<<<destroy &a>>>\n");
//	assert(0 == vector_int_capacity(&a));
//	assert(0 == vector_int_size(&a));
//	print_vector_int(&a, stdout);

	int hehe[5] = {1,2,3,4,5};
	vector_int_init_copy(&a, hehe, 5);
	printf("<<<init_copy &a {1,2,3,4,5}>>>\n");
	assert(5 == vector_int_capacity(&a));
	assert(5 == vector_int_size(&a));
	assert(3 == VECTOR(a)[2]);
	print_vector_int(&a, stdout);

	int hehe2[5];
	vector_int_copy_to(&a, hehe2);
	for (int i = 0; i < 5; i++)
		assert(hehe[i] == hehe2[i]);

	vector_int_add_constant(&a, 1);
	printf("<<<add_constant &a 1>>>\n");
	assert(4 == VECTOR(a)[2]);
	print_vector_int(&a, stdout);
	vector_int a2;
	vector_int af;
	vector_int_init(&af,0);
	vector_int_init_value(&a2, 5, 5,4,3,2,1);
	vector_int_add(&a2, &a);
	assert(7 == VECTOR(a2)[2]);
	print_vector_int(&a2, stdout);
	vector_int_ele_freq_min_max(&af, &a2, &min, &max);
	print_vector_int(&af,stdout);
	vector_int_ele_freq_min_max(&af, &a2, &min, &max);
	print_vector_int(&af,stdout);
	vector_int_sub(&a2, &a);
	assert(3 == VECTOR(a2)[2]);
	print_vector_int(&a2, stdout);
	vector_int_cumsum(&a2, &a);
	assert(20 == VECTOR(a2)[4]);
	print_vector_int(&a2, stdout);
	int sum = vector_int_sum(&a);
	assert(sum == VECTOR(a2)[4]);


	vector_int b;
	printf("<<<copy &a &b>>>\n");
	print_vector_int(&a, stdout);
	vector_int_copy(&b, &a);
	for (int i = 0; i < vector_int_size(&b); i++)
		assert(VECTOR(a)[i] == VECTOR(b)[i]);
	print_vector_int(&b, stdout);

	vector_int b2;
	vector_int_init_value(&b2, 5, 0,1,2,3,4,5);
	printf("<<<init_value &b2 5 0,1,2,3,4,5>>>\n");
	assert(5 == vector_int_capacity(&b2));
	assert(5 == vector_int_size(&b2));
	assert(2 == VECTOR(b2)[2]);
	print_vector_int(&b2, stdout);

	vector_int_remove_section(&b2, 1, 3);
	printf("<<<remove_section &b2 1 3>>>\n");
	assert(5 == vector_int_capacity(&b2));
	assert(3 == vector_int_size(&b2));
	assert(3 == VECTOR(b2)[1]);
	print_vector_int(&b2, stdout);

	vector_int_remove(&b2, 1);
	printf("<<<remove &b2, 1>>>\n");
	assert(5 == vector_int_capacity(&b2));
	assert(2 == vector_int_size(&b2));
	assert(4 == VECTOR(b2)[1]);
	print_vector_int(&b2, stdout);

	vector_int c;
	vector_int_init_value_end(&c, 14, 1,2,14,4);
	printf("<<<init_value_end &c 14 1,2,14,4>>>\n");
	assert(2 == vector_int_capacity(&c));
	assert(2 == vector_int_size(&c));
	assert(2 == VECTOR(c)[1]);
	print_vector_int(&c, stdout);


	vector_int v1,v2,res,res2;
	vector_int_init_value(&v1, 8, 4,5,2,3,7,1,6,1);
	vector_int_init_value(&v2, 8, 3,4,1,7,2,5,1,3);
	vector_int_init(&res, 1);
	vector_int_init(&res2, 1);
	print_vector_int(&res2, stdout);
	vector_int_order_inc2(&v1, &v2, &res, 8);
	vector_int_scan_tie(&res2, &v1, &res, 8);
	printf("<<<order&v1, &v2, &res, 7>>>\n");
	print_vector_int(&v1, stdout);
	print_vector_int(&v2, stdout);
	print_vector_int(&res, stdout);
	print_vector_int(&res2, stdout);
	assert(7 == VECTOR(res)[0]);
	assert(5 == VECTOR(res)[1]);
	assert(2 == VECTOR(res)[2]);
	assert(3 == VECTOR(res)[3]);
	assert(0 == VECTOR(res)[4]);
	assert(1 == VECTOR(res)[5]);
	assert(6 == VECTOR(res)[6]);
	assert(4 == VECTOR(res)[7]);

	assert(0 == VECTOR(res2)[0]);
	assert(0 == VECTOR(res2)[1]);
	assert(2 == VECTOR(res2)[2]);
	assert(3 == VECTOR(res2)[3]);
	assert(4 == VECTOR(res2)[4]);
	assert(5 == VECTOR(res2)[5]);
	assert(6 == VECTOR(res2)[6]);
	assert(7 == VECTOR(res2)[7]);


	vector_double d;
	vector_double_init(&d, 3);
	print_vector_double(&d, stdout);

	vector_double e;
	vector_double_init_value(&e, 3, 1.0, 1.2, 1.3);
	print_vector_double(&e, stdout);

	vector_double f;
	vector_double_init_value_end(&f, 1.4, 1.0, 1.3, 1.4, 10.0);
	print_vector_double(&f, stdout);

	vector_test_t g;
	vector_test_t_init(&g, 3);
	test_t tmp_test_t = {1,3};
	vector_test_t_set(&g, 1, tmp_test_t);
	print_vector_test_t(&g, stdout);
	assert(1 == vector_test_t_contains_op(&g, tmp_test_t, test_t_op));

	vector_char aaa;
	vector_char_init(&aaa, 4);
	VECTOR(aaa)[0] = 'a';
	VECTOR(aaa)[1] = 'a';
	VECTOR(aaa)[2] = 'a';
	VECTOR(aaa)[3] = '\0';
	printf("%s\n",VECTOR(aaa));

	vector_int haha;
	vector_int_init_value_end(&haha, -1, 3,4,7,1,6,5, -1);
	print_vector_int(&haha, stdout);
	vector_int order;
	vector_int_init(&order, 0);
	vector_int_order_inc(&haha, &order,8); 
	print_vector_int(&order, stdout);
	vector_int_order_dec(&haha, &order,8);
	print_vector_int(&order, stdout);
	vector_int_destroy(&order);
	vector_int_destroy(&haha);
	return 0;
}
