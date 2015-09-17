#include "graph_topological_properties.h"
#include "graph_centrality.h"
#include <limits.h>
#include <assert.h>
#include <stdio.h>

/*
 * graph_neighbors_ignore_direction - gives the neighbors of a vertex
 *
 * @graph: the graph
 * @neis: the neighbors
 * @vid: the vertex id
 * @mode: specifies which kind of edges to include for directed graphs.
 *        the available modes are: graph_out, graph_in, graph_all
 *        this parameter is ignored for undirected graphs.
 */
int graph_neighbors_ignore_direction(const graph_t *graph, vector_int *neis, int vid, graph_neimode_t mode)
{
	assert(vid >= 0);
	assert(vid < graph_vertices_count(graph));
	assert(mode == GRAPH_OUT || mode == GRAPH_IN || mode == GRAPH_ALL);

	int length = 0;
	if (mode & GRAPH_OUT)
		length += (VECTOR(graph->os)[vid+1] - VECTOR(graph->os)[vid]);
	if (mode & GRAPH_IN)
		length += (VECTOR(graph->is)[vid+1] - VECTOR(graph->is)[vid]);
	vector_int_resize(neis, length);

	int idx = 0;
	if (!graph_is_directed(graph) || mode != GRAPH_ALL) {
		if (mode & GRAPH_OUT) {
			int j = VECTOR(graph->os)[vid+1];
			for (int i = VECTOR(graph->os)[vid]; i < j; i++)
				VECTOR(*neis)[idx++] = VECTOR(graph->to)[VECTOR(graph->oi)[i]];
		}
		if (mode & GRAPH_IN) {
			int j = VECTOR(graph->is)[vid+1];
			for (int i = VECTOR(graph->is)[vid]; i < j; i++)
				VECTOR(*neis)[idx++] = VECTOR(graph->from)[VECTOR(graph->ii)[i]];
		}
	} else {
		int j1 = VECTOR(graph->os)[vid+1];
		int i1 = VECTOR(graph->os)[vid];
		int j2 = VECTOR(graph->is)[vid+1];
		int i2 = VECTOR(graph->is)[vid];
		while (i1 < j1 && i2 < j2) {
			int n1 = VECTOR(graph->to)[VECTOR(graph->oi)[i1]];
			int n2 = VECTOR(graph->from)[VECTOR(graph->ii)[i2]];
			if (n1 < n2) {
				VECTOR(*neis)[idx++] = n1;
				i1++;
			} else if (n2 < n1) {
				VECTOR(*neis)[idx++] = n2;
				i2++;
			} else {
				VECTOR(*neis)[idx++] = n1;
				VECTOR(*neis)[idx++] = n2;
				i1++; i2++;
			}
		}
		while (i1 < j1) {
			int n1 = VECTOR(graph->to)[VECTOR(graph->oi)[i1]];
			VECTOR(*neis)[idx++] = n1;
			i1++;
		}
		while (i2 < j2) {
			int n2 = VECTOR(graph->from)[VECTOR(graph->ii)[i2]];
			VECTOR(*neis)[idx++] = n2;
			i2++;
		}
	}

	return 0;
}

/*
 * graph_neighbors_direction - gives the neighbors of a vertex
 *
 * @graph: the graph
 * @neis: the neighbors
 * @vid: the vertex id
 * @mode: specifies which kind of edges to include for directed graphs.
 *        the available modes are: graph_all_nobidir
 *        this parameter is ignored for undirected graphs.
 */
