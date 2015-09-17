#include "input.h"
#include <stdlib.h>

unsigned char END_OF_FILE = 255;
Input input;
FILE* pFile;

long MapFile();

/**
 * Reads the whole source file into memory.
 */
void ReadSourceFile(const char* pFileName)
{
    pFile = fopen(pFileName, "r");

	if(NULL == pFile)
	{
		printf("Can't open file %s", pFileName);
	}
	else
	{
        input.iSize = MapFile();
		fclose(pFile);

		input.pcFileName = pFileName;
		input.pucBase[input.iSize] = END_OF_FILE;
		input.pucCursor = input.pucBase;

		input.iLine = 0;
	}
}

void CloseSourceFile(void)
{
	free(input.pucBase);
    free(input.lineFlags);
}

int GetCurLineNum()
{
    int offset = input.pucCursor - input.pucBase;
    
    if(input.iSize < offset)
        fprintf(stderr,"Error:in GetCurLineNum, input.c");
    while(input.lineFlags[input.iLine + 1] < offset)
        input.iLine++;

    return input.iLine + 1;
}

int GetCurCol()
{
    int offset = input.pucCursor - input.pucBase;
    GetCurLineNum();
    return offset - input.lineFlags[input.iLine] + 1;
}

long MapFile()
{
	int times = 1;
    int ltimes = 1;
    input.pucBase = (unsigned char*)malloc(sizeof(unsigned char) * 1024);
    input.lineFlags = (int*)malloc(sizeof(int) * 512);

	long index = 0;
    int lineIndex = 0;
    input.lineFlags[lineIndex] = 0;
	while(!feof(pFile))
	{
		if(index == (times * 1024 - 1))
		{
			times++;
			input.pucBase = (unsigned char*) realloc(input.pucBase,times * 1024 * sizeof(unsigned char));
		}

        if(lineIndex == (ltimes * 512 - 1))
        {
            ltimes++;
            input.lineFlags = (int*)realloc(input.lineFlags, ltimes * 512 * sizeof(int));
        }

		input.pucBase[index] = fgetc(pFile);
        if('\n' == input.pucBase[index])
        {
            if('\\'==input.pucBase[index-1])
                index-=2;
            lineIndex++;
            input.lineFlags[lineIndex] = index + 1;
        }

		index++;
	}
    input.numLine = lineIndex;
	return index;
}
