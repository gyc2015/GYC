#include "parser.h"
#include <dirent.h>
#include <lex.h>
#include <input.h>
#include <error.h>
#include <assert.h>

#define T ffpair
#define TEMPLATE_TYPE(a) a ##_ffpair
#define TEMPLATE_FUNCTION(a,b) a ##_ffpair_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int ct_compare(const void* a, const void* b)
{
    if(a > b)
        return 1;
    else if(a < b)
        return -1;
    else
		return 0;
}

int is_dir(const struct dirent *unused)
{
    if(DT_DIR == unused->d_type)
        return 1;
    return 0;
}

int is_file(const struct dirent *unused)
{
    if(DT_REG == unused->d_type)
        return 1;
    return 0;
}

int match_extension(const char* file, const char* ex)
{
    char *point = strrchr(file,'.');
    if(NULL != point && 0 == strcmp(point,ex))
        return 1;
    return 0;
}

void print_funcs_ct(struct dictionary *dict, FILE *f)
{
    int nfunc = dict_size(dict);
    struct rb_node *d, *c, *tmp;

	vector_char str;
	vector_char_init(&str,100);
	VECTOR(str)[0] = '\0';


    for (int i = 0; i < nfunc; i++) {
        d = dict_ele(dict, i); 
		dic_traceback_string(d, &str);
		fprintf(f, "%s:\n",VECTOR(str));

        tmp = rb_get_minimum(DIC_BITINFO(d)->rbt, DIC_BITINFO(d)->rbt->root);
        while (DIC_BITINFO(d)->rbt->nil != tmp) {
            c = tmp->key;
			dic_traceback_string(c, &str);
			fprintf(f, "|   %s\n",VECTOR(str));
            tmp = rb_get_successor(DIC_BITINFO(d)->rbt, tmp);
        }
    }

	vector_char_destroy(&str);
}

void print_funcs_dot(struct dictionary *dict, FILE *f)
{
    int nfunc = dict_size(dict);
    struct rb_node *d, *c, *tmp;

	vector_char d_str, c_str;
	vector_char_init(&d_str,100);
	vector_char_init(&c_str,100);
	VECTOR(d_str)[0] = '\0';
	VECTOR(c_str)[0] = '\0';

	fprintf(f, "strict digraph {\n");
    for (int i = 0; i < nfunc; i++) {
        d = dict_ele(dict, i); 
		dic_traceback_string(d, &d_str);

        tmp = rb_get_minimum(DIC_BITINFO(d)->rbt, DIC_BITINFO(d)->rbt->root);
        while (DIC_BITINFO(d)->rbt->nil != tmp) {
            c = tmp->key;
			dic_traceback_string(c, &c_str);
			fprintf(f, "\t%s -> %s;\n", VECTOR(d_str), VECTOR(c_str));
            tmp = rb_get_successor(DIC_BITINFO(d)->rbt, tmp);
        }
    }
	fprintf(f, "}\n");

	vector_char_destroy(&c_str);
	vector_char_destroy(&d_str);
}

ffpair create_ffpair(int funcID, int fileID)
{
	ffpair tmpff;
	tmpff.fileID = fileID;
	tmpff.funcID = funcID;
	tmpff.ndef = 0;
	tmpff.ncal = 0;
	return tmpff;
}

ffpair* scan_func_file(vector_ffpair *fflist, int funcID, int fileID, int s)
{
	int nff = vector_ffpair_size(fflist);
	assert(s <= nff);

	int i;
	int flag = 0;
	for (i = s; i < nff; i++) {
		if (funcID == VECTOR(*fflist)[i].funcID && fileID == VECTOR(*fflist)[i].fileID) {
			flag = 1;
			break;
		}
	}

	if (1 == flag) {
		return &(VECTOR(*fflist)[i]);
	} else {
		vector_ffpair_push_back(fflist, create_ffpair(funcID, fileID));
		return &(VECTOR(*fflist)[nff]);
	}
}

