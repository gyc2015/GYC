#include "input.h"
#include "lex.h"
#include "keyword.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>

#define CURSOR      (input.pucCursor)
typedef int (*Scanner)(void);
static Scanner Scanners[256];
static unsigned char* pucPeekPoint;
static Coord pPeekCoord;

unsigned char* CurTokenStr = 0;
int LenWhiteSpace;
Coord TokenCoord;
Coord PrevCoord;

static void SkipWhiteSpace()
{
    unsigned char* start = CURSOR;
	while(' ' == CURSOR[0] || '\t' == CURSOR[0] || '\r' == CURSOR[0] ||
          '\f'== CURSOR[0] ||  '$' == CURSOR[0])
	{
		CURSOR++;
	}
    LenWhiteSpace = CURSOR - start;
}

static int FindKeyword(char *str, int len)
{
	struct keyword *p = NULL;
	int index = 0;

	if (*str != '_')
		index = ToUpper(*str) - 'A' + 1;

	p = keywords[index];
	while (p->name)
	{
		if (p->len == len && strncmp(str, p->name, len) == 0)
			break;
		p++;
	}
	return p->tok;
}

static int ScanCharLiteral()
{
    if('L' == *CURSOR)
    {
        CURSOR++;
    }
    CURSOR++;

    while('\'' != *CURSOR)
    {
        if('\\' == *CURSOR)
           CURSOR++;
        CURSOR++; 
    }
    CURSOR++;
    return TK_INTCONST; 
}

static int ScanStringLiteral()
{
    if('L' == *CURSOR)
    {
        CURSOR++;
    }
    CURSOR++;

    while('\"' != *CURSOR)
    {
        if('\\' == *CURSOR)
            CURSOR++;
        CURSOR++;        
    }
    CURSOR++;

	return TK_STRING;
}

static int ScanFloatLiteral()
{
    if('.' == *CURSOR)
    {
        CURSOR++;
        while(IsDigit(*CURSOR))
        {
            CURSOR++;
        }
    }

    if('e' == *CURSOR || 'E' == *CURSOR)
    {
        CURSOR++;
        if('+' == *CURSOR || '-' == *CURSOR)
            CURSOR++;
        while(IsDigit(*CURSOR))
            CURSOR++;
    }

    while('f' == *CURSOR || 'F' == *CURSOR ||
          'l' == *CURSOR || 'L' == *CURSOR)
        CURSOR++;

	return TK_FLOATCONST;
}

static int ScanNumericLiteral()
{
    int base = 10;
    if('.' == *CURSOR)
    {
        return ScanFloatLiteral();
    }

    if('0' == *CURSOR && (CURSOR[1] == 'x'|| CURSOR[1] == 'X'))
    {
        base = 16;
        CURSOR += 2;
        while(IsHexDigit(*CURSOR))
        {
            CURSOR++;
        }
    }
    else if('0' == *CURSOR)
    {
        base = 8;
        CURSOR++;
        while(IsOctDigit(*CURSOR))
        {
            CURSOR++;
        }
    }
    else
    {
        CURSOR++;
        while(IsDigit(*CURSOR))
        {
            CURSOR++;
        }
    }

    if(base == 16 || (*CURSOR == '.' && *CURSOR == 'e' && *CURSOR == 'E'))
    {
        while('u' == *CURSOR || 'U' == *CURSOR ||
              'l' == *CURSOR || 'L' == *CURSOR)
        {
            CURSOR++;
        }
        return TK_INTCONST;
    }
    else
    {
        return ScanFloatLiteral();
    }
}

static int ScanIdentifier()
{
    unsigned char* start = CURSOR;
    if('L' == *CURSOR)
    {
        if('\'' == CURSOR[1])
        {
            return ScanCharLiteral();
        }
        else if('\"' == CURSOR[1])
        {
            return ScanStringLiteral();
        }
    }

    CURSOR++;
    while(IsLetterOrDigit(*CURSOR))
    {
        CURSOR++;
    }
    


    return FindKeyword((char *)start, (int)(CURSOR - start));
}

