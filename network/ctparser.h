#ifndef CTPARSER_H
#define CTPARSER_H

#include "dictionary.h"
#include "graph.h"
#include "vector_ptr.h"
#include <dirent.h>

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

typedef struct funcP
{
	int funcID;
	vector_ffpair ffps; /* ffpair */
} funcP;

#define T funcP
#define TEMPLATE_TYPE(a) a ##_funcP
#define TEMPLATE_FUNCTION(a,b) a ##_funcP_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int vector_funcP_init_(vector_funcP *v, int n);
ffpair* _scan_func_for_file(const funcP *funcP, int fileID);

void catch_function_call_dir(const char *path, struct dictionary *dict, vector_int *edges);
void get_function_filename(const char *path, const struct dictionary *dict, struct dictionary *filedict, vector_funcP *flist);

#endif
