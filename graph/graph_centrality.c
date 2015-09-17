#include "graph_centrality.h"
#include "graph_topological_properties.h"
#include "graph_adjacentlist.h"
#include <limits.h>
#include <assert.h>
#include <stdio.h>

int _graph_v_degree_all_out_in(const graph_t *graph, int vid, graph_neimode_t mode, graph_i_loops_t selfloops)
{
	int res = 0;

    if (GRAPH_LOOPS == selfloops) {
        if (mode & GRAPH_OUT)
            res += (VECTOR(graph->os)[vid+1] - VECTOR(graph->os)[vid]);
        if (mode & GRAPH_IN)
            res += (VECTOR(graph->is)[vid+1] - VECTOR(graph->is)[vid]);
    } else if (GRAPH_NO_LOOPS == selfloops) {
        if (mode & GRAPH_OUT) {
            res += (VECTOR(graph->os)[vid+1] - VECTOR(graph->os)[vid]);
            for (int j = VECTOR(graph->os)[vid]; j < VECTOR(graph->os)[vid+1]; j++) {
                if (VECTOR(graph->to)[VECTOR(graph->oi)[j]] == vid)
                    res -= 1;
            }
        }
        if (mode & GRAPH_IN) {
            res += (VECTOR(graph->is)[vid+1] - VECTOR(graph->is)[vid]);
            for (int j = VECTOR(graph->is)[vid]; j<VECTOR(graph->is)[vid+1]; j++) {
                if (VECTOR(graph->from)[VECTOR(graph->ii)[j]] == vid)
                    res -= 1;
            }
        }
    }  /* loops */

	return res;
}

int _graph_v_degree_bidir(const graph_t *graph, int vid)
{
	int res  = 0;

	vector_int ineis, oneis;
	vector_int_init(&ineis, 0);
	vector_int_init(&oneis, 0);
	graph_neighbors(graph, &ineis, vid, GRAPH_IN);
	graph_neighbors(graph, &oneis, vid, GRAPH_OUT);

	for (int i = 0; i < vector_int_size(&oneis); i++) {
		int v = VECTOR(oneis)[i];
		if (vector_int_contains(&ineis, v))
			res++;
	}

	vector_int_destroy(&oneis);
	vector_int_destroy(&ineis);
	return res;
}

int _graph_v_degree_all_nobidir(const graph_t *graph, int vid, graph_neimode_t mode, graph_i_loops_t selfloops)
{
	int bidir = _graph_v_degree_bidir(graph, vid);
	int aoi = _graph_v_degree_all_out_in(graph, vid, mode, selfloops);
	return aoi - bidir;
}
/*
 * graph_v_degree
 */
int graph_v_degree(const graph_t *graph, int vid, graph_neimode_t mode, graph_i_loops_t selfloops)
{
	if(mode == GRAPH_OUT || mode == GRAPH_IN || mode == GRAPH_ALL)
		return _graph_v_degree_all_out_in(graph, vid, mode, selfloops);
	else if (mode == GRAPH_BIDIR)
		return _graph_v_degree_bidir(graph, vid);
	else if (mode == GRAPH_ALL_NOBIDIR)
		return _graph_v_degree_all_nobidir(graph, vid, mode, selfloops);
	else
		assert(0);
	return 1;
}

/*
 * graph_degree
 *
 * @graph: the graph
 * @deglist: the list where result is stored into
 * @vids: giving the vertex ids of which the degree will be calculated
 * @mode: specifies the type of degree. Avaliable modes are: GRAPH_OUT, GRAPH_IN, GRAPH_ALL
 * @loops: specifies whether self-loop is allowed.
 */
int graph_degree(const graph_t *graph, vector_int *deglist, graph_vs_t vids, graph_neimode_t mode, graph_i_loops_t selfloops)
{
	if (!graph_is_directed(graph))
		mode = GRAPH_ALL;

    graph_vit_t vit;
	graph_vit_create(graph, vids, &vit);

	int nv = GRAPH_VIT_SIZE(vit);
	int i, j;
	vector_int_resize(deglist, nv);
	vector_int_null(deglist);

    for (GRAPH_VIT_RESET(vit), i=0; !GRAPH_VIT_END(vit); GRAPH_VIT_NEXT(vit), i++) {
        int vid = GRAPH_VIT_GET(vit);
		VECTOR(*deglist)[i] = graph_v_degree(graph, vid, mode, selfloops);
	}

    graph_vit_destroy(&vit);
	return 0;
}