int parse_file(const char *file_name, struct dictionary *func_dict, struct dictionary *file_dict, vector_ffpair *fflist)
{
    char *fname;
    struct rb_node *d = NULL, *c = NULL;
    int ft = 0;
    int ncall = 0;

	stack_PPSignal ppStack;
	stack_PPSignal_init(&ppStack, 4);
    int (*find_function)(char** fname, stack_PPSignal *ppStack);

	if (match_extension(file_name, ".c"))
		find_function = find_function_c; 
	else if (match_extension(file_name, ".ct"))
		find_function = find_function_ct; 
	else
		exit(1);

    ReadSourceFile(file_name);
    TokenCoord.pcFileName = file_name;
    TokenCoord.line = 1;
    TokenCoord.col = 1;

	struct rb_node *f = dict_add_str(file_dict, file_name);
	int fileID = DIC_BITINFO(f)->ID;
	int s = vector_ffpair_size(fflist);
    while (-1 != ft) {
        ft = find_function(&fname, &ppStack);
        if(FUNC_DEF == ft) {
			d = dict_add_str(func_dict, fname);
            DIC_BITINFO(d)->rbt->compare = ct_compare;
			int funcID = DIC_BITINFO(d)->ID;
			ffpair *tmpff = scan_func_file(fflist, funcID, fileID, s);
			(tmpff->ndef)++;
        } else if(FUNC_CALL == ft) {
			c = dict_add_str(func_dict, fname);
			DIC_BITINFO(c)->rbt->compare = ct_compare;
			if (NULL != d && NULL != c && d != c && DIC_BITINFO(d)->rbt->nil == rb_search(DIC_BITINFO(d)->rbt, c)) {
				int funcID = DIC_BITINFO(c)->ID;
				ffpair *tmpff = scan_func_file(fflist, funcID, fileID, s);
				(tmpff->ncal)++;
				ncall++;
                rb_insert(DIC_BITINFO(d)->rbt, c);
			}
        }
	}

	stack_PPSignal_destroy(&ppStack);

    CloseSourceFile();
	return ncall;
}

int parse_dir(const char *path, struct dictionary *func_dict, struct dictionary *file_dict, vector_ffpair *fflist, const char *extention)
{
    char *npath = NULL;
    struct dirent **dirs;
    struct dirent **files;
    int ncall = 0;

    int ndir = scandir(path, &dirs, is_dir, alphasort);
    int nfile = scandir(path, &files, is_file, alphasort);

    if (ndir >= 0) {
        for (int cnt = 0; cnt < ndir; cnt++) {
            if(0 == strcmp(".",dirs[cnt]->d_name) || 0 == strcmp("..",dirs[cnt]->d_name))
                continue;
            asprintf(&npath, "%s/%s", path, dirs[cnt]->d_name);
            ncall += parse_dir(npath, func_dict, file_dict, fflist, extention);
        }
    }

    if (nfile >= 0) {
        for (int cnt = 0; cnt < nfile; cnt++) {
            asprintf(&npath, "%s/%s", path, files[cnt]->d_name);
            if (match_extension(npath, extention)) {
				ncall += parse_file(npath, func_dict, file_dict, fflist);
            }
        }
    }
	return ncall;
}

int dict_2_edges(struct dictionary *dict, vector_int *edges, int nedge)
{
    int nfunc = dict_size(dict);
    struct rb_node *d, *c, *tmp;
	vector_int_resize(edges, 2 * nedge);
	int ie = 0;

    for (int i = 0; i < nfunc; i++) {
        d = dict_ele(dict, i); 
        tmp = rb_get_minimum(DIC_BITINFO(d)->rbt, DIC_BITINFO(d)->rbt->root);
        while (DIC_BITINFO(d)->rbt->nil != tmp) {
            c = tmp->key;
            VECTOR(*edges)[ie + 0] = DIC_BITINFO(d)->ID;
            VECTOR(*edges)[ie + 1] = DIC_BITINFO(c)->ID;
            tmp = rb_get_successor(DIC_BITINFO(d)->rbt, tmp);
            ie += 2;
        }
    }
    assert(ie == 2 * nedge);
	return ie;
}

