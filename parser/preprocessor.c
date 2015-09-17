#include "preprocessor.h"
#include <stdlib.h>
#include <assert.h>

#define T PPSignal 
#define TEMPLATE_TYPE(a) a ##_PPSignal
#define TEMPLATE_FUNCTION(a,b) a ##_PPSignal_## b
#include "stack.tmp.h"
//#include "vector.num.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

PPSignal create_signal(int nest_level, int isMacro)
{
	PPSignal result;
	result.nest_level = nest_level;
	result.isMacro = isMacro;
	return result;
}
