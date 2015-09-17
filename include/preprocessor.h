#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <stdio.h>
#include "mm.h"

typedef struct PPSignal
{
    int nest_level; /* for {} nesting */
    int isMacro;    /* for # line */
} PPSignal;

#define T PPSignal 
#define TEMPLATE_TYPE(a) a ##_PPSignal
#define TEMPLATE_FUNCTION(a,b) a ##_PPSignal_## b
#include "stack_tmp.h"
//#include "vector_num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

PPSignal create_signal(int nest_level, int isMacro);
/*
void push(struct ppsignal *pps, struct list_head *bottom);

struct ppsignal* new_pp_signal(int nest_level, int isMacro);

struct ppsignal* pop(struct list_head *bottom);

struct ppsignal* peek(struct list_head *bottom);
*/
#endif
