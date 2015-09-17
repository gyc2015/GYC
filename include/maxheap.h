#ifndef MAXHEAD_H
#define MAXHEAD_H

void build_max_heap(void** a, int len,
                    int (*compare)(void**, int, int),
                    void (*swap)(void**, int, int));

void max_heapfy(void** a, int len, int i,
                int (*compare)(void**, int, int),
                void (*swap)(void**, int, int));

void heap_sort(void** a, int len,
               int (*compare)(void**, int, int),
               void (*swap)(void**, int, int));
#endif
