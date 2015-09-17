#ifndef GRAPH_DATATYPE_H
#define GRAPH_DATATYPE_H

#include "vector_int.h"

/*
 * the i-th edge is from[oi[i]]->to[ii[i]];
 * the first edge originating from vertex v is from[oi[os[v]]]->to[ii[os[v]]];
 * the first edge target to vertex v is from[oi[is[v]]]->to[ii[is[v]]];
 *
 * for undirected graph, an undirected edge is stored only once, and for checking
 * whether there is an undirected edge from v1 to v2 on should search for both
 * from=v1 to=v2 and from=v2 to=v1
 */
typedef struct graph_t
{
	/* number of vertices */
	int n;
	/* whether the graph is directed */
	int directed;
	/* the first column of the edge list */
	vector_int from;
	/* the second column of the edge list */
	vector_int to;
	/* the index of the edge list by the first column */
	vector_int oi;
	/* the index of the edge list by the second column*/
	vector_int ii;
	/* contains pointers to the edgelist for every vetex (outgoing based) */
	vector_int os;
	/* similar as os, but is incoming based */
	vector_int is;
} graph_t;

#endif
