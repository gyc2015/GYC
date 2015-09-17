#include "ctparser.h"
#include "input.h"
#include "lex.h"
#include "error.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FUNC_DEF 1
#define FUNC_CALL 0

#define T ffpair
#define TEMPLATE_TYPE(a) a ##_ffpair
#define TEMPLATE_FUNCTION(a,b) a ##_ffpair_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T funcP
#define TEMPLATE_TYPE(a) a ##_funcP
#define TEMPLATE_FUNCTION(a,b) a ##_funcP_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int vector_funcP_init_(vector_funcP *v, int n)
{
	vector_funcP_init(v,n);
	for (int i = 0; i < n; i++)
		vector_ffpair_init(&(VECTOR(*v)[i].ffps), 0);
}

int Match(int tkType)
{
    int tmp;
    tmp = GetNextToken();
    if(tkType == tmp)
        return 1;
    Error(&TokenCoord, "Token is not match");
    return 0;
}

int findFunc(char** fname)
{
    int tkType;
    int fType;

    tkType = GetNextToken();
    if(TK_ID == tkType) {
        *fname = CurTokenStr;
        Match(TK_COLON);
        Match(TK_NEWLINE);
        return FUNC_DEF;
    } else if (TK_BITOR == tkType) {
        Match(TK_ID);
        *fname = CurTokenStr;
        Match(TK_NEWLINE);
        return FUNC_CALL;
    } else if (TK_NOT == tkType) {
        while(tkType != TK_NEWLINE)
            tkType = GetNextToken();
    } else if (TK_END == tkType) {
        return -1;
    }
}

