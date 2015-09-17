#include "graph_adjacentlist.h"
#include "graph_topological_properties.h"
#include "graph.h"
#include <assert.h>

/*
 * graph_adjlist_init - initialize an adjacent list for the given graph
 *
 * @graph: the given graph
 * @al: the adjacent list
 * @mode: specifies whitch type of neighbour is included into the adjacent list.
 *        avaliable modes are: GRAPH_OUT, GRAPH_IN and GRAPH_ALL
 */
int graph_adjlist_init(const graph_t *graph, graph_adjlist_t *al, graph_neimode_t mode)
{
	al->length = graph_vertices_count(graph);
	al->adjs = Calloc(al->length, vector_int);
	assert(0 != al->adjs);

	vector_int tmp;
	vector_int_init(&tmp, 0);
	for (int i = 0; i < al->length; i++) {
		graph_neighbors(graph, &tmp, i, mode);
		int n = vector_int_size(&tmp);
		vector_int_init(&(al->adjs[i]),n);
		for (int j = 0; j < n; j++) {
			VECTOR(al->adjs[i])[j] = VECTOR(tmp)[j];
		}
	}
	vector_int_destroy(&tmp);
	return 0;
}
/*
 * graph_adjlist_init_empty
 */
int graph_adjlist_init_empty(graph_adjlist_t *al, int nv)
{
	al->length = nv;
	al->adjs = Calloc(al->length, vector_int);
	assert(0 != al->adjs);

	for (int i = 0; i < al->length; i++)
		vector_int_init(&(al->adjs[i]),0);
	return 0;
}
/*
 * graph_adjlist_destroy
 */
void graph_adjlist_destroy(graph_adjlist_t *al)
{
	for (int i = 0; i < al->length; i++)
		if (0 != &al->adjs[i])
			vector_int_destroy(&(al->adjs[i]));
	Free(al->adjs);
}

