#include "graph.h"
#include "graph_topological_properties.h"
#include <assert.h>

/*
 * new_graph
 *
 * @graph: an uninitialized graph.
 * @edges: the edges to add, the first two elements refer to
 *         the "from" and "to" items of the first edge respectively.
 * @n: the number of vertices.
 * @directed: specifies whether the created graph is directed.
 */
int new_graph(graph_t *graph, vector_int *edges,int n, int directed)
{
	assert(0 == (vector_int_size(edges) % 2));
	assert(0 <= vector_int_min(edges));

	int max = vector_int_max(edges);
	n = (n > max) ? n : (max + 1);
	new_graph_empty(graph, n, directed);
	if (vector_int_size(edges) > 0)
		graph_add_edges(graph, edges,0);

	return 0;
}

/*
 * new_graph_empty - return a graph with n vertices and 0 edges
 *
 * @graph: an uninitialized graph
 * @n: the number of vertices
 * @directed: specifies whether the created graph is directed.
 */
int new_graph_empty(graph_t *graph,int n, int directed)
{
	return new_graph_empty_attrs(graph, n, directed, 0);
}

/*
 * new_graph_empty_attrs - similar to new_graph_empty.
 * It also assign attributes to the graph
 *
 * @graph: an uninitialized graph
 * @n: the number of vertices
 * @directed: specifies whether the created graph is directed
 * @attr: the attributes.
 */
int new_graph_empty_attrs(graph_t *graph,int n, int directed, void *attr)
{
	assert(0 <= n);

	graph->n = 0;
	graph->directed = directed;
	vector_int_init(&(graph->from),0);
	vector_int_init(&(graph->to),  0);
	vector_int_init(&(graph->oi),  0);
	vector_int_init(&(graph->ii),  0);
	vector_int_init(&(graph->os),  1);
	vector_int_init(&(graph->is),  1);

	VECTOR(graph->os)[0] = 0;
	VECTOR(graph->is)[0] = 0;

	/* init attributes -mark-gyc-*/

	/* add the vertices */
	graph_add_vertices(graph, n, 0);

	return 0;
}

/*
 * graph_destroy
 */
int graph_destroy(graph_t *graph)
{
	vector_int_destroy(&(graph->from));
	vector_int_destroy(&(graph->to));
	vector_int_destroy(&(graph->oi));
	vector_int_destroy(&(graph->ii));
	vector_int_destroy(&(graph->os));
	vector_int_destroy(&(graph->is));
	return 0;
}

/*
 * graph_edges_count
 */
int graph_edges_count(const graph_t *graph)
{
	return vector_int_size(&(graph->from));
}

/*
 * graph_vertices_count
 */
int graph_vertices_count(const graph_t *graph)
{
	return graph->n;
}

/*
 * graph_is_directed
 */
int graph_is_directed(const graph_t *graph)
{
	return graph->directed;
}

/*
 * graph_has_multiple
 */
int graph_has_multiple(const graph_t *graph)
{
	int vc = graph_vertices_count(graph);
	int ec = graph_edges_count(graph);
	int dflag = graph_is_directed(graph);

	if (vc == 0 || ec == 0)
		return 0;

	vector_int neis;
	vector_int_init(&neis, 0);
	int found = 0, n;
	for (int i = 0; i < vc && !found; i++) {
		graph_neighbors(graph, &neis, i, GRAPH_OUT);
		n = vector_int_size(&neis);
		for (int j = 1; j < n; j++) {
			if (VECTOR(neis)[j-1] == VECTOR(neis)[j]) {
				if (dflag)
				/* Directed, so this is a real multiple edge */
					return 1;
				else if (VECTOR(neis)[j-1] != i)
				/* Undirected, but not a loop edge */
					return 1;
				else if (j < n-1 && VECTOR(neis)[j] == VECTOR(neis)[j+1])
				/* Undirected, loop edge, multiple times */
					return 1;
			}
		}
	}
	return 0;
}


int graph_edge_contains(const graph_t *graph, int v, int w, graph_neimode_t mode)
{
	assert(0 != graph);
	int vc = graph_vertices_count(graph);
	if (v >= vc || w >= vc)
		return 0;

	vector_int neis;
	vector_int_init(&neis, 0);
	graph_neighbors(graph, &neis, v, mode);
	int res = vector_int_contains(&neis, w) ? 1 : 0; 
	vector_int_destroy(&neis);

	return res;
}

