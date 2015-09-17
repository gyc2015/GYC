#include <assert.h>
#include <stdarg.h>

#ifndef T
#error "Specify the matrix type"
#endif

#ifndef TEMPLATE_TYPE
#error "Specify the matrix template type"
#endif

#ifndef TEMPLATE_FUNCTION
#error "Specify the matrix template function"
#endif

/*
 * matrix_T_init
 */
int TEMPLATE_FUNCTION(matrix,init)(TEMPLATE_TYPE(matrix) *m, int nrow, int ncol)
{
	int ret1;
	ret1=TEMPLATE_FUNCTION(vector,init)(&m->data, nrow*ncol);
	m->nrow=nrow;
	m->ncol=ncol;
	return ret1;
}

/*
 * matrix_T_destroy
 */
void TEMPLATE_FUNCTION(matrix,destroy)(TEMPLATE_TYPE(matrix) *m)
{
	TEMPLATE_FUNCTION(vector,destroy)(&m->data);
}

/*
 * matrix_T_copy
 */
int TEMPLATE_FUNCTION(matrix,copy)(TEMPLATE_TYPE(matrix) *to, const TEMPLATE_TYPE(matrix) *from)
{
	to->nrow = from->nrow;
	to->ncol = from->ncol;
	return TEMPLATE_FUNCTION(vector,copy)(&to->data, &from->data);
}

/*
 * matrix_T_copy_to
 */
void TEMPLATE_FUNCTION(matrix,copy_to)(const TEMPLATE_TYPE(matrix) *m, T *to)
{
	TEMPLATE_FUNCTION(vector,copy_to)(&m->data, to);
}

/*
 * matrix_T_null
 */
void TEMPLATE_FUNCTION(matrix,null)(TEMPLATE_TYPE(matrix) *m)
{
	TEMPLATE_FUNCTION(vector,null)(&m->data);
}

/*
 * matrix_T_fill
 */
void TEMPLATE_FUNCTION(matrix,fill)(TEMPLATE_TYPE(matrix) *m, T e)
{
	TEMPLATE_FUNCTION(vector,fill)(&m->data, e);
}

/*
 * matrix_T_capacity
 */
int TEMPLATE_FUNCTION(matrix,capacity)(const TEMPLATE_TYPE(matrix) *m)
{
	return TEMPLATE_FUNCTION(vector,capacity)(&m->data);
}

/*
 * matrix_T_size
 */
int TEMPLATE_FUNCTION(matrix,size)(const TEMPLATE_TYPE(matrix) *m)
{
	return (m->nrow) * (m->ncol);
}

/*
 * matrix_T_nrow
 */
int TEMPLATE_FUNCTION(matrix,nrow)(const TEMPLATE_TYPE(matrix) *m)
{
	return m->nrow;
}

/*
 * matrix_T_ncol
 */
int TEMPLATE_FUNCTION(matrix,ncol)(const TEMPLATE_TYPE(matrix) *m)
{
	return m->ncol;
}

/*
 * matrix_T_resize
 * the matrix contains arbitrary data after resize.
 */
int TEMPLATE_FUNCTION(matrix,resize)(TEMPLATE_TYPE(matrix) *m, int nrow, int ncol)
{
	TEMPLATE_FUNCTION(vector,resize)(&m->data, nrow*ncol);
	m->nrow=nrow;
	m->ncol=ncol;
	return 0;
}

/*
 * matrix_T_resize_min
 */
int TEMPLATE_FUNCTION(matrix,resize_min)(TEMPLATE_TYPE(matrix) *m)
{
	TEMPLATE_TYPE(vector) tmp;
	int size = TEMPLATE_FUNCTION(matrix,size)(m);
	int capacity = TEMPLATE_FUNCTION(matrix,capacity)(m);
	if (size == capacity) { return 0; }

	TEMPLATE_FUNCTION(vector, init)(&tmp,size);
	TEMPLATE_FUNCTION(vector,update)(&tmp, &m->data);
	TEMPLATE_FUNCTION(vector,destroy)(&m->data);
	m->data = tmp;
	return 0;
}

/*
 * matrix_T_add_cols
 */
int TEMPLATE_FUNCTION(matrix,add_cols)(TEMPLATE_TYPE(matrix) *m, int n)
{
	TEMPLATE_FUNCTION(vector,resize)(&m->data, (m->nrow)*(m->ncol+n));
	for (int i = m->nrow - 1; i >=0; i--) {
		TEMPLATE_FUNCTION(vector, move_interval2)(&m->data, (m->ncol)*i, (m->ncol)*(i+1), (m->ncol + n) * i);
	}
	m->ncol += n;
	return 0;
}

/*
 * matrix_T_add_rows
 */
int TEMPLATE_FUNCTION(matrix,add_rows)(TEMPLATE_TYPE(matrix) *m, int n)
{
	TEMPLATE_FUNCTION(vector,resize)(&m->data, (m->nrow+n)*(m->ncol));
	m->nrow += n;
	return 0;
}

