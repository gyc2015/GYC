#include "maxheap.h"
#include <stdio.h>
#include <math.h>

void max_heapfy(void** a, int len, int i, int (*compare)(void**, int, int), void (*swap)(void**, int, int))
{
    int tmp = i;
    int l = (i * 2) + 1;
    int r = (i * 2) + 2;

    if (l < len && (0 < compare(a, l, i)))
        tmp = l;
    if (r < len && (0 < compare(a, r, tmp)))
        tmp = r;
    if (tmp != i){
        swap(a, i, tmp);
        max_heapfy(a, len, tmp, compare, swap);
    }
}

void build_max_heap(void** a, int len, int (*compare)(void**, int, int), void (*swap)(void**, int, int))
{
    int i = (int)floor((len - 1.0) / 2.0);
    for(;i >= 0; i--) {
        max_heapfy(a, len, i, compare, swap);
    }
}

void heap_sort(void** a, int len, int (*compare)(void**, int, int), void (*swap)(void**, int, int))
{
    build_max_heap(a, len, compare, swap);
    int i;

    for (i = len - 1; i > 0; i--) {
        swap(a, 0, i);
        len--;
        max_heapfy(a, len, 0, compare, swap);
    }
}
