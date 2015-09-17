#ifndef GRAPH_H
#define GRAPH_H

#include "graph_datatype.h"
#include "graph_iterator.h"

/*
 * graph constructors
 */
int new_graph(graph_t *graph, vector_int *edges, int n, int directed);
int new_graph_empty(graph_t *graph, int n, int directed);
int new_graph_empty_attrs(graph_t *graph, int n, int directed, void *attr);
int graph_combine_vertices(const graph_t *graph, const vector_int *membership, graph_t *res);
int graph_destroy(graph_t *graph);
int graph_neighbor_subgraph(const graph_t *graph, graph_t *subgraph, vector_int *neis, int s, graph_neimode_t mode);
int graph_subgraph(const graph_t *graph, graph_t *subgraph, graph_vs_t vids);

/*
 * maintainance
 */
void graph_reverse(graph_t *graph);
void graph_remove_multi_edges(graph_t *graph);

/*
 * elements
 */
int graph_edge(const graph_t *graph, int eid, int *from, int *to);
int graph_edges(const graph_t *graph, graph_es_t eids,vector_int *edges);
#define GRAPH_FROM(g,e) ((VECTOR((g)->from)[(e)]))
#define GRAPH_TO(g,e)   ((VECTOR((g)->to)[(e)]))

/*
 * properties
 */
int graph_edges_count(const graph_t *graph);
int graph_vertices_count(const graph_t *graph);
int graph_is_directed(const graph_t *graph);
int graph_has_multiple(const graph_t *graph);
int graph_edge_contains(const graph_t *graph, int v, int w, graph_neimode_t mode);
/*
 * change the graph
 */
int graph_add_vertices(graph_t *graph, int nv, void *attr);
int graph_add_edges(graph_t *graph, vector_int *edges, void *attr);
int graph_del_edges(graph_t *graph, graph_es_t eids);

/*
 * print
 */
void print_graph_ct(const graph_t *al, graph_neimode_t mode, FILE *f);
void print_graph_vectors(const graph_t *graph, FILE *f);
#endif
