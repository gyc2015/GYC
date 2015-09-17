#ifndef GRAPH_FASTCOMMUNITY_H
#define GRAPH_FASTCOMMUNITY_H

#include "graph.h"
#include "matrix_int.h"
#include "vector_ptr.h"
#include "graph_community.h"

/* Structure storing a pair of communities along with their dQ values */
typedef struct fc_pair
{
	int ci; /* the id of the community pair's first member */
	int cj; /* the seconde member */
	double eij;
	double eji;
	double dQ;
	int flag;
} fc_pair;

/* Structure storing a community */
typedef struct fc_community
{
	int id;
	int ld;
	int rd;
	int size;
	vector_ptr neis; /* fc_pair */
	double a_in;
	double a_out;
	int flag;
} fc_community;

#define T fc_pair 
#define TEMPLATE_TYPE(a) a ##_fc_pair
#define TEMPLATE_FUNCTION(a,b) a ##_fc_pair_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T fc_community 
#define TEMPLATE_TYPE(a) a ##_fc_community
#define TEMPLATE_FUNCTION(a,b) a ##_fc_community_## b
#include "vector_tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

/*
 * [1] M.E.J. Newman. Fast Algorithm for Detecting Community Structure
 * in Networks. Phys. Rev. E69(6)(2004)066133.
 * [2] A. Clauset, M.E.J. Newman, C.Moore. Finding Community Structure
 * in Very Large Networks. Phys. Rev. E70(6)(2004)066111.
 */

int graph_community_merge_communities_fastgreedy(const graph_t *graph, vector_fc_community *communities, vector_double *modularity);
void graph_community_collapse(int mid, vector_fc_community *com, vector_int *ass, int mmid);
void graph_community_reset_vertices(int vc, vector_fc_community *com);
double graph_community_fastgreedy(const graph_t *graph, vector_int *ass, int *ncom);
#endif
