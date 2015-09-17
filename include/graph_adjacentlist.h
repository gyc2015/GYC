#ifndef GRAPH_ADJACENTLIST_H
#define GRAPH_ADJACENTLIST_H

#include "graph_constant.h"
#include "graph_datatype.h"

typedef struct graph_adjlist_t
{ 
	int length;
	vector_int *adjs;
} graph_adjlist_t;

/*
 * allocation
 */
int graph_adjlist_init(const graph_t *graph, graph_adjlist_t *al, graph_neimode_t mode);
int graph_adjlist_init_empty(graph_adjlist_t *al, int nv);

void graph_adjlist_destroy(graph_adjlist_t *al);

#define graph_adjlist_get(al,no) (&(al)->adjs[(no)])


#endif
