
#ifndef VECTOR_NUMBER
#error "Do not include this file, if the data type is not double or int."
#endif

/*
 * allocation
 */
int TEMPLATE_FUNCTION(vector, init_value)(TEMPLATE_TYPE(vector) *v, int no, ...);
int TEMPLATE_FUNCTION(vector, init_value_end)(TEMPLATE_TYPE(vector) *v, T endmark, ...);

/*
 * properties
 */
int TEMPLATE_FUNCTION(vector, contains)(const TEMPLATE_TYPE(vector) *v, T e);
int TEMPLATE_FUNCTION(vector, isininterval)(const TEMPLATE_TYPE(vector) *v, T low, T high);

/*
 * finding maximum and minimum
 */
T TEMPLATE_FUNCTION(vector, max)(const TEMPLATE_TYPE(vector) *v);
T TEMPLATE_FUNCTION(vector, min)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, whereis)(const TEMPLATE_TYPE(vector) *v, T e); 
int TEMPLATE_FUNCTION(vector, which_max)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, which_min)(const TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector, minmax)(const TEMPLATE_TYPE(vector) *v, T *min, T *max);
int TEMPLATE_FUNCTION(vector, which_minmax)(const TEMPLATE_TYPE(vector) *v, int *which_min, int *which_max);

/*
 * vector operators
 */
int TEMPLATE_FUNCTION(vector,add_constant)(TEMPLATE_TYPE(vector) *v, T plus);
int TEMPLATE_FUNCTION(vector,add)(TEMPLATE_TYPE(vector) *v1, TEMPLATE_TYPE(vector) *v2);
int TEMPLATE_FUNCTION(vector,sub)(TEMPLATE_TYPE(vector) *v1, TEMPLATE_TYPE(vector) *v2);
T TEMPLATE_FUNCTION(vector,sum)(TEMPLATE_TYPE(vector) *v);
int TEMPLATE_FUNCTION(vector,cumsum)(TEMPLATE_TYPE(vector) *to, TEMPLATE_TYPE(vector) *from);