/*
 * graph_add_vertices
 *
 * @graph: target
 * @nv: number of vertices
 * @attr: the attributes of the new vertices
 */
int graph_add_vertices(graph_t *graph, int nv, void *attr)
{
	assert(0 <= nv);

	vector_int_resize(&(graph->os), graph->n + nv + 1);
	vector_int_resize(&(graph->is), graph->n + nv + 1);

	int ec = graph_edges_count(graph);
	for (int i = graph->n + 1; i < graph->n + nv + 1; i++) {
		VECTOR(graph->os)[i] = ec;
		VECTOR(graph->is)[i] = ec;
	}
	graph->n += nv;

	/* attribute -mark-gyc-*/

	return 0;
}

/*
 * graph_add_edges
 */
int graph_add_edges(graph_t *graph, vector_int *edges, void *attr)
{
	assert(0 == vector_int_size(edges) % 2);
	assert(1 == vector_int_isininterval(edges, 0, graph_vertices_count(graph)-1));

	int ec = graph_edges_count(graph);
	int ec_to_add = vector_int_size(edges) / 2;

	/* from & to */
	vector_int_reserve(&(graph->from), ec + ec_to_add);
	vector_int_reserve(&(graph->to), ec + ec_to_add);
	int i = 0;
	int directed = graph->directed;
	while (i < ec_to_add * 2) {
		if (directed || VECTOR(*edges)[i] > VECTOR(*edges)[i+1]) {
			vector_int_push_back(&(graph->from), VECTOR(*edges)[i++]);
			vector_int_push_back(&(graph->to),   VECTOR(*edges)[i++]);
		} else {
			vector_int_push_back(&(graph->to),   VECTOR(*edges)[i++]);
			vector_int_push_back(&(graph->from), VECTOR(*edges)[i++]);
		}
	}

	/* oi & ii */
	vector_int newoi, newii;
	vector_int_init(&newoi, ec + ec_to_add);
	vector_int_init(&newii, ec + ec_to_add);
	vector_int_order_inc2(&(graph->from), &(graph->to), &newoi, graph->n);
	vector_int_order_inc2(&(graph->to), &(graph->from), &newii, graph->n);

	/* attributes */

	/* os & is */
	vector_int_scan_tie(&(graph->os), &(graph->from), &newoi, graph->n);
	vector_int_scan_tie(&(graph->is), &(graph->to),   &newii, graph->n);

	/* everything went fine */
	vector_int_destroy(&(graph->oi));
	vector_int_destroy(&(graph->ii));
	graph->oi = newoi;
	graph->ii = newii;

	return 0;
}

/*
 * graph_del_edges - delete the edges
 */
int graph_del_edges(graph_t *graph, graph_es_t eids)
{
	int ec = graph_edges_count(graph);
	int vc = graph_vertices_count(graph);
	/* mark the edges needed to be removed */
	int *mark = Calloc(ec, int);
	assert(0 != mark);
	graph_eit_t eit;
	graph_eit_create(graph, eids, &eit);
	int edges_to_remove = 0;
	for (GRAPH_EIT_RESET(eit); !GRAPH_EIT_END(eit); GRAPH_EIT_NEXT(eit)) {
		int e = GRAPH_EIT_GET(eit);
		if (0 == mark[e]) {
			edges_to_remove++;
			mark[e]++;
		}
	}
	int remaining_edges = ec - edges_to_remove;
	/* We don't need the iterator any more */
	graph_eit_destroy(&eit);
	/* We build some new vectors to store the remaining edges */
	vector_int newfrom, newto, newoi, newii;
	vector_int_init(&newfrom, remaining_edges);
	vector_int_init(&newto, remaining_edges);
	vector_int_init(&newoi, remaining_edges);
	vector_int_init(&newii, remaining_edges);
	/* Actually remove the edges, move from pos i to pos j in newfrom/newto */
	for (int i = 0, j = 0; j < remaining_edges; i++) {
		if (0 == mark[i]) {
			VECTOR(newfrom)[j] = VECTOR(graph->from)[i];
			VECTOR(newto)[j] = VECTOR(graph->to)[i];
			j++;
		}
	}
	vector_int_order_inc2(&newfrom, &newto, &newoi, graph->n);
	vector_int_order_inc2(&newto, &newfrom, &newii, graph->n);

	/* attributes */

	/* os & is */
	vector_int_scan_tie(&(graph->os), &newfrom, &newoi, graph->n);
	vector_int_scan_tie(&(graph->is), &newto,   &newii, graph->n);

	/* Ok, we've all memory needed, free the old structure  */
	vector_int_destroy(&graph->from);
	vector_int_destroy(&graph->to);
	vector_int_destroy(&graph->oi);
	vector_int_destroy(&graph->ii);
	graph->from = newfrom;
	graph->to = newto;
	graph->oi = newoi;
	graph->ii = newii;
	Free(mark);
	return 0;
}

