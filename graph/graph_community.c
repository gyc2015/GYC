#include "graph_community.h"
#include "graph_iterator.h"
#include "graph_centrality.h"
#include <assert.h>

#define T cedge 
#define TEMPLATE_TYPE(a) a ##_cedge
#define TEMPLATE_FUNCTION(a,b) a ##_cedge_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T cvertex 
#define TEMPLATE_TYPE(a) a ##_cvertex
#define TEMPLATE_FUNCTION(a,b) a ##_cvertex_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

cedge create_cedge(int ori, int tar)
{
	cedge r;
	r.oriID = ori;
	r.tarID = tar;
	r.ne = 0;
	return r;
}

void vector_cvertex_destroy_all(vector_cvertex *v)
{
	int n = vector_cvertex_size(v);
	for (int i = 0; i < n; i++) {
		vector_int_destroy(&(VECTOR(*v)[i].ori));
		vector_int_destroy(&(VECTOR(*v)[i].tar));
	}
	vector_cvertex_destroy(v);
}

int vector_cvertex_initialize(const graph_t *graph, const vector_int *ass, vector_cvertex *v)
{
	assert(NULL != v);
	assert(NULL != ass);

	int n = vector_int_max(ass) + 1;
	int vc = graph_vertices_count(graph);

	vector_cvertex_init(v, n);
	for (int i = 0; i < n; i++) {
		VECTOR(*v)[i].ID = i;
		VECTOR(*v)[i].size = 0;
		VECTOR(*v)[i].indeg = 0;
		VECTOR(*v)[i].outdeg = 0;
		VECTOR(*v)[i].nori = 0;
		VECTOR(*v)[i].ntar = 0;
		vector_int_init(&(VECTOR(*v)[i].ori),0);
		vector_int_init(&(VECTOR(*v)[i].tar),0);
	}

	/* update community vertices size, degree */
	vector_int indeg;
	vector_int outdeg;
	vector_int_init(&indeg, 0);
	vector_int_init(&outdeg, 0);
	graph_degree(graph, &indeg, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	graph_degree(graph, &outdeg, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	int ci;
	for (int i = 0; i < vc; i++) {
		ci = VECTOR(*ass)[i];
		VECTOR(*v)[ci].size += 1;
		VECTOR(*v)[ci].indeg += VECTOR(indeg)[i];
		VECTOR(*v)[ci].outdeg += VECTOR(outdeg)[i];
	}
	vector_int_destroy(&indeg);
	vector_int_destroy(&outdeg);

	return 0;
}

/*
 * scan_cedge_ori - scan the edges originating from @u to find the the edge targeting to @v
 * if found return the address of the cedge otherwise return 0.
 */
int scan_cedge_ori(const cvertex *u, const vector_cedge *c, int vid)
{
	int n = vector_int_size(&(u->ori));

	int cid;
	for (int i = 0; i < n; i++) {
		cid = VECTOR(u->ori)[i];
		if (vid == VECTOR(*c)[cid].tarID)
			return cid;
	}
	return -1;
}

/*
 * scan_cedge_tar - scan the edges targeting to @u to find the the edge originating from @v
 * if found return the address of the cedge otherwise return 0.
 */
int scan_cedge_tar(const cvertex *u, const vector_cedge *c, int vid)
{
	int n = vector_int_size(&(u->tar));

	int cid;
	for (int i = 0; i < n; i++) {
		cid = VECTOR(u->tar)[i];
		if (vid == VECTOR(*c)[cid].oriID)
			return cid;
	}
	return -1;
}

/*
 * graph_build_communities
 *
 * @graph: the graph
 * @ass: the assignment of each vertex
 * @c: the community edge connections - NOT initialized
 * @v: the communities - NOT initialized
 */
void graph_build_communities(const graph_t *graph, const vector_int *ass, vector_cedge *c, vector_cvertex *v)
{
	vector_cvertex_initialize(graph, ass, v);
	vector_cedge_init(c, 0);

	graph_eit_t edgeit;
	graph_eit_create(graph, graph_ess_all(GRAPH_EDGEORDER_ID), &edgeit); 
	int i, j, ci, cj, tmp1, tmp2, pid = 0;
	for (GRAPH_EIT_RESET(edgeit); !GRAPH_EIT_END(edgeit); GRAPH_EIT_NEXT(edgeit)) {
		int eidx = GRAPH_EIT_GET(edgeit);
        /* Create the pairs themselves */
		graph_edge(graph, eidx, &i, &j); 
		ci = VECTOR(*ass)[i];
		cj = VECTOR(*ass)[j];
		/* update community vertices */
		if (ci == cj) continue;
		VECTOR(*v)[ci].nori++;
		VECTOR(*v)[cj].ntar++;
		/* update the connections */
		tmp1 = scan_cedge_ori(&(VECTOR(*v)[ci]), c, cj);
		tmp2 = scan_cedge_tar(&(VECTOR(*v)[cj]), c, ci);
		assert(tmp1 == tmp2);
		if (tmp1 == -1) {
			vector_cedge_push_back(c, create_cedge(ci,cj));
			vector_int_push_back(&(VECTOR(*v)[ci].ori),pid);
			vector_int_push_back(&(VECTOR(*v)[cj].tar),pid);
			tmp1 = pid;
			pid++;
		}
		VECTOR(*c)[tmp1].ne++;
	}
	graph_eit_destroy(&edgeit);
}

void print_vector_cedge(const vector_cedge *c, FILE *f)
{
	assert(NULL != c);
	assert(NULL != c->stor_begin);

	fprintf(f, "oriID\ttarID\teij\n");
	int n = vector_cedge_size(c);
	for (int i = 0; i < n ; i++) {
		fprintf(f, "%d\t%d\t%d\n",VECTOR(*c)[i].oriID,VECTOR(*c)[i].tarID,VECTOR(*c)[i].ne);
	}
}

void print_vector_cvertex(const vector_cvertex *v, FILE *f)
{
	assert(NULL != v);
	assert(NULL != v->stor_begin);

	fprintf(f, "ID\tsize\tindeg\toutdeg\tnori\tntar\n");
	int n = vector_cvertex_size(v);
	for (int i = 0; i < n; i++) {
		fprintf(f, "%d\t%d\t",VECTOR(*v)[i].ID,    VECTOR(*v)[i].size);
		fprintf(f, "%d\t%d\t",VECTOR(*v)[i].indeg, VECTOR(*v)[i].outdeg);
		fprintf(f, "%d\t%d\n",VECTOR(*v)[i].nori,  VECTOR(*v)[i].ntar);
	}
}

void output_community_graph(const graph_t *graph, const vector_int *ass, FILE *vfile, FILE *efile)
{
	vector_cvertex cv;
	vector_cedge ce;
	graph_build_communities(graph, ass, &ce, &cv);

	print_vector_cedge(&ce, efile);
	print_vector_cvertex(&cv,vfile);

	vector_cedge_destroy(&ce);
	vector_cvertex_destroy(&cv);
}

void print_communities(const graph_t *graph, const vector_int *ass, char *vfilename, char *efilename)
{
	vector_cvertex cv;
	vector_cedge ce;
	graph_build_communities(graph, ass, &ce, &cv);

    FILE *f = fopen(efilename,"w");
	print_vector_cedge(&ce,f);
	fclose(f);

    f = fopen(vfilename,"w");
	print_vector_cvertex(&cv,f);
	fclose(f);

	vector_cedge_destroy(&ce);
	vector_cvertex_destroy(&cv);
}