int graph_neighbors_direction(const graph_t *graph, vector_int *neis, int vid)
{
	assert(vid >= 0);
	assert(vid < graph_vertices_count(graph));

	int length = graph_v_degree(graph, vid, GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS); 
	vector_int_resize(neis, length);

	int idx = 0;
	int tmp = -1;
	int j1 = VECTOR(graph->os)[vid+1];
	int i1 = VECTOR(graph->os)[vid];
	int j2 = VECTOR(graph->is)[vid+1];
	int i2 = VECTOR(graph->is)[vid];
	while (i1 < j1 && i2 < j2) {
		int n1 = VECTOR(graph->to)[VECTOR(graph->oi)[i1]];
		int n2 = VECTOR(graph->from)[VECTOR(graph->ii)[i2]];
		if (n1 < n2) {
			if (n1 != tmp) { tmp = n1; VECTOR(*neis)[idx++] = n1; }
			i1++;
		} else if (n2 < n1) {
			if (n2 != tmp) { tmp = n2; VECTOR(*neis)[idx++] = n2; }
			i2++;
		} else {
			if (n1 != tmp) { tmp = n1; 	VECTOR(*neis)[idx++] = n1; }
			i1++; i2++;
		}
	}
	while (i1 < j1) {
		int n1 = VECTOR(graph->to)[VECTOR(graph->oi)[i1]];
		if (n1 != tmp) { tmp = n1; VECTOR(*neis)[idx++] = n1; }
		i1++;
	}
	while (i2 < j2) {
		int n2 = VECTOR(graph->from)[VECTOR(graph->ii)[i2]];
		if (n2 != tmp) { tmp = n2; VECTOR(*neis)[idx++] = n2; }
		i2++;
	}

	return 0;
}

/*
 * graph_neighbors - gives the neighbors of a vertex
 *
 * @graph: the graph
 * @neis: the neighbors
 * @vid: the vertex id
 * @mode: specifies which kind of edges to include for directed graphs.
 *        the available modes are: graph_out, graph_in, graph_all, graph_all_nobidir
 *        this parameter is ignored for undirected graphs.
 */
int graph_neighbors(const graph_t *graph, vector_int *neis, int vid, graph_neimode_t mode)
{
	assert(vid >= 0);
	assert(vid < graph_vertices_count(graph));

	if (!graph->directed)
		mode = GRAPH_ALL;

	if (mode == GRAPH_OUT || mode == GRAPH_IN || mode == GRAPH_ALL)
		return graph_neighbors_ignore_direction(graph, neis, vid, mode);
	else if (mode == GRAPH_ALL_NOBIDIR)
		return graph_neighbors_direction(graph, neis, vid);
	else
		assert(0);
	return 1;
}


/*
 * graph_incident - gives the incident edges of a vertex
 *
 * @graph: the graph
 * @eids: an initialized vector_int. It will be resized to hold the result.
 * @vid: the vertex id
 * @mode: specifies which kind of edges to include for directed graphs.
 *        the available modes are: GRAPH_OUT, GRAPH_IN, GRAPH_ALL
 *        this parameter is ignored for undirected graphs.
 */
int graph_incident(const graph_t *graph, vector_int *eids, int vid, graph_neimode_t mode)
{
	assert(vid >= 0);
	assert(vid < graph_vertices_count(graph));
	assert(mode == GRAPH_OUT || mode == GRAPH_IN || mode == GRAPH_ALL);

	if (!graph->directed)
		mode = GRAPH_ALL;

	int length = 0;
	if (mode & GRAPH_OUT)
		length += (VECTOR(graph->os)[vid+1] - VECTOR(graph->os)[vid]);
	if (mode & GRAPH_IN)
		length += (VECTOR(graph->is)[vid+1] - VECTOR(graph->is)[vid]);
	vector_int_resize(eids, length);

	int idx = 0;
	if (mode & GRAPH_OUT) {
		int j = VECTOR(graph->os)[vid+1];
		for (int i = VECTOR(graph->os)[vid]; i < j; i++)
			VECTOR(*eids)[idx++] = VECTOR(graph->oi)[i];
	}
	if (mode & GRAPH_IN) {
		int j = VECTOR(graph->is)[vid+1];
		for (int i = VECTOR(graph->is)[vid]; i < j; i++)
			VECTOR(*eids)[idx++] = VECTOR(graph->ii)[i];
	}

	return 0;
}

