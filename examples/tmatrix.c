#include "matrix_int.h"
#include <assert.h>
#include <stdio.h>

int main()
{
	matrix_int a;
	matrix_int sa;
	vector_int rows;
	vector_int cols;
	printf(">>>init int a:\n");
	matrix_int_init(&a, 8, 3);
	print_matrix_int(&a,stdout);

	printf(">>>add_rows a 2\n");
	matrix_int_add_rows(&a, 2);
	print_matrix_int(&a,stdout);

	printf(">>>add_cols a 2\n");
	matrix_int_add_cols(&a, 2);
	print_matrix_int(&a,stdout);

	printf(">>>fill a 2\n");
	matrix_int_fill(&a, 2);
	print_matrix_int(&a,stdout);

	printf(">>>set row a 0\n");
	vector_int_init_value_end(&rows, -1, 0,1,2,3,4, -1);
	matrix_int_set_row(&a, &rows, 0);
	vector_int_destroy(&rows);
	vector_int_init_value_end(&rows, -1, 1,1,1,1,1, -1);
	matrix_int_set_row(&a, &rows, 1);
	vector_int_destroy(&rows);
	print_matrix_int(&a,stdout);

	printf(">>>set col a 0\n");
	vector_int_init_value_end(&cols, -1, 0,1,2,3,4,5,6,7,8,9, -1);
	matrix_int_set_col(&a, &cols, 0);
	vector_int_destroy(&cols);
	print_matrix_int(&a,stdout);

	printf(">>>select rows 0,2,3,4\n");
	vector_int_init_value_end(&rows, -1, 0,2,3,4, -1);
	matrix_int_init(&sa, 0,0);
	matrix_int_select_rows(&a, &sa, &rows);
	print_matrix_int(&sa,stdout);
	matrix_int_destroy(&sa);
	vector_int_destroy(&rows);

	printf(">>>select cols 0,2\n");
	vector_int_init_value_end(&cols, -1, 0,2, -1);
	matrix_int_init(&sa, 0,0);
	matrix_int_select_cols(&a, &sa, &cols);
	print_matrix_int(&sa,stdout);
	matrix_int_destroy(&sa);
	vector_int_destroy(&cols);

	printf(">>>select rows 0,2,3,4\n");
	printf(">>>select cols 0,2\n");
	vector_int_init_value_end(&rows, -1, 0,2,3,4, -1);
	vector_int_init_value_end(&cols, -1, 0,2, -1);
	matrix_int_init(&sa, 0,0);
	matrix_int_select_rows_cols(&a, &sa, &rows, &cols);
	vector_int_destroy(&rows);
	vector_int_destroy(&cols);
	print_matrix_int(&sa,stdout);
	matrix_int_destroy(&sa);


	printf(">>>get row a 2\n");
	vector_int_init(&rows,0);
	matrix_int_get_row(&a, &rows, 2);
	print_vector_int(&rows, stdout);
	vector_int_destroy(&rows);

	printf(">>>get col a 2\n");
	vector_int_init(&cols,0);
	matrix_int_get_col(&a, &cols, 2);
	print_vector_int(&cols, stdout);
	vector_int_destroy(&cols);


	printf(">>>null a\n");
	matrix_int_null(&a);
	print_matrix_int(&a,stdout);

	printf(">>>remove_col a 2\n");
	matrix_int_remove_col(&a,2);
	print_matrix_int(&a,stdout);

	printf(">>>remove_row a 2\n");
	matrix_int_remove_row(&a,2);
	print_matrix_int(&a,stdout);

	printf(">>>init double b\n");
	matrix_double b;
	matrix_double_init(&b, 3, 3);
	print_matrix_double(&b, stdout);
	return 0;
}
