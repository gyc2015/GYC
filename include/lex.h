#ifndef LEX_H
#define LEX_H

enum token
{
	TK_BEGIN,
#define TOKEN(k, s) k,
#include "token.h"
#undef  TOKEN
	TK_KEYWORD
};

#define IsDigit(c)         (c >= '0' && c <= '9')
#define IsOctDigit(c)      (c >= '0' && c <= '7')
#define IsHexDigit(c)      (IsDigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))
#define IsLetter(c)        ((c >= 'a' && c <= 'z') || (c == '_') || (c >= 'A' && c <= 'Z'))
#define IsLetterOrDigit(c) (IsLetter(c) || IsDigit(c))
#define ToUpper(c)		   (c & ~0x20)

void initkeyw();
int IsKeyWord(char* name);
void PrintKeyWord();

void SetupLexer();
void BeginPeekToken(void);
void EndPeekToken(void);
int GetNextToken(void);
int PeekNextToken();
extern int LenWhiteSpace; 
extern unsigned char* CurTokenStr;
extern struct coord TokenCoord;
extern struct coord PrevCoord;
extern char* TokenStrings[];

#endif