/*
 * graph_degree_distribution
 *
 * @graph: the graph
 * @degdis: an initialized vector the degree distribution list
 * @vids: vertex selector
 * @mode: specifies the type of degree. Avaliable modes are: GRAPH_OUT, GRAPH_IN, GRAPH_ALL
 * @loops: specifies whether self-loop is allowed.
 */
int graph_degree_distribution(graph_t *graph, vector_double *degdis, graph_vs_t vids, graph_neimode_t mode, graph_i_loops_t selfloops)
{
	vector_int deglist;
	vector_int_init(&deglist, 0);
	graph_degree(graph, &deglist, vids, mode, selfloops);

	vector_int degfreq;
	vector_int_init(&degfreq, 0);
	int min, max;
	vector_int_ele_freq_min_max(&degfreq, &deglist, &min, &max);

	int vc = vector_int_size(&deglist);
	int n = vector_int_size(&degfreq);
	vector_double_resize(degdis, n);
	for (int i = 0; i < n; i++)
		VECTOR(*degdis)[i] = (double)VECTOR(degfreq)[i] / (double)vc;

	vector_int_destroy(&deglist);
	vector_int_destroy(&degfreq);
	return 0;
}

/*
 * graph_degree_minmax_avesum
 */
int graph_degree_minmax_avesum(graph_t *graph, int *min, int *max, double *ave, int *sum, graph_neimode_t mode, graph_i_loops_t selfloops)
{
	vector_int deglist;
	vector_int_init(&deglist, 0);
	graph_degree(graph, &deglist, graph_vss_all(), mode, selfloops);

	vector_int_minmax(&deglist, min, max);
	int vc = graph_vertices_count(graph);
	*sum = vector_int_sum(&deglist);
	*ave = (double)(*sum) / (double)vc;

	return 0;
}

/*
 * reciprocity parameter
 * [1]. L.E.C. da Rocha. Structural evolution of the Brazilian airport network.
 * J. Stat. Mech. THeory Exp., Vol. 2009, no. 04,2009.
 */
double _reciprocal_a(const graph_adjlist_t *graph)
{
	int n = graph->length;
	int tmp = 0;
	assert (n > 1);

	for (int v = 0; v < n; v++) {
		vector_int *suc = graph_adjlist_get(graph,v);
		for (int j = 0; j < vector_int_size(suc); j++) {
			int w = VECTOR(*suc)[j];
			if (v != w)
				tmp++;
		}
	}
	double a = 1.0 * tmp / (1.0 * n * (n-1));
    return a;
}

double graph_reciprocal_adjlist(const graph_adjlist_t *graph)
{
    double a = _reciprocal_a(graph);
	int n = graph->length;
	double term = 0;
	double term1 = 0;

	for (int v = 0; v < n; v++) {
		vector_int *suc = graph_adjlist_get(graph,v);
		for (int j = 0; j < vector_int_size(suc); j++) {
			int w = VECTOR(*suc)[j];
			if (v != w) {
				vector_int *wsuc = graph_adjlist_get(graph, w);
				term1 += (1.0 - a) * (1.0 - a);
				if (vector_int_contains(wsuc, v))
					term += (1.0 - a) * (1.0 - a);
				else
					term += (1.0 - a) * (0.0 - a);
			}
		}
	}
	double R = term / term1;
    return R;
}

double graph_reciprocal(const graph_t *graph)
{
	graph_adjlist_t adjlist;
	graph_adjlist_init(graph, &adjlist, GRAPH_OUT);
	double r = graph_reciprocal_adjlist(&adjlist);
	graph_adjlist_destroy(&adjlist);
	return r;
}

/*
 * _accumulate_dist_np
 */