/*
 *  Shortest Path Vertex Betweenness
 *
 *  [1] Ulrik Brandes. A Faster Algorithm for Betweenness Centrality.
 *  Journal of Mathmatical Sociology, 25(2):163-177, 2001.
 *  [2] Ulrik Brandes. On variants of Shortest-Path Betweenness Centrality
 *  and Their Generic Computation. Social Networks, 30(2):136-145, 2008.
 */
#define T spvb_info 
#define TEMPLATE_TYPE(a) a ##_spvb_info
#define TEMPLATE_FUNCTION(a,b) a ##_spvb_info_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

/*
 * .dist: distance from source
 * .pred: the list of predecessors on shortest paths from source
 * .n: the number of shortest paths from source to v
 * .d: dependency of source on v
 * .betw: the betweenness of v
 */

void _spvb_info_init_begin(vector_spvb_info *res)
{
	assert(0 != res);
	assert(0 != res->stor_begin);

	int n = vector_spvb_info_size(res);
	for (int i = 0; i < n; i++) {
		VECTOR(*res)[i].dist = INT_MAX;
		VECTOR(*res)[i].num = 0;
		VECTOR(*res)[i].n = 0;
		VECTOR(*res)[i].d = 0;
		VECTOR(*res)[i].betw = 0;
		vector_int_init(&(VECTOR(*res)[i].pred), 0);
	}
}

/*
 * _spvb_info_init_source - initialize for single source shortest paths problem
 *
 * @res: the vector
 * @s: the source
 */
void _spvb_info_init_source(vector_spvb_info *res, int s)
{
	int n = vector_spvb_info_size(res);
	for (int i = 0; i < n; i++) {
		vector_int_clear(&(VECTOR(*res)[i].pred));
		VECTOR(*res)[i].dist = INT_MAX;
		VECTOR(*res)[i].n = 0;
	}
	VECTOR(*res)[s].dist = 0;
	VECTOR(*res)[s].n = 1;
}

/*
 * _sssp - Single Source Shortest Paths
 *
 * @graph: the graph in adjacent list form
 * @res: the vector of shortest path vertex betweenness
 * @s: the index of the source vertex
 */
void _sssp(const graph_adjlist_t *graph, vector_spvb_info *res, queue_int *pQ, stack_int *pS, int s)
{
	int nv = graph->length;

	/* initialization */
	_spvb_info_init_source(res, s);
	queue_int_enq(pQ, s);

	while (!queue_int_is_empty(pQ)) {
		int v = queue_int_deq(pQ);
		stack_int_push(pS, v);

		vector_int *suc = graph_adjlist_get(graph,v);
		for (int i = 0; i < vector_int_size(suc); i++) {
			int w = VECTOR(*suc)[i];
			/* path discovery */
			if (VECTOR(*res)[w].dist == INT_MAX) {
				VECTOR(*res)[w].dist = VECTOR(*res)[v].dist + 1;
				queue_int_enq(pQ, w);
			}
			/* path counting */
			if (VECTOR(*res)[w].dist == VECTOR(*res)[v].dist + 1) {
				VECTOR(*res)[w].n = VECTOR(*res)[w].n + VECTOR(*res)[v].n;
				vector_int_push_back(&(VECTOR(*res)[w].pred), v);
			}
		}

	}

}

/*
 * _accumulate_betweenness - back-propagate dependency to accumulate the betweenness
 */
