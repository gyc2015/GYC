#ifndef GRAPH_COMMUNITY_H
#define GRAPH_COMMUNITY_H

#include "graph.h"
#include "vector_int.h"

/*
 * community edge
 */
typedef struct cedge
{
	int oriID;
	int tarID;
	int ne;
} cedge;

/*
 * community vertex
 */
typedef struct cvertex
{
	int ID;
	int size;
	/* the sum of in-degrees */
	int indeg;
	/* the sum of out-degrees */
	int outdeg;
	/* the number of edges that originate from this community */
	int nori;
	/* the number of edges that target to this community */
	int ntar;
	vector_int ori; /* cedge index */
	vector_int tar;
} cvertex;

#define T cedge 
#define TEMPLATE_TYPE(a) a ##_cedge
#define TEMPLATE_FUNCTION(a,b) a ##_cedge_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T cvertex 
#define TEMPLATE_TYPE(a) a ##_cvertex
#define TEMPLATE_FUNCTION(a,b) a ##_cvertex_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T
void vector_cvertex_destroy_all(vector_cvertex *v);

void graph_build_communities(const graph_t *graph, const vector_int *ass, vector_cedge *c, vector_cvertex *v);
void print_vector_cvertex(const vector_cvertex *v, FILE *f);
void print_vector_cedge(const vector_cedge *c, FILE *f);
void print_communities(const graph_t *graph, const vector_int *ass, char *vfilename, char *efilename);
void output_community_graph(const graph_t *graph, const vector_int *ass, FILE *vfile, FILE *efile);
#endif
