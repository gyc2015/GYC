#ifndef GRAPH_CENTRALITY_H
#define GRAPH_CENTRALITY_H

#include "graph.h"
#include "graph_iterator.h"
#include "queue_int.h"
#include "stack_int.h"

int graph_degree(const graph_t *graph, vector_int *deglist, graph_vs_t vids, graph_neimode_t mode, graph_i_loops_t selfloops);
int graph_v_degree(const graph_t *graph, int vid, graph_neimode_t mode, graph_i_loops_t selfloops);

int graph_v_triangle_circle(const graph_t *graph, int vid);
int graph_v_triangle_middleman(const graph_t *graph, int vid);
int graph_v_triangle_in(const graph_t *graph, int vid);
int graph_v_triangle_out(const graph_t *graph, int vid);
int graph_v_triangle_all(const graph_t *graph, int vid);
int graph_triangle(const graph_t *graph, vector_int *trilist, graph_vs_t vids, graph_triangle_mode mode);

double graph_v_clustering_circle(const graph_t *graph, int vid);
double graph_v_clustering_middleman(const graph_t *graph, int vid);
double graph_v_clustering_in(const graph_t *graph, int vid);
double graph_v_clustering_out(const graph_t *graph, int vid);
double graph_v_clustering_all(const graph_t *graph, int vid);
int graph_clustering_directed(const graph_t *graph, vector_double *clulist, graph_vs_t vids, graph_triangle_mode mode);

/*
 *  Shortest Path Vertex Betweenness
 *
 *  [1] Ulrik Brandes. A Faster Algorithm for Betweenness Centrality.
 *  Journal of Mathmatical Sociology, 25(2):163-177, 2001.
 *  [2] Ulrik Brandes. On variants of Shortest-Path Betweenness Centrality
 *  and Their Generic Computation. Social Networks, 30(2):136-145, 2008.
 */
typedef struct spvb_info
{
	/* number of shortest paths from source to v(\in)V */
	int num;
	/* distance from source */
	int dist;
	/* list of predecessors on shortest paths from source */
	vector_int pred;
	/* dependency of source on v */
	double d;
	/* number of shortest path from source to v */
	int n;
	/* shortest path vertex betweenness */
	double betw;
} spvb_info;

#define T spvb_info 
#define TEMPLATE_TYPE(a) a ##_spvb_info
#define TEMPLATE_FUNCTION(a,b) a ##_spvb_info_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

void _spvb_info_init_begin(vector_spvb_info *res);

int graph_betweenness(const graph_t *graph, vector_double *res, const graph_vs_t vids, int directed);
void graph_sssp(const graph_t *graph, vector_spvb_info *res, queue_int *pQ, stack_int *pS, int s, graph_neimode_t mode);

#endif