void _accumulate_betweenness(stack_int *pS, vector_spvb_info *res, int s)
{
	int n = vector_spvb_info_size(res);
	double tmp;

	for (int v = 0; v < n; v++)
		VECTOR(*res)[v].d = 0;

	while (!stack_int_is_empty(pS)) {
		int w = stack_int_pop(pS);
		vector_int *pred = &(VECTOR(*res)[w].pred);
		for (int i = 0; i < vector_int_size(pred); i++) {
			int v = VECTOR(*pred)[i];
			tmp = (double)VECTOR(*res)[v].n / (double)VECTOR(*res)[w].n * (1 + VECTOR(*res)[w].d);
			VECTOR(*res)[v].d += tmp;
		}
		if (w != s)
			VECTOR(*res)[w].betw = VECTOR(*res)[w].betw + VECTOR(*res)[w].d;
	}
}

/*
 * graph_betweenness_adjlist - calculate the vertex betweenness of the given graph in adjacent list form
 *
 * @graph: the graph in adjacent list form
 * @res: the vector of Shortest Path Vertex Betweenness
 */
int graph_betweenness_adjlist(const graph_adjlist_t *graph, vector_spvb_info *res)
{
	int nv = graph->length;

	vector_spvb_info_resize(res, nv);
	_spvb_info_init_begin(res);

	queue_int Q;
	queue_int_init(&Q, 100);
	stack_int S;
	stack_int_init(&S, nv);

	for (int s = 0; s < nv; s++) {
		_sssp(graph, res, &Q, &S, s);
		_accumulate_betweenness(&S, res, s);
	}
	queue_int_destroy(&Q);
	stack_int_destroy(&S);

	return 0;
}

/*
 * graph_betweenness
 *
 * @graph: the graph
 * @res: the result of the computation
 * @directed
 */
int graph_betweenness(const graph_t *graph, vector_double *res, const graph_vs_t vids, int directed)
{
	directed = directed && graph_is_directed(graph);
	graph_adjlist_t adjlist;
	if (directed)
		graph_adjlist_init(graph, &adjlist, GRAPH_OUT);
	else
		graph_adjlist_init(graph, &adjlist, GRAPH_ALL);

	vector_spvb_info tmpres;
	vector_spvb_info_init(&tmpres, 0);
	graph_betweenness_adjlist(&adjlist, &tmpres);

	int k;
	graph_vit_t vit;
	graph_vit_create(graph, vids, &vit);
	vector_double_resize(res, GRAPH_VIT_SIZE(vit));
	for (k = 0, GRAPH_VIT_RESET(vit); !GRAPH_VIT_END(vit); GRAPH_VIT_NEXT(vit), k++) {
		int v = GRAPH_VIT_GET(vit);
		VECTOR(*res)[v] = VECTOR(tmpres)[v].betw;
	}

	graph_vit_destroy(&vit);
	for (int i = 0; i < vector_spvb_info_size(&tmpres); i++)
		vector_int_destroy(&(VECTOR(tmpres)[i].pred));
	vector_spvb_info_destroy(&tmpres);
	graph_adjlist_destroy(&adjlist);
	return 0;
}


/*
 * cal_sssp - Calculate Single Source Shortest Paths
 *
 * @graph: the graph
 * @res: the vector of shortest path vertex betweenness
 * @s: the index of the source vertex
 */
void graph_sssp(const graph_t *graph, vector_spvb_info *res, queue_int *pQ, stack_int *pS, int s, graph_neimode_t mode)
{
	int nv = graph_vertices_count(graph);
	vector_int suc;
	vector_int_init(&suc, 0);

	/* initialization */
	_spvb_info_init_source(res, s);
	queue_int_enq(pQ, s);

	while (!queue_int_is_empty(pQ)) {
		int v = queue_int_deq(pQ);
		stack_int_push(pS, v);

		graph_neighbors(graph, &suc, v, mode);
		for (int i = 0; i < vector_int_size(&suc); i++) {
			int w = VECTOR(suc)[i];
			/* path discovery */
			if (VECTOR(*res)[w].dist == INT_MAX) {
				VECTOR(*res)[w].dist = VECTOR(*res)[v].dist + 1;
				queue_int_enq(pQ, w);
			}
			/* path counting */
			if (VECTOR(*res)[w].dist == VECTOR(*res)[v].dist + 1) {
				VECTOR(*res)[w].n = VECTOR(*res)[w].n + VECTOR(*res)[v].n;
				vector_int_push_back(&(VECTOR(*res)[w].pred), v);
			}
		}

	}

	vector_int_destroy(&suc);
}