int _accumulate_dist_np(stack_int *pS, vector_spvb_info *res, int s, int *dist, int *np)
{
	int n = vector_spvb_info_size(res);

	while (!stack_int_is_empty(pS)) {
		int w = stack_int_pop(pS);
		if (w == s) continue;
		*dist += VECTOR(*res)[w].dist * VECTOR(*res)[w].n;
		*np += VECTOR(*res)[w].n;
	}
	return 0;
}

double graph_ave_shortest_path_length(const graph_t *graph, graph_neimode_t mode)
{
	int nv = graph_vertices_count(graph);
	if (0 == nv || 1 == nv)
		return 0;

	vector_spvb_info res;
	vector_spvb_info_init(&res, nv);
	_spvb_info_init_begin(&res);

	queue_int Q;
	queue_int_init(&Q, 100);
	stack_int S;
	stack_int_init(&S, nv);

	int dist = 0, np = 0;
	for (int s = 0; s < nv; s++) {
		graph_sssp(graph, &res, &Q, &S, s, mode);
		_accumulate_dist_np(&S, &res, s, &dist, &np);
	}
	double tmp = (0 == np) ? 0.0 : 1.0 * dist / np;

	queue_int_destroy(&Q);
	stack_int_destroy(&S);
	vector_spvb_info_destroy(&res);
	return tmp;
}

int _accumulate_dist(stack_int *pS, vector_spvb_info *res, int s)
{
	int n = vector_spvb_info_size(res);

	int tmp = 0;
	while (!stack_int_is_empty(pS)) {
		int w = stack_int_pop(pS);
		if (w == s) continue;
		tmp += (double)VECTOR(*res)[w].dist;
	}
	return tmp;
}

double graph_characteristics_path_length(const graph_t *graph, graph_neimode_t mode)
{
	int nv = graph_vertices_count(graph);
	if (0 == nv || 1 == nv)
		return 0;

	vector_spvb_info res;
	vector_spvb_info_init(&res, nv);
	_spvb_info_init_begin(&res);

	queue_int Q;
	queue_int_init(&Q, 100);
	stack_int S;
	stack_int_init(&S, nv);

	double tmp = 0;
	for (int s = 0; s < nv; s++) {
		graph_sssp(graph, &res, &Q, &S, s, mode);
		tmp += _accumulate_dist(&S, &res, s);
	}
	tmp = tmp / (nv * (nv -1.0));

	queue_int_destroy(&Q);
	stack_int_destroy(&S);
	vector_spvb_info_destroy(&res);
	return tmp;
}

/*
 * _accumulate_eglob
 */
double _accumulate_eglob(stack_int *pS, vector_spvb_info *res, int s)
{
	int n = vector_spvb_info_size(res);

	double tmp = 0;
	while (!stack_int_is_empty(pS)) {
		int w = stack_int_pop(pS);
		if (w == s) continue;
		tmp += 1.0 / (double)VECTOR(*res)[w].dist;
	}
	return tmp;
}

/*
 * graph_global_efficiency
 *
 * @graph: the graph
 */
double graph_global_efficiency(const graph_t *graph, graph_neimode_t mode)
{
	int nv = graph_vertices_count(graph);
	if (0 == nv || 1 == nv)
		return 0;

	vector_spvb_info res;
	vector_spvb_info_init(&res, nv);
	_spvb_info_init_begin(&res);

	queue_int Q;
	queue_int_init(&Q, 100);
	stack_int S;
	stack_int_init(&S, nv);

	double lalala = 0.0;
	double tmp = 0;
	for (int s = 0; s < nv; s++) {
		graph_sssp(graph, &res, &Q, &S, s, mode);
		tmp += _accumulate_eglob(&S, &res, s);
	}
	tmp = tmp / (nv * (nv -1.0));

	queue_int_destroy(&Q);
	stack_int_destroy(&S);
	vector_spvb_info_destroy(&res);
	return tmp;
}

/*
 * graph_local_efficiency
 */
