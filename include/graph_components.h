#ifndef GRAPH_COMPONENTS_H
#define GRAPH_COMPONENTS_H

#include "graph.h"

int graph_clusters_weak(const graph_t *graph, vector_int *membership, vector_int *csize, int *no);
int graph_clusters_strong(const graph_t *graph, vector_int *membership, vector_int *csize, int *no);

#endif