/*
 * clustering coefficiency
 *
 * [1]. Fagiolo G.. Clustering in complex directed networks. Phys. Rev. E. 76, 026107(2007).
 */

/*
 * graph_v_triangle_circle
 *
 *  v-->w-->x-->v 
 */
int graph_v_triangle_circle(const graph_t *graph, int vid)
{
	assert(graph_is_directed(graph));

	vector_int vins, vons, wons;
	vector_int_init(&vins, 0);
	vector_int_init(&vons, 0);
	vector_int_init(&wons, 0);
	graph_neighbors(graph, &vins, vid, GRAPH_IN);
	graph_neighbors(graph, &vons, vid, GRAPH_OUT);

	int wid, xid, res = 0;
	/* v -> w */
	for (int i = 0; i < vector_int_size(&vons); i++) {
		wid = VECTOR(vons)[i];
		if (wid == vid) continue;
		graph_neighbors(graph, &wons, wid, GRAPH_OUT);
		/* w -> x */
		for (int j = 0; j < vector_int_size(&wons); j++) {
			xid = VECTOR(wons)[j];
			if (xid == vid) continue;
			if (xid == wid) continue;
			/* x -> v */
			if (vector_int_contains(&vins,xid))
				res++;
		}
	}

	vector_int_destroy(&wons);
	vector_int_destroy(&vons);
	vector_int_destroy(&vins);
	return res;
}

/*
 * graph_v_triangle_middleman
 *
 *  u -> v -> w
 *  u ------> w
 */
int graph_v_triangle_middleman(const graph_t *graph, int vid)
{
	assert(graph_is_directed(graph));

	vector_int vins, vons, uons;
	vector_int_init(&vins, 0);
	vector_int_init(&vons, 0);
	vector_int_init(&uons, 0);
	graph_neighbors(graph, &vins, vid, GRAPH_IN);
	graph_neighbors(graph, &vons, vid, GRAPH_OUT);

	int uid, wid, res = 0;
	/* v -> w */
	for (int i = 0; i < vector_int_size(&vons); i++) {
		wid = VECTOR(vons)[i];
		if (wid == vid) continue;
		/* u -> v */
		for (int j = 0; j < vector_int_size(&vins); j++) {
			uid = VECTOR(vins)[j];
			if (uid == vid) continue;
			if (uid == wid) continue;
			graph_neighbors(graph, &uons, uid, GRAPH_OUT);
			/* u -> w */
			if (vector_int_contains(&uons,wid))
				res++;
		}
	}

	vector_int_destroy(&uons);
	vector_int_destroy(&vons);
	vector_int_destroy(&vins);
	return res;
}

/*
 * graph_v_triangle_in
 *
 *  t -> u -> v
 *  t ------> v
 */
int graph_v_triangle_in(const graph_t *graph, int vid)
{
	assert(graph_is_directed(graph));

	vector_int vins, uins;
	vector_int_init(&vins, 0);
	vector_int_init(&uins, 0);
	graph_neighbors(graph, &vins, vid, GRAPH_IN);

	int tid, uid, res = 0;
	/* u -> v */
	for (int i = 0; i < vector_int_size(&vins); i++) {
		uid = VECTOR(vins)[i];
		if (uid == vid) continue;
		/* t -> v */
		for (int j = 0; j < vector_int_size(&vins); j++) {
			tid = VECTOR(vins)[j];
			if (tid == vid) continue;
			if (tid == uid) continue;
			graph_neighbors(graph, &uins, uid, GRAPH_IN);
			/* t -> u */
			if (vector_int_contains(&uins, tid))
				res++;
		}
	}
	
	vector_int_destroy(&uins);
	vector_int_destroy(&vins);
	return res;
}

/*
 * graph_v_triangle_out
 *
 *  v -> w -> x
 *  v ------> x
 */
