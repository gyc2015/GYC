#ifndef CPARSER_H
#define CPARSER_H

#include "preprocessor.h"
#include <dictionary.h>

#define FUNC_DEF 1
#define FUNC_CALL 2

typedef struct ffpair
{
	int funcID;
	int fileID;
	int ndef;
	int ncal;
} ffpair;

#define T ffpair
#define TEMPLATE_TYPE(a) a ##_ffpair
#define TEMPLATE_FUNCTION(a,b) a ##_ffpair_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int find_function_c(char** fname, stack_PPSignal *ppStack);
int find_function_ct(char** fname, stack_PPSignal *ppStack);

int parse_file(const char *file_name, struct dictionary *func_dict, struct dictionary *file_dict, vector_ffpair *fflist);
int parse_dir(const char *path, struct dictionary *func_dict, struct dictionary *file_dict, vector_ffpair *fflist, const char *extention);
int dict_2_edges(struct dictionary *dict, vector_int *edges, int nedge);

void print_funcs_ct(struct dictionary *dict, FILE *f);
void print_funcs_dot(struct dictionary *dict, FILE *f);
#endif