double graph_local_efficiency(const graph_t *graph, graph_neimode_t mode)
{
	int nv = graph_vertices_count(graph);
	graph_t Gi;
	vector_int neis;
	vector_int_init(&neis,0);

	double tmp = 0;
	for (int i = 0; i < nv; i++) {
		graph_neighbor_subgraph(graph, &Gi, &neis, i, GRAPH_ALL_NOBIDIR);
		tmp += graph_global_efficiency(&Gi, mode);
	}
	tmp = tmp / nv;

	vector_int_destroy(&neis);
	graph_destroy(&Gi);
	return tmp;
}

/*
 * graph_ave_clustering
 */
double graph_ave_clustering_directed(const graph_t *graph, graph_vs_t vids, graph_triangle_mode mode)
{
	int vc = graph_vertices_count(graph);
	if (vc < 3) return 0.0;

	vector_double clus;
	vector_double_init(&clus, 0);
	graph_clustering_directed(graph, &clus, vids, mode);

	double tmp = 0;
	for (int i = 0; i < vc; i++)
		tmp += VECTOR(clus)[i];

	tmp = tmp / vc;
	return tmp;
}

int graph_ave_clustering_directed_degree(const graph_t *graph, vector_double *cludis, graph_vs_t vids,
		graph_neimode_t nmode, graph_triangle_mode tmode, graph_i_loops_t selfloops)
{
	int vc = graph_vertices_count(graph);
	if (vc < 3) return 1;

	vector_double clus;
	vector_double_init(&clus, 0);
	graph_clustering_directed(graph, &clus, vids, tmode);

	int mindeg, maxdeg;
	vector_int deglist, degdist;
	vector_int_init(&deglist, 0);
	graph_degree(graph, &deglist, vids, nmode, selfloops);
	vector_int_minmax(&deglist, &mindeg, &maxdeg);

	vector_int_init(&degdist, maxdeg - mindeg + 1);
	vector_int_fill(&degdist, 0);
	vector_double_resize(cludis, maxdeg - mindeg + 1);
	vector_double_fill(cludis, 0.0);

	for (int i = 0; i < vc; i++) {
		int tmpdeg = VECTOR(deglist)[i] - mindeg;
		VECTOR(*cludis)[tmpdeg] += VECTOR(clus)[i];
		VECTOR(degdist)[tmpdeg]++;
	}

	for (int i = 0; i < maxdeg - mindeg + 1; i++)
		VECTOR(*cludis)[i] = (0 == VECTOR(degdist)[i]) ? -1 : VECTOR(*cludis)[i] / VECTOR(degdist)[i];

	vector_int_destroy(&degdist);
	vector_int_destroy(&deglist);
	vector_double_destroy(&clus);
	return 0;
}