static int ScanPlus(void)
{
	CURSOR++;
	if (*CURSOR == '+')
	{
		CURSOR++;
		return TK_INC;
	}
	else if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_ADD_ASSIGN;
	}
	else
	{
		return TK_ADD;
	}
}

static int ScanMinus(void)
{
	CURSOR++;
	if (*CURSOR == '-')
	{
		CURSOR++;
		return TK_DEC;
	}
	else if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_SUB_ASSIGN;
	}
	else if (*CURSOR == '>')
	{
		CURSOR++;
		return TK_POINTER;
	}
	else
	{
		return TK_SUB;
	}
}

static int ScanStar(void)
{
	CURSOR++;
	if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_MUL_ASSIGN;
	}
	else
	{
		return TK_MUL;
	}
}

static int ScanSlash(void)
{
	CURSOR++;
	if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_DIV_ASSIGN;
	}
    else if(*CURSOR == '/')
    {
        CURSOR++;
        while('\n' != *CURSOR)
        {
            if('\\' == *CURSOR)
            {
                CURSOR++;
            }
            CURSOR++;
        }
        return TK_COMMENT;
    }
    else if(*CURSOR == '*')
    {
        CURSOR++;
        while('*' != *CURSOR || '/' != CURSOR[1])
        {
            CURSOR++;
        }
        CURSOR+=2;
        return TK_COMMENT;
    }
	else
	{
		return TK_DIV;
	}
}


static int ScanPercent(void)
{
	CURSOR++;
	if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_MOD_ASSIGN;
	}
	else
	{
		return TK_MOD;
	}
}

static int ScanLess(void)
{
	CURSOR++;
	if (*CURSOR == '<')
	{
		CURSOR++;
		if (*CURSOR == '=')
		{
			CURSOR++;
			return TK_LSHIFT_ASSIGN;
		}
		return TK_LSHIFT;
	}
	else if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_LESS_EQ;
	}
	else
	{
		return TK_LESS;
	}
}

static int ScanGreat(void)
{
	CURSOR++;
	if (*CURSOR == '>')
	{
		CURSOR++;
		if (*CURSOR == '=')
		{
			CURSOR++;
			return TK_RSHIFT_ASSIGN;
		}
		return TK_RSHIFT;
	}
	else if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_GREAT_EQ;
	}
	else
	{
		return TK_GREAT;
	}
}

static int ScanExclamation(void)
{
	CURSOR++;
	if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_UNEQUAL;
	}
	else
	{
		return TK_NOT;
	}
}

static int ScanEqual(void)
{
	CURSOR++;
	if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_EQUAL;
	}
	else
	{
		return TK_ASSIGN;
	}
}

static int ScanBar(void)
{
	CURSOR++;
	if (*CURSOR == '|')
	{
		CURSOR++;
		return TK_OR;
	}
	else if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_BITOR_ASSIGN;
	}
	else
	{
		return TK_BITOR;
	}
}

static int ScanAmpersand(void)
{
	CURSOR++;
	if (*CURSOR == '&')
	{
		CURSOR++;
		return TK_AND;
	}
	else if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_BITAND_ASSIGN;
	}
	else
	{
		return TK_BITAND;
	}
}

static int ScanCaret(void)
{
	CURSOR++;
	if (*CURSOR == '=')
	{
		CURSOR++;
		return TK_BITXOR_ASSIGN;
	}
	else
	{
		return TK_BITXOR;
	}
}

static int ScanDot(void)
{
	if (IsDigit(CURSOR[1]))
	{
		return ScanFloatLiteral(CURSOR);
	}
	else if (CURSOR[1] == '.' && CURSOR[2] == '.')
	{
		CURSOR += 3;
		return TK_ELLIPSE;
	}
	else
	{
		CURSOR++;
		return TK_DOT;
	}
}

