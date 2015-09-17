
#ifndef T
#error "Specify the matrix type"
#endif

#ifndef TEMPLATE_TYPE
#error "Specify the matrix template type"
#endif

#ifndef TEMPLATE_FUNCTION
#error "Specify the matrix template function"
#endif

typedef struct TEMPLATE_TYPE(matrix)
{
	TEMPLATE_TYPE(vector) data;
	int nrow, ncol;
} TEMPLATE_TYPE(matrix);

/*
 * assessing element
 */
#ifndef MATRIX
#define MATRIX(m,i,j) ((m).data.stor_begin[(m).ncol*(i)+(j)])
#endif

/*
 * allocation
 */
int TEMPLATE_FUNCTION(matrix,init)(TEMPLATE_TYPE(matrix) *m, int nrow, int ncol);
void TEMPLATE_FUNCTION(matrix,destroy)(TEMPLATE_TYPE(matrix) *m);
int TEMPLATE_FUNCTION(matrix,copy)(TEMPLATE_TYPE(matrix) *to, const TEMPLATE_TYPE(matrix) *from);
void TEMPLATE_FUNCTION(matrix,copy_to)(const TEMPLATE_TYPE(matrix) *m, T *to);

/*
 * properties
 */
int TEMPLATE_FUNCTION(matrix,capacity)(const TEMPLATE_TYPE(matrix) *m);
int TEMPLATE_FUNCTION(matrix,size)(const TEMPLATE_TYPE(matrix) *m);
int TEMPLATE_FUNCTION(matrix,nrow)(const TEMPLATE_TYPE(matrix) *m);
int TEMPLATE_FUNCTION(matrix,ncol)(const TEMPLATE_TYPE(matrix) *m);

/*
 * initialization 
 */
void TEMPLATE_FUNCTION(matrix,null)(TEMPLATE_TYPE(matrix) *m);
void TEMPLATE_FUNCTION(matrix,fill)(TEMPLATE_TYPE(matrix) *m, T e);

/*
 * Resize Operations
 */
int TEMPLATE_FUNCTION(matrix,resize)(TEMPLATE_TYPE(matrix) *m, int nrow, int ncol);
int TEMPLATE_FUNCTION(matrix,resize_min)(TEMPLATE_TYPE(matrix) *m);
int TEMPLATE_FUNCTION(matrix,add_cols)(TEMPLATE_TYPE(matrix) *m, int n);
int TEMPLATE_FUNCTION(matrix,add_rows)(TEMPLATE_TYPE(matrix) *m, int n);

/*
 * rows & cols
 */
int TEMPLATE_FUNCTION(matrix,remove_col)(TEMPLATE_TYPE(matrix) *m, int col);
int TEMPLATE_FUNCTION(matrix,remove_row)(TEMPLATE_TYPE(matrix) *m, int row);
int TEMPLATE_FUNCTION(matrix,select_rows)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(matrix) *res, const vector_int *rows);
int TEMPLATE_FUNCTION(matrix,select_cols)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(matrix) *res, const vector_int *cols);
int TEMPLATE_FUNCTION(matrix,get_row)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(vector) *res, int index);
int TEMPLATE_FUNCTION(matrix,get_col)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(vector) *res, int index);
int TEMPLATE_FUNCTION(matrix,set_row)(TEMPLATE_TYPE(matrix) *m, const TEMPLATE_TYPE(vector) *v, int index);
int TEMPLATE_FUNCTION(matrix,set_col)(TEMPLATE_TYPE(matrix) *m, const TEMPLATE_TYPE(vector) *v, int index);
int TEMPLATE_FUNCTION(matrix,select_rows_cols)(const TEMPLATE_TYPE(matrix) *m,TEMPLATE_TYPE(matrix) *res,
											   const vector_int *rows,const vector_int *cols);