int graph_v_triangle_out(const graph_t *graph, int vid)
{
	assert(graph_is_directed(graph));

	vector_int vons, wons;
	vector_int_init(&vons, 0);
	vector_int_init(&wons, 0);
	graph_neighbors(graph, &vons, vid, GRAPH_OUT);

	int wid, xid, res = 0;
	/* v -> w */
	for (int i = 0; i < vector_int_size(&vons); i++) {
		wid = VECTOR(vons)[i];
		if (wid == vid) continue;
		/* v -> x */
		for (int j = 0; j < vector_int_size(&vons); j++) {
			xid = VECTOR(vons)[j];
			if (xid == vid) continue;
			if (xid == wid) continue;
			graph_neighbors(graph, &wons, wid, GRAPH_OUT);
			/* w -> x*/
			if (vector_int_contains(&wons, xid))
				res++;
		}
	}

	vector_int_destroy(&wons);
	vector_int_destroy(&vons);
	return res;
}

int graph_v_triangle_all(const graph_t *graph, int vid)
{
	int res = graph_v_triangle_circle(graph, vid);	
	res += graph_v_triangle_middleman(graph, vid);
	res += graph_v_triangle_in(graph, vid);
	res += graph_v_triangle_out(graph,vid);
	return res;
}

/*
 * graph_triangle
 *
 * @graph: the graph
 * @trilist: the list where result is stored into
 * @vids: giving the vertex ids of which the degree will be calculated
 */
int graph_triangle(const graph_t *graph, vector_int *trilist, graph_vs_t vids, graph_triangle_mode mode)
{
	assert(graph_is_directed(graph));

    graph_vit_t vit;
	graph_vit_create(graph, vids, &vit);

	int nv = GRAPH_VIT_SIZE(vit);
	int i, j;
	vector_int_resize(trilist, nv);
	vector_int_null(trilist);

    for (GRAPH_VIT_RESET(vit), i=0; !GRAPH_VIT_END(vit); GRAPH_VIT_NEXT(vit), i++) {
        int vid = GRAPH_VIT_GET(vit);
		switch (mode) {
		case GRAPH_TRIANGLE_CIRCLE:
			VECTOR(*trilist)[i] = graph_v_triangle_circle(graph, vid);
			break;
		case GRAPH_TRIANGLE_MIDDLEMAN:
			VECTOR(*trilist)[i] = graph_v_triangle_middleman(graph, vid);
			break;
		case GRAPH_TRIANGLE_IN:
			VECTOR(*trilist)[i] = graph_v_triangle_in(graph, vid);
			break;
		case GRAPH_TRIANGLE_OUT:
			VECTOR(*trilist)[i] = graph_v_triangle_out(graph, vid);
			break;
		case GRAPH_TRIANGLE_ALL:
			VECTOR(*trilist)[i] = graph_v_triangle_all(graph, vid);
			break;
		default:
			assert(0);
			break;
		}
	}

    graph_vit_destroy(&vit);
	return 0;
}

/*
 * graph_v_clustering_circle
 */
double graph_v_clustering_circle(const graph_t *graph, int vid)
{
	int ideg = graph_v_degree(graph, vid, GRAPH_IN, GRAPH_NO_LOOPS);
	int odeg = graph_v_degree(graph, vid, GRAPH_OUT, GRAPH_NO_LOOPS);
	int bdeg = graph_v_degree(graph, vid, GRAPH_BIDIR, GRAPH_NO_LOOPS);
	if (ideg < 1 || odeg < 1 || ideg + odeg - bdeg < 2)
		return 0;

	double Tran = 1.0 * ideg * odeg - bdeg;
	int tran = graph_v_triangle_circle(graph, vid);
	return 1.0 * tran / Tran;
}

/*
 * graph_v_clustering_middleman
 */
