#ifndef SYM_H
#define SYM_H

typedef struct Symbol
{
	// fileID: the ID of the file in file_dict
	// funcID: the ID of the function in func_dict
	// lineNo: the number of the line that the function is found in the file 
	int fileID;
	int funcID;
	int lineNo;
} Symbol;


Symbol create_Symbol(int fileID, int funcID, int lineNo);
int init_Symbol(Symbol *sym, int fileID, int funcID, int lineNo);

/*
 * normal words and outliers
 * keyw.c
 */
void init_special_words();
int is_special_words(char *name);
int is_normal_pre_words(char *name);
int is_sys_words(char* name);
#endif
