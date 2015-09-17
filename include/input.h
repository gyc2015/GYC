#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

typedef struct coord
{
	const char* pcFileName;
	int line;
	int col;
} Coord;

typedef struct
{
	const char* pcFileName;
	unsigned char* pucBase;
	unsigned char* pucCursor;
    int* lineFlags;
	int numLine;
    int iLine;
	unsigned long iSize;
} Input;

void ReadSourceFile(const char *fileName);
void CloseSourceFile(void);
int GetCurLineNum();
int GetCurCol();

extern unsigned char END_OF_FILE;
extern Input input;

#endif
