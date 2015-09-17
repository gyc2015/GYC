#ifndef GRAPH_TOPOLOGICAL_PROPERTIES_H
#define GRAPH_TOPOLOGICAL_PROPERTIES_H

#include "graph.h"
#include "graph_iterator.h"
#include "graph_adjacentlist.h"

/*
 * connections
 */
int graph_incident(const graph_t *graph, vector_int *eids, int vid, graph_neimode_t mode);
int graph_neighbors(const graph_t *graph, vector_int *neis, int vid, graph_neimode_t mode);

/*
 * degree
 */
int graph_degree_distribution(graph_t *graph, vector_double *degdis, graph_vs_t vids, graph_neimode_t mode, graph_i_loops_t selfloops);
int graph_degree_minmax_avesum(graph_t *graph, int *min, int *max, double *ave, int *sum, graph_neimode_t mode, graph_i_loops_t selfloops);
int graph_relation_in_out_degree(const graph_t *graph, vector_double *inout, vector_double *outin, graph_vs_t vids, graph_i_loops_t selfloops);
int graph_relation_out_dev_in_degree(const graph_t *graph, vector_double *indev, vector_double *outdev, graph_vs_t vids, graph_i_loops_t selfloops);

/*
 * reciprocity parameter
 * L.E.C. da Rocha. Structural evolution of the Brazilian airport network.
 * J. Stat. Mech. THeory Exp., Vol. 2009, no. 04,2009.
 */
double graph_reciprocal_adjlist(const graph_adjlist_t *graph);
double graph_reciprocal(const graph_t *graph);

/*
 * clustering coefficient
 */
double graph_ave_clustering_directed(const graph_t *graph, graph_vs_t vids, graph_triangle_mode mode);
int graph_ave_clustering_directed_degree(const graph_t *graph, vector_double *cludis, graph_vs_t vids,
		graph_neimode_t nmode, graph_triangle_mode tmode, graph_i_loops_t selfloops);

/*
 * global_efficiency & local_efficiency
 *
 * [1] Latora V., Marchiori M.. Efficient Behavior of Small-World Networks.
 *	   Physical Review Letters, 2001.
 * [2] Crucitti P., Latora V., Marchiori M., Rapisarda A.. Efficiency of Scale-free Networks:
 *     error and attack tolerance. Physica A, 2003.
 */
double graph_global_efficiency(const graph_t *graph, graph_neimode_t mode);
double graph_local_efficiency(const graph_t *graph, graph_neimode_t mode);

#endif