/*
 * matrix_T_remove_col
 */
int TEMPLATE_FUNCTION(matrix,remove_col)(TEMPLATE_TYPE(matrix) *m, int col)
{
	assert(col < m->ncol);
	int leap = 0;
	int index = 0;

	for (int r = 0; r < m->nrow; r++) {
		for (int c = 0; c < m->ncol; c++) {
			if (c == col)
				leap++;
			else
				VECTOR(m->data)[index - leap] = VECTOR(m->data)[index];
			index++;
		}
		leap++;
	}
	m->ncol--;
	TEMPLATE_FUNCTION(vector, resize)(&m->data, m->nrow * m->ncol);
	return 0;
}

/*
 * matrix_T_remove_row
 */
int TEMPLATE_FUNCTION(matrix,remove_row)(TEMPLATE_TYPE(matrix) *m, int row)
{
	TEMPLATE_FUNCTION(vector,remove_section)(&m->data, (m->ncol)*row, (m->ncol)*(row+1));
	m->nrow--;
	return 0;
}

/*
 * matrix_T_select_rows
 */
int TEMPLATE_FUNCTION(matrix,select_rows)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(matrix) *res, const vector_int *rows)
{
	int norows = vector_int_size(rows);
	int nocols = m->ncol;

	TEMPLATE_FUNCTION(matrix,resize)(res, norows, nocols);
  
	for (int i=0; i<norows; i++) {
		for (int j=0; j<nocols; j++) {
			MATRIX(*res, i, j) = MATRIX(*m, (long int)VECTOR(*rows)[i], j);
		}
	}
	return 0;
}

/*
 * matrix_T_select_cols
 */
int TEMPLATE_FUNCTION(matrix,select_cols)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(matrix) *res, const vector_int *cols)
{
	int norows = m->nrow;
	int nocols = vector_int_size(cols);

	TEMPLATE_FUNCTION(matrix,resize)(res, norows, nocols);
  
	for (int i=0; i<norows; i++) {
		for (int j=0; j<nocols; j++) {
			MATRIX(*res, i, j) = MATRIX(*m, i, (long int)VECTOR(*cols)[j]);
		}
	}
	return 0;
}

/*
 * matrix_T_get_row
 */
int TEMPLATE_FUNCTION(matrix,get_row)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(vector) *res, int index)
{
	assert(index < m->nrow);
	int ncol = m->ncol;
	TEMPLATE_FUNCTION(vector,get_interval)(&m->data, res, ncol*index, ncol*(index+1));
	return 0;
}

/*
 * matrix_T_get_col
 */
int TEMPLATE_FUNCTION(matrix,get_col)(const TEMPLATE_TYPE(matrix) *m, TEMPLATE_TYPE(vector) *res, int index)
{
	assert(index < m->ncol);
	int nrow = m->nrow;
	int ncol = m->ncol;
  
	TEMPLATE_FUNCTION(vector,resize)(res,nrow);
  
	int j = index;
	for (int i = 0; i < nrow; i++) {
		VECTOR(*res)[i] = VECTOR(m->data)[j];
		j += ncol;
	}
	return 0;
}

/*
 * matrix_T_set_row
 */
int TEMPLATE_FUNCTION(matrix,set_row)(TEMPLATE_TYPE(matrix) *m, const TEMPLATE_TYPE(vector) *v, int index)
{
	assert(index < m->nrow);
	assert(m->ncol == TEMPLATE_FUNCTION(vector,size)(v));

	for (int j = 0; j < m->ncol; j++) {
		MATRIX(*m,index,j) = VECTOR(*v)[j];
	}
	return 0;
}

/*
 * matrix_T_set_col
 */
int TEMPLATE_FUNCTION(matrix,set_col)(TEMPLATE_TYPE(matrix) *m, const TEMPLATE_TYPE(vector) *v, int index)
{
	assert(index < m->ncol);
	assert(m->nrow == TEMPLATE_FUNCTION(vector,size)(v));

	for (int i = 0; i < m->nrow; i++) {
		MATRIX(*m,i,index) = VECTOR(*v)[i];
	}
	return 0;

}

/*
 * matrix_T_select_rows_cols
 */
int TEMPLATE_FUNCTION(matrix,select_rows_cols)(const TEMPLATE_TYPE(matrix) *m,TEMPLATE_TYPE(matrix) *res,
											   const vector_int *rows,const vector_int *cols)
{
	int nrows = vector_int_size(rows);
	int ncols = vector_int_size(cols);

	TEMPLATE_FUNCTION(matrix, resize)(res,nrows,ncols);

	for (int i = 0; i < nrows; i++)
		for (int j = 0; j < ncols; j++)
			MATRIX(*res,i,j) = MATRIX(*m,VECTOR(*rows)[i],VECTOR(*cols)[j]);
  
	return 0;
}

