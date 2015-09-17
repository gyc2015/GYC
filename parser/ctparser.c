#include "parser.h"
#include <lex.h>
#include <input.h>
#include <error.h>

int Match(int tkType)
{
    int tmp;
    tmp = GetNextToken();
    if(tkType == tmp)
        return 1;
    Error(&TokenCoord, "Token is not match");
    return 0;
}

int find_function_ct(char** fname, stack_PPSignal *ppStack)
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


