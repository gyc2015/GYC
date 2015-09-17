#include "sym.h"
#include <assert.h>

Symbol create_Symbol(int fileID, int funcID, int lineNo)
{
	Symbol result;
	result.fileID = fileID;
	result.funcID = funcID;
	result.lineNo = lineNo;
	return result;
}

int init_Symbol(Symbol *sym, int fileID, int funcID, int lineNo)
{
	assert(0 != sym);
	sym->fileID = fileID;
	sym->funcID = funcID;
	sym->lineNo = lineNo;
	return 0;
}
