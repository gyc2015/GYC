#include "graph_components.h"
#include "graph_topological_properties.h"
#include "vector_int.h"
#include "queue_int.h"
#include "stack_int.h"

/*
 * _bfs_search - bfs search form the source @s
 *
 * @graph: the graph
 * @s: the source
 * @membership: indicates which component the related vertex belong to.
 *				as input, it should be initialized if some vertices have special assignments should reflect in this parameter.
 *				as output, it will return the owner components ID.
 * @mode: indicates the neighborhoods model
 * @cid: specify which component the nodes belong to.
 * return: the size of the component
 */
int _bfs_search(const graph_t *graph, int s, vector_int *membership, graph_neimode_t mode, int cid)
{
	vector_int neis;
	vector_int_init(&neis, 0);

	queue_int Q;
	queue_int_init(&Q, 100);
	queue_int_enq(&Q, s);

	int size = 0;
	while (!queue_int_is_empty(&Q)) {
		int v = queue_int_deq(&Q);
		if (VECTOR(*membership)[v] != -1)
			continue;
		VECTOR(*membership)[v] = cid;
		size++;
		graph_neighbors(graph, &neis, v, mode);
		for (int i = 0; i < vector_int_size(&neis); i++) {
			int w = VECTOR(neis)[i];
			if (-1 == VECTOR(*membership)[w])
				queue_int_enq(&Q, w);
		}
	}

	queue_int_destroy(&Q);
	vector_int_destroy(&neis);
	return size;
}

/*
 * graph_clusters_weak - calculate the weakly connected components in the given graph.
 *
 * @graph: the graph
 * @membership: indicates which component the related vertex belong to.
 *				as input, it should be initialized if some vertices have special assignments should reflect in this parameter.
 *				as output, it will return the owner components ID.
 * @csize: return the size of each component.
 * @no: return the total number of components
 */
int graph_clusters_weak(const graph_t *graph, vector_int *membership, vector_int *csize, int *no)
{
	int vc = graph_vertices_count(graph);
	*no = 0;

	for (int i = 0; i < vc; i++) {
		if (-1 != VECTOR(*membership)[i])
			continue;
		vector_int_push_back(csize, _bfs_search(graph, i, membership, GRAPH_ALL, *no));
		(*no)++;
	}

	return 0;
}


int _dfs_search(const graph_t *graph, int u, vector_int *color, vector_int *d, vector_int *f,graph_neimode_t mode, int cid, int *time)
{
	int size = 0;
	
	VECTOR(*color)[u] = cid;
	(*time)++;
	VECTOR(*d)[u] = *time;

	vector_int neis;
	vector_int_init(&neis, 0);
	graph_neighbors(graph, &neis, u, mode);
	for (int i = 0; i < vector_int_size(&neis); i++) {
		int v = VECTOR(neis)[i];
		if (VECTOR(*color)[v] != -1)
			continue;
		size += _dfs_search(graph, v, color, d, f, mode, cid, time);
	}
	(*time)++;
	VECTOR(*f)[u] = *time;

	vector_int_destroy(&neis);
	return size;
}

/*
 * graph_clusters_strong - calculate the weakly connected components in the given graph.
 *
 * @graph: the graph
 * @membership: indicates which component the related vertex belong to.
 *				as input, it should be initialized if some vertices have special assignments should reflect in this parameter.
 *				as output, it will return the owner components ID.
 * @csize: return the size of each component.
 * @no: return the total number of components
 */
int graph_clusters_strong(const graph_t *graph, vector_int *membership, vector_int *csize, int *no)
{
	int vc = graph_vertices_count(graph);
	*no = 0;

	vector_int color, d, f;
	vector_int_copy(&color, membership);
	vector_int_init(&d, vc);
	vector_int_init(&f, vc);
	vector_int_null(&d);
	vector_int_null(&f);
	int time = 0;
	for (int i = 0; i < vc; i++) {
		if (-1 != VECTOR(color)[i])
			continue;
		_dfs_search(graph, i, &color, &d, &f, GRAPH_OUT, *no, &time);
		(*no)++;
	}

	vector_int vi;
	vector_int_init(&vi, 0);
	vector_int_order_dec(&f, &vi, time);

	*no = 0;
	time = 0;
	vector_int_null(&d);
	vector_int_null(&f);
	for (int j = 0; j < vector_int_size(&vi); j++) {
		int i = VECTOR(vi)[j];
		if (-1 != VECTOR(*membership)[i])
			continue;
		vector_int_push_back(csize, _dfs_search(graph, i, membership, &d, &f, GRAPH_IN, *no, &time));
		(*no)++;
	}

	vector_int_destroy(&vi);
	vector_int_destroy(&color);
	vector_int_destroy(&d);
	vector_int_destroy(&f);
}