/*
 * graph_edge
 */
int graph_edge(const graph_t *graph, int eid, int *from, int *to)
{
	*from = VECTOR(graph->from)[eid];
	*to = VECTOR(graph->to)[eid];

	if (!graph_is_directed(graph) && *from > *to) {
		int tmp = *from;
		*from = *to;
		*to = tmp;
	}
	return 0;
}

/*
 * graph_edges
 */
int graph_edges(const graph_t *graph, graph_es_t eids,vector_int *edges)
{
	graph_eit_t eit;
	graph_eit_create(graph, eids, &eit);

	int ptr = 0;
	int n = GRAPH_EIT_SIZE(eit);
	vector_int_resize(edges, n * 2);
	for(; !GRAPH_EIT_END(eit); GRAPH_EIT_NEXT(eit)) {
		int e = GRAPH_EIT_GET(eit);
		VECTOR(*edges)[ptr++] = GRAPH_FROM(graph, e);
		VECTOR(*edges)[ptr++] = GRAPH_TO(graph,e);
	}
	graph_eit_destroy(&eit);
	return 0;
}

/*
 * graph_reverse
 */
void graph_reverse(graph_t *graph)
{
	vector_int tmp;

	tmp = graph->from;
	graph->from = graph->to;
	graph->to = tmp;

	tmp = graph->oi;
	graph->oi = graph->ii;
	graph->ii = tmp;

	tmp = graph->os;
	graph->os = graph->is;
	graph->is = tmp;
}

int _graph_edges_to(const graph_t *graph, vector_int *eids, int vid)
{
	assert(vid >= 0);
	assert(vid < graph_vertices_count(graph));

	int length = (VECTOR(graph->os)[vid+1] - VECTOR(graph->os)[vid]);
	vector_int_resize(eids, length);

	int idx = 0;
	int j = VECTOR(graph->os)[vid+1];
	for (int i = VECTOR(graph->os)[vid]; i < j; i++)
		VECTOR(*eids)[idx++] = VECTOR(graph->oi)[i];
	return 0;
}
/*
 * graph_remove_multi_edges
 */
void graph_remove_multi_edges(graph_t *graph)
{
	int ec = graph_edges_count(graph);
	int vc = graph_vertices_count(graph);
	/* mark the edges needed to be removed */
	int *mark = Calloc(ec, int);
	assert(0 != mark);

	int to,_to,e;
	vector_int eids;
	vector_int_init(&eids, 0);
	for (int i = 0; i < vc; i++) {
		_graph_edges_to(graph, &eids, i);
		for (int j = 1; j < vector_int_size(&eids); j++) {
			_to = GRAPH_TO(graph, VECTOR(eids)[j-1]);
			to = GRAPH_TO(graph, VECTOR(eids)[j]);
			e = VECTOR(eids)[j];
			if (_to == to)
				mark[e] = 1;
		}
	}

	vector_int_clear(&eids);
	for (int i = 0; i < ec; i++) {
		if (mark[i] != 0)
			vector_int_push_back(&eids, i);
	}
	graph_del_edges(graph, graph_ess_vector(&eids));

	vector_int_destroy(&eids);
	Free(mark);
}

/*
 * print_graph_ct
 */
void print_graph_ct(const graph_t *graph, graph_neimode_t mode, FILE *f)
{
	vector_int tmp;
	vector_int_init(&tmp, 0);

	for (int i = 0; i < graph_vertices_count(graph); i++) {
		fprintf(f,"v%d:\n",i);
		graph_neighbors(graph, &tmp, i, mode);
		for (int j = 0; j < vector_int_size(&tmp); j++)
			fprintf(f,"|   v%d\n", VECTOR(tmp)[j]);
	}
}