int match_extension(const char* file, const char* ex)
{
    char *point = strrchr(file,'.');
    if(NULL != point && 0 == strcmp(point,ex))
        return 1;
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

int ct_compare(const void* a, const void* b)
{
    if(a > b)
        return 1;
    else if(a < b)
        return -1;
    else return 0;
}

ffpair* _scan_func_for_file(const funcP *funcP, int fileID)
{
	int nfile = vector_ffpair_size(&(funcP->ffps));

	for (int i = 0; i < nfile; i++) {
		if (VECTOR(funcP->ffps)[i].fileID == fileID)
			return &(VECTOR(funcP->ffps)[i]);
	}
	return 0;
}

void _get_function_filename(const char *fileName, const struct dictionary *dict, struct dictionary *filedict,vector_funcP *funcPs)
{
	char *fname;
	struct rb_node *funcnode, *filenode;
	int ft, funcID, fileID;
	ffpair *ffp;

	ReadSourceFile(fileName);
	TokenCoord.pcFileName = fileName;
	TokenCoord.line = 1;
	TokenCoord.col = 1;

	filenode = dict_add_str(filedict, fileName);
	fileID = DIC_BITINFO(filenode)->ID;

	while (-1 != ft) {
		ft = findFunc(&fname);
		if (FUNC_DEF == ft) {
			funcnode = dict_find_str(dict, fname);
			if (dict->root->nil == funcnode) continue;
			funcID = DIC_BITINFO(funcnode)->ID;
			ffp = _scan_func_for_file(&(VECTOR(*funcPs)[funcID]),fileID);
			if (0 == ffp) {
				ffp = vector_ffpair_new_item(&(VECTOR(*funcPs)[funcID].ffps));	
				ffp->funcID = funcID;
				ffp->fileID = fileID;
				ffp->ndef = 0;
				ffp->ncal = 0;
			}
			ffp->ndef++; 
		}  else if (FUNC_CALL == ft) {
			funcnode = dict_find_str(dict, fname);
			if (dict->root->nil == funcnode) continue;
			funcID = DIC_BITINFO(funcnode)->ID;
			ffp = _scan_func_for_file(&(VECTOR(*funcPs)[funcID]),fileID);
            if (0 == ffp) {
				ffp = vector_ffpair_new_item(&(VECTOR(*funcPs)[funcID].ffps));	
				ffp->funcID = funcID;
				ffp->fileID = fileID;
				ffp->ndef = 0;
				ffp->ncal = 0;
			}
			ffp->ncal++;
        }
	}

	CloseSourceFile();
}

void get_function_filename(const char *path, const struct dictionary *dict, struct dictionary *filedict, vector_funcP *flist)
{
    char *npath = NULL;
    struct dirent **dirs;
    struct dirent **files;
    int ndir, nfile;

    ndir = scandir(path, &dirs, is_dir, alphasort);
    nfile = scandir(path, &files, is_file, alphasort); 

    if(ndir >= 0) {
        int cnt;
        for(cnt = 0; cnt < ndir; cnt++) {
            if(0 == strcmp(".", dirs[cnt]->d_name) ||
               0 == strcmp("..", dirs[cnt]->d_name))
                continue;
            asprintf(&npath,"%s/%s",path,dirs[cnt]->d_name);
            get_function_filename(npath, dict, filedict, flist);
        }
    }

    if(nfile >= 0) {
        int cnt;
        for(cnt = 0; cnt < nfile; cnt++) {
            asprintf(&npath,"%s/%s",path,files[cnt]->d_name);
            if(match_extension(npath,".ct")) {
                _get_function_filename(npath, dict, filedict, flist);
            }
        }
    }
}

int count_function_call_file(const char *fileName, struct dictionary *dict)
{
    char* fname;
    struct rb_node *d, *c;
    int ft;
    int ncall = 0;

    ReadSourceFile(fileName);
    TokenCoord.pcFileName = fileName;
    TokenCoord.line = 1;
    TokenCoord.col = 1;

    while (-1 != ft) {
        ft = findFunc(&fname);
        if (FUNC_DEF == ft) {
            d = dict_add_str(dict, fname);
            DIC_BITINFO(d)->rbt->compare = ct_compare;
        } else if (FUNC_CALL == ft) {
            c = dict_add_str(dict, fname);
            DIC_BITINFO(c)->rbt->compare = ct_compare;
            if (d != c) {
                if (DIC_BITINFO(d)->rbt->nil == rb_search(DIC_BITINFO(d)->rbt, c)) {
                    ncall++;
                    rb_insert(DIC_BITINFO(d)->rbt, c);
                }
            }
        }
    }
    CloseSourceFile();
    return ncall;
}

int count_function_call(const char *path, struct dictionary *dict)
{
    char *npath = NULL;
    struct dirent **dirs;
    struct dirent **files;
    int ndir, nfile;
    int nedge = 0;

    ndir = scandir(path, &dirs, is_dir, alphasort);
    nfile = scandir(path, &files, is_file, alphasort); 

    if(ndir >= 0) {
        int cnt;
        for(cnt = 0; cnt < ndir; cnt++) {
            if(0 == strcmp(".", dirs[cnt]->d_name) ||
               0 == strcmp("..", dirs[cnt]->d_name))
                continue;
            asprintf(&npath,"%s/%s",path,dirs[cnt]->d_name);
            nedge += count_function_call(npath, dict);
        }
    }

    if(nfile >= 0) {
        int cnt;
        for(cnt = 0; cnt < nfile; cnt++) {
            asprintf(&npath,"%s/%s",path,files[cnt]->d_name);
            if(match_extension(npath,".ct")) {
                nedge += count_function_call_file(npath, dict);
            }
        }
    }
    return nedge;
}

void _catch_function_call_dir(struct dictionary *dict, vector_int *edges, int *ie)
{
    int nfunc = dict_size(dict);
    struct rb_node *d, *c, *tmp;

    for (int i = 0; i < nfunc; i++) {
        d = dict_ele(dict, i); 
        tmp = rb_get_minimum(DIC_BITINFO(d)->rbt, DIC_BITINFO(d)->rbt->root);
        while (DIC_BITINFO(d)->rbt->nil != tmp) {
            c = tmp->key;
            VECTOR(*edges)[*ie + 0] = DIC_BITINFO(d)->ID;
            VECTOR(*edges)[*ie + 1] = DIC_BITINFO(c)->ID;
            tmp = rb_get_successor(DIC_BITINFO(d)->rbt, tmp);
            *ie += 2;
        }
    }
}

void catch_function_call_dir(const char *path, struct dictionary *dict, vector_int *edges)
{
    int nedge = count_function_call(path, dict);
    vector_int_init(edges, 2 * nedge);

    printf("nedge = %d\n",nedge);
    
    int nedge2 = 0;
    _catch_function_call_dir(dict, edges, &nedge2);
    assert(nedge2 == 2 * nedge);
}

/*
 *
 */