int graph_relation_out_dev_in_degree(const graph_t *graph, vector_double *indev, vector_double *outdev, graph_vs_t vids, graph_i_loops_t selfloops)
{
	int vc = graph_vertices_count(graph);
	int inmindeg, inmaxdeg, outmindeg, outmaxdeg;
	vector_int indeg, outdeg;
	vector_int_init(&indeg, 0);
	vector_int_init(&outdeg, 0);
	vector_double degdist;

	graph_degree(graph, &indeg, vids, GRAPH_IN, selfloops);
	graph_degree(graph, &outdeg, vids, GRAPH_OUT, selfloops);
	vector_int_minmax(&indeg, &inmindeg, &inmaxdeg);
	vector_int_minmax(&outdeg, &outmindeg, &outmaxdeg);

	vector_double devdeg;
	vector_double_init(&devdeg, vc);
	for (int i = 0; i < vc; i++)
		VECTOR(devdeg)[i] = 1.0 * VECTOR(outdeg)[i] / (1.0 + VECTOR(indeg)[i]);

	vector_double_init(&degdist, inmaxdeg - inmindeg + 1);
	vector_double_fill(&degdist, 0);
	vector_double_resize(indev, inmaxdeg - inmindeg + 1);
	vector_double_fill(indev, 0.0);
	for (int i = 0; i < vc; i++) {
		int tmpdeg = VECTOR(indeg)[i] - inmindeg;
		VECTOR(*indev)[tmpdeg] += VECTOR(devdeg)[i];
		VECTOR(degdist)[tmpdeg]++;
	}
	for (int i = 0; i < inmaxdeg - inmindeg + 1; i++)
		VECTOR(*indev)[i] = (0 == VECTOR(degdist)[i]) ? -1 : VECTOR(*indev)[i] / VECTOR(degdist)[i];


	vector_double_resize(&degdist, outmaxdeg - outmindeg + 1);
	vector_double_fill(&degdist, 0);
	vector_double_resize(outdev, outmaxdeg - outmindeg + 1);
	vector_double_fill(outdev, 0.0);
	for (int i = 0; i < vc; i++) {
		int tmpdeg = VECTOR(outdeg)[i] - outmindeg;
		VECTOR(*outdev)[tmpdeg] += VECTOR(devdeg)[i];
		VECTOR(degdist)[tmpdeg]++;
	}
	for (int i = 0; i < outmaxdeg - outmindeg + 1; i++)
		VECTOR(*outdev)[i] = (0 == VECTOR(degdist)[i]) ? -1 : VECTOR(*outdev)[i] / VECTOR(degdist)[i];

	vector_double_destroy(&degdist);
	vector_double_destroy(&devdeg);
	vector_int_destroy(&outdeg);
	vector_int_destroy(&indeg);
}

int graph_relation_in_out_degree(const graph_t *graph, vector_double *inout, vector_double *outin, graph_vs_t vids, graph_i_loops_t selfloops)
{
	int vc = graph_vertices_count(graph);

	int inmindeg, inmaxdeg, outmindeg, outmaxdeg;
	vector_int indeg, outdeg;
	vector_int_init(&indeg, 0);
	vector_int_init(&outdeg, 0);
	vector_double degdist;

	graph_degree(graph, &indeg, vids, GRAPH_IN, selfloops);
	graph_degree(graph, &outdeg, vids, GRAPH_OUT, selfloops);
	vector_int_minmax(&indeg, &inmindeg, &inmaxdeg);
	vector_int_minmax(&outdeg, &outmindeg, &outmaxdeg);

	vector_double_init(&degdist, inmaxdeg - inmindeg + 1);
	vector_double_fill(&degdist, 0);
	vector_double_resize(inout, inmaxdeg - inmindeg + 1);
	vector_double_fill(inout, 0.0);
	for (int i = 0; i < vc; i++) {
		int tmpdeg = VECTOR(indeg)[i] - inmindeg;
		VECTOR(*inout)[tmpdeg] += VECTOR(outdeg)[i];
		VECTOR(degdist)[tmpdeg]++;
	}
	for (int i = 0; i < inmaxdeg - inmindeg + 1; i++)
		VECTOR(*inout)[i] = (0 == VECTOR(degdist)[i]) ? -1 : VECTOR(*inout)[i] / VECTOR(degdist)[i];

	vector_double_resize(&degdist, outmaxdeg - outmindeg + 1);
	vector_double_fill(&degdist, 0);
	vector_double_resize(outin, outmaxdeg - outmindeg + 1);
	vector_double_fill(outin, 0.0);
	for (int i = 0; i < vc; i++) {
		int tmpdeg = VECTOR(outdeg)[i] - outmindeg;
		VECTOR(*outin)[tmpdeg] += VECTOR(indeg)[i];
		VECTOR(degdist)[tmpdeg]++;
	}
	for (int i = 0; i < outmaxdeg - outmindeg + 1; i++)
		VECTOR(*outin)[i] = (0 == VECTOR(degdist)[i]) ? -1 : VECTOR(*outin)[i] / VECTOR(degdist)[i];

	vector_double_destroy(&degdist);
	vector_int_destroy(&outdeg);
	vector_int_destroy(&indeg);
}