double graph_v_clustering_middleman(const graph_t *graph, int vid)
{
	int ideg = graph_v_degree(graph, vid, GRAPH_IN, GRAPH_NO_LOOPS);
	int odeg = graph_v_degree(graph, vid, GRAPH_OUT, GRAPH_NO_LOOPS);
	int bdeg = graph_v_degree(graph, vid, GRAPH_BIDIR, GRAPH_NO_LOOPS);
	if (ideg < 1 || odeg < 1 || ideg + odeg - bdeg < 2)
		return 0;

	double Tran = 1.0 * ideg * odeg - bdeg;
	int tran = graph_v_triangle_middleman(graph, vid);
	return 1.0 * tran / Tran;
}

/*
 * graph_v_clustering_in
 */
double graph_v_clustering_in(const graph_t *graph, int vid)
{
	int ideg = graph_v_degree(graph, vid, GRAPH_IN, GRAPH_NO_LOOPS);
	if (ideg < 2)
		return 0;

	double Tran = 1.0 * ideg * (ideg - 1.0);
	int tran = graph_v_triangle_in(graph, vid);
	return 1.0 * tran / Tran;
}

/*
 * graph_v_clustering_out
 */
double graph_v_clustering_out(const graph_t *graph, int vid)
{
	int odeg = graph_v_degree(graph, vid, GRAPH_OUT, GRAPH_NO_LOOPS);
	if (odeg < 2)
		return 0;

	double Tran = 1.0 * odeg * (odeg - 1.0);
	int tran = graph_v_triangle_out(graph, vid);
	return 1.0 * tran / Tran;
}

/*
 * graph_v_clustering_all
 */
double graph_v_clustering_all(const graph_t *graph, int vid)
{
	int tdeg = graph_v_degree(graph, vid, GRAPH_ALL, GRAPH_NO_LOOPS);
	int bdeg = graph_v_degree(graph, vid, GRAPH_BIDIR, GRAPH_NO_LOOPS);
	if (tdeg - bdeg < 2)
		return 0;

	double Tran = 1.0 * tdeg * (tdeg - 1.0) - 2.0 * bdeg;
	int tran = graph_v_triangle_circle(graph, vid)
		     + graph_v_triangle_middleman(graph, vid)
			 + graph_v_triangle_in(graph, vid)
			 + graph_v_triangle_out(graph, vid);
	return 1.0 * tran /Tran;
}

/*
 * graph_clustering_all
 *
 * @graph: the graph
 * @clulist: the list where result is stored into
 * @vids: giving the vertex ids of which the degree will be calculated
 * @mode:
 */
int graph_clustering_directed(const graph_t *graph, vector_double *clulist, graph_vs_t vids, graph_triangle_mode mode)
{
	assert(graph_is_directed(graph));
	assert(mode == GRAPH_TRIANGLE_CIRCLE || mode == GRAPH_TRIANGLE_MIDDLEMAN
		|| mode == GRAPH_TRIANGLE_IN || mode == GRAPH_TRIANGLE_OUT || mode == GRAPH_TRIANGLE_ALL);

    graph_vit_t vit;
	graph_vit_create(graph, vids, &vit);

	int nv = GRAPH_VIT_SIZE(vit);
	int i, j;
	vector_double_resize(clulist, nv);
	vector_double_null(clulist);

    for (GRAPH_VIT_RESET(vit), i=0; !GRAPH_VIT_END(vit); GRAPH_VIT_NEXT(vit), i++) {
        int vid = GRAPH_VIT_GET(vit);
		switch (mode) {
		case GRAPH_TRIANGLE_CIRCLE:
			VECTOR(*clulist)[i] = graph_v_clustering_circle(graph, vid);
			break;
		case GRAPH_TRIANGLE_MIDDLEMAN:
			VECTOR(*clulist)[i] = graph_v_clustering_middleman(graph, vid);
			break;
		case GRAPH_TRIANGLE_IN:
			VECTOR(*clulist)[i] = graph_v_clustering_in(graph, vid);
			break;
		case GRAPH_TRIANGLE_OUT:
			VECTOR(*clulist)[i] = graph_v_clustering_out(graph, vid);
			break;
		case GRAPH_TRIANGLE_ALL:
			VECTOR(*clulist)[i] = graph_v_clustering_all(graph, vid);
			break;
		default:
			assert(0);
			break;
		}
	}

    graph_vit_destroy(&vit);
	return 0;
}
