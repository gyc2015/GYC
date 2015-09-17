#include "sym.h"
#include "parser.h"
#include <lex.h>
#include <input.h>
#include <error.h>

int isMacro = 0;   /* for # line */
int nestlevel = 0; /* for {} nesting, used by parser */

Symbol *dsym = NULL;

void findNewLine(int *tkType)
{
    while (*tkType != TK_NEWLINE && *tkType != TK_END) {
        *tkType = GetNextToken();
	}
}

int findRParen(int n)
{
    int tkType;

    while ((tkType = GetNextToken()) != TK_END) {
        switch (tkType) {
        case TK_LPAREN:
            n++;
            break;
        case TK_RPAREN:
            n--;
            break;
        }
        
        if (0 == n)
            return 1;
    }
    return 0;
}

int checkMac(char** fname, stack_PPSignal *ppStack)
{
    int tkType;
    tkType = GetNextToken();
    
    isMacro = 1;
    if (0 != CurTokenStr) {
        if (0 == strcmp("define",CurTokenStr)) {
            GetNextToken();
            *fname = CurTokenStr;
            BeginPeekToken();
            tkType = GetNextToken();
            EndPeekToken();
            if(tkType == TK_LPAREN && 0 == LenWhiteSpace) {
                if(!findRParen(0))
                    return -1;
                return FUNC_DEF;
            }
		} else if ( 0 == strcmp("ifdef", CurTokenStr) ||
                    0 == strcmp("ifndef",CurTokenStr) ||
                    0 == strcmp("if", CurTokenStr)) {
			stack_PPSignal_push(ppStack, create_signal(nestlevel, isMacro));
        } else if ( 0 == strcmp("elif", CurTokenStr) ||
                    0 == strcmp("else", CurTokenStr)) {
			PPSignal temp = stack_PPSignal_top(ppStack);
			nestlevel = temp.nest_level;
			isMacro = temp.isMacro;
        } else if (0 == strcmp("endif",CurTokenStr)) {
			PPSignal temp = stack_PPSignal_pop(ppStack);
        }
    }

    findNewLine(&tkType);
    isMacro = 0;
    return -1;
}

void ignore_normal_pre_words(char** fname)
{
    BeginPeekToken();
    int tkType;
    tkType = GetNextToken();
    while (TK_NEWLINE == tkType)
        tkType = GetNextToken();
    if (TK_ID == tkType) {
        tkType = GetNextToken();
        if(TK_LPAREN == tkType && findRParen(1)) {
            EndPeekToken();
            GetNextToken();
            *fname = CurTokenStr;
            findRParen(0);
            return;
        }
    }

    EndPeekToken();
}

int checkFunc_DEF(char** fname)
{
    char* temp;
    int line, col;
    const char* fileName;
    int n = 0;

    if(!findRParen(0))
        return -1;

    if(is_normal_pre_words(*fname) && !isMacro)
        ignore_normal_pre_words(fname);

    if (!isMacro) {
        BeginPeekToken();
        int tkType;
        tkType = GetNextToken();
        while (TK_NEWLINE == tkType)
            tkType = GetNextToken();
        
        while (TK_ID == tkType && is_special_words(CurTokenStr)) {
            temp = CurTokenStr;
            line = TokenCoord.line;
            col = TokenCoord.col;
            fileName = TokenCoord.pcFileName;
            tkType = GetNextToken();
            if(TK_LPAREN == tkType && findRParen(1))
                n++;
            tkType = GetNextToken();
            while (tkType == TK_NEWLINE)
                tkType = GetNextToken();
        }
        EndPeekToken();
        
        while (n > 0) {
            tkType = GetNextToken();
            while(TK_NEWLINE == tkType)
                tkType = GetNextToken();

            if (TK_ID == tkType && is_special_words(CurTokenStr)) {
                temp = CurTokenStr;
                line = TokenCoord.line;
                col = TokenCoord.col;
                fileName = TokenCoord.pcFileName;
                tkType = GetNextToken();
                if (TK_LPAREN == tkType && findRParen(1))
                    n--;
            }
        }
    }
    return FUNC_DEF;
}

void ignore_sys_words(char** fname)
{
    int tkType;
    char* tmp;

    asprintf(&tmp,"sys_");

    BeginPeekToken();
    tkType = GetNextToken();
    while (TK_ID != tkType)
        tkType = GetNextToken();
    EndPeekToken();

    asprintf(fname,"%s%s",tmp,CurTokenStr);
}

int checkFunc(char** fname)
{
	if (is_sys_words(*fname))
		ignore_sys_words(fname);

    if(nestlevel == 0) {
        return checkFunc_DEF(fname);
    } else {
        if(!findRParen(0))
            return -1;
        return FUNC_CALL;
    }
}

int find_function_c(char** fname, stack_PPSignal *ppStack)
{
    int tkType;
    int fType;
    char* temp = NULL;

    while((tkType = GetNextToken()) != TK_END) {
		switch(tkType) {
        case TK_LBRACE:
            if(!isMacro)
                nestlevel++;
            break;
        case TK_RBRACE:
            if(!isMacro)
                nestlevel--;
            if (nestlevel == 0)
                dsym = 0;
            else if(nestlevel < 0) {
                Error(&TokenCoord,"error: Found '}' without open");
			}
            break;
        case TK_HASH:
            fType = checkMac(fname, ppStack);
            if (FUNC_DEF == fType)
                return fType;
            break;
        case TK_NEWLINE:
            isMacro = 0;
            break;
        case TK_ID:
            temp = CurTokenStr;
            BeginPeekToken();
            tkType = GetNextToken();
            EndPeekToken();
            if(tkType == TK_LPAREN) {
                fType = checkFunc(&temp);
                if(0 <= fType) {
                    *fname = temp;
                    return fType;
                }
            }
            break;
		default:
			break;
		}
	}

	return -1;
}