void print_graph_vectors(const graph_t *graph, FILE *f)
{
	print_vector_int(&(graph->from),f);
	print_vector_int(&(graph->to),f);
	print_vector_int(&(graph->oi),f);
	print_vector_int(&(graph->ii),f);
	print_vector_int(&(graph->os),f);
	print_vector_int(&(graph->is),f);
}

/*
 * graph_combine_vertices
 */
int graph_combine_vertices(const graph_t *graph, const vector_int *membership, graph_t *res)
{
	int ec = graph_edges_count(graph);

	vector_int new_edges;
	vector_int_init(&new_edges, 2 * ec);

	int from, to, nef, net;
	int eid = 0;
	for (int i = 0; i < ec; i++) {
		graph_edge(graph, i, &from, &to);
		nef = VECTOR(*membership)[from];
		net = VECTOR(*membership)[to];
		if (nef == net || nef < 0 || net < 0) continue;
		VECTOR(new_edges)[2*eid+0] = nef;
		VECTOR(new_edges)[2*eid+1] = net;
		eid++;
	}
	eid--;
	vector_int_resize(&new_edges, 2 * eid);
	new_graph(res, &new_edges, 0, graph->directed);
	graph_remove_multi_edges(res);

	return 0;
}

/*
 * graph_neighbor_subgraph - calculate the @subgraph consists of the neighbors of vertex @s 
 * used for calculate the local efficiency.
 *
 * @neis: neighbors' id
 */
int graph_neighbor_subgraph(const graph_t *graph, graph_t *subgraph, vector_int *neis, int s, graph_neimode_t mode)
{
	int vc = graph_vertices_count(graph);

	graph_neighbors(graph, neis, s, mode);

	vector_int edges;
	vector_int_init(&edges, 0);

	vector_int neis2;
	vector_int_init(&neis2, 0);
	int v, w;
	for (int i = 0; i < vector_int_size(neis); i++) {
		v = VECTOR(*neis)[i];
		if (v == s) continue;
		graph_neighbors(graph, &neis2, v, GRAPH_OUT);
		for (int j = 0; j < vector_int_size(&neis2); j++) {
			w = VECTOR(neis2)[j];
			if (w == s || w == v) continue;
			int wid = vector_int_whereis(neis, w);
			if (-1 != wid) {
				vector_int_push_back(&edges, i);
				vector_int_push_back(&edges, wid);
			}
		}
	}

	new_graph(subgraph, &edges, vector_int_size(neis), graph_is_directed(graph));

	vector_int_destroy(&neis2);
	vector_int_destroy(&edges);
	return 0;
}

int graph_subgraph(const graph_t *graph, graph_t *subgraph, graph_vs_t vids)
{
	int vc = graph_vertices_count(graph);
	int ec = graph_edges_count(graph);

	vector_int vss;
	vector_int_init(&vss, vc);
	vector_int_fill(&vss, -1);

	int i,j,u,v;
    graph_vit_t vit;
	graph_vit_create(graph, vids, &vit);
	int nvc = GRAPH_VIT_SIZE(vit);
    for (GRAPH_VIT_RESET(vit), i=0; !GRAPH_VIT_END(vit); GRAPH_VIT_NEXT(vit), i++) {
        int vid = GRAPH_VIT_GET(vit);
		VECTOR(vss)[vid] = i;
	}
	graph_vit_destroy(&vit);

	int nec = 0;
	vector_int new_edges;
	vector_int_init(&new_edges, 2 * ec);
	for (int eid = 0; eid < ec; eid++) {
		graph_edge(graph, eid, &i, &j);
		u = VECTOR(vss)[i];
		v = VECTOR(vss)[j];
		if (-1 == u || -1 == v)
			continue;
		VECTOR(new_edges)[2*nec+0] = u;
		VECTOR(new_edges)[2*nec+1] = v;
		nec++;
	}
	vector_int_resize(&new_edges, 2 * nec);

	new_graph(subgraph, &new_edges, nvc, graph_is_directed(graph));

	vector_int_destroy(&new_edges);
	vector_int_destroy(&vss);
	return 0;
}
