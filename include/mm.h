#ifndef MM_H
#define MM_H

#include <stdlib.h>
#include <string.h>

#define Calloc(n, t) (t*) calloc((size_t)(n), sizeof(t))
#define Realloc(p, n, t) (t*) realloc((void*)(p), ((size_t)(n)*sizeof(t)))
#define Free(p) (free((void*)(p)), (p) = NULL)

#endif