#define SINGLE_CHAR_SCANNER(t) \
static int Scan##t(void)       \
{                              \
    CURSOR++;                  \
    return TK_##t;             \
}

SINGLE_CHAR_SCANNER(LBRACE)
SINGLE_CHAR_SCANNER(RBRACE)
SINGLE_CHAR_SCANNER(LBRACKET)
SINGLE_CHAR_SCANNER(RBRACKET)
SINGLE_CHAR_SCANNER(LPAREN)
SINGLE_CHAR_SCANNER(RPAREN)
SINGLE_CHAR_SCANNER(COMMA)
SINGLE_CHAR_SCANNER(SEMICOLON)
SINGLE_CHAR_SCANNER(COMP)
SINGLE_CHAR_SCANNER(QUESTION)
SINGLE_CHAR_SCANNER(COLON)

static int ScanHASH()
{
    CURSOR++;
    return TK_HASH;
}



static int ScanBadChar(void)
{
    Error(&TokenCoord,"illegal character:\\x%x",*CURSOR);
    CURSOR++;
    return GetNextToken();
}

static int ScanEOF(void)
{
	return TK_END;
}

static int ScanNewLine()
{
	CURSOR++;
	return TK_NEWLINE;
}

void SetupLexer()
{
	int i;

	for (i = 0; i < END_OF_FILE + 1; i++)
	{
		if (IsLetter(i))
		{
			Scanners[i] = ScanIdentifier;
		}
		else if (IsDigit(i))
		{
			Scanners[i] = ScanNumericLiteral;
		}
		else
		{
			Scanners[i] = ScanBadChar;
		}
	}

	Scanners[END_OF_FILE] = ScanEOF;
	Scanners['\''] = ScanCharLiteral;
	Scanners['"']  = ScanStringLiteral;
	Scanners['+']  = ScanPlus;
	Scanners['-']  = ScanMinus;
	Scanners['*']  = ScanStar;
	Scanners['/']  = ScanSlash;
    Scanners['%']  = ScanPercent;
	Scanners['<']  = ScanLess;
	Scanners['>']  = ScanGreat;
	Scanners['!']  = ScanExclamation;
	Scanners['=']  = ScanEqual;
	Scanners['|']  = ScanBar;
	Scanners['&']  = ScanAmpersand;
	Scanners['^']  = ScanCaret;
	Scanners['.']  = ScanDot;
	Scanners['{']  = ScanLBRACE;
	Scanners['}']  = ScanRBRACE;
	Scanners['[']  = ScanLBRACKET;
	Scanners[']']  = ScanRBRACKET;
	Scanners['(']  = ScanLPAREN;
	Scanners[')']  = ScanRPAREN;
	Scanners[',']  = ScanCOMMA;
	Scanners[';']  = ScanSEMICOLON;
	Scanners['~']  = ScanCOMP;
	Scanners['?']  = ScanQUESTION;
	Scanners[':']  = ScanCOLON;
	Scanners['#']  = ScanHASH;
    Scanners['\n'] = ScanNewLine;
}

int GetNextToken()
{
    int tok;
    
    LenWhiteSpace = 0;
    PrevCoord = TokenCoord;
    SkipWhiteSpace();

    TokenCoord.line = GetCurLineNum();
    TokenCoord.col = GetCurCol();

    unsigned char *start = CURSOR;
    tok = (*Scanners[*CURSOR])();
    int length = CURSOR-start;
    CurTokenStr = (unsigned char*)malloc(length* sizeof(char) + 1);
    strncpy((char*)CurTokenStr, (char*)start, length);
    CurTokenStr[length] = '\0';
    
    return tok;
}

void BeginPeekToken(void)
{
	pucPeekPoint = CURSOR;
	pPeekCoord = TokenCoord;
}

void EndPeekToken(void)
{
	CURSOR = pucPeekPoint;
	TokenCoord = pPeekCoord;
}

int PeekNextToken()
{
    BeginPeekToken();
    int re = GetNextToken();
    EndPeekToken();
    return re;
}
