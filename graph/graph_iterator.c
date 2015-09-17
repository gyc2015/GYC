#include "graph_iterator.h"
#include "graph_topological_properties.h"
#include <assert.h>

/* -------------------------------------------------- */
/* Vertex selectors                                   */
/* -------------------------------------------------- */
/*
 * graph_vs_all
 */
int graph_vs_all(graph_vs_t *vs)
{
	vs->type=GRAPH_VS_ALL;
	return 0;
}
/*
 * graph_vss_all
 */
graph_vs_t graph_vss_all() {
  graph_vs_t allvs;
  allvs.type=GRAPH_VS_ALL;
  return allvs;  
}

int graph_vs_is_all(graph_vs_t *vs)
{
	return vs->type == GRAPH_VS_ALL;
}

/*
 * graph_vss_vector
 */
graph_vs_t graph_vss_vector(const vector_int *v)
{
  graph_vs_t vecvs;
  vecvs.type=GRAPH_VS_VECTORPTR;
  vecvs.data.vecptr=v;
  return vecvs;
}

int graph_vs_vector(graph_vs_t *vs, const vector_int *v)
{
  vs->type=GRAPH_VS_VECTORPTR;
  vs->data.vecptr=v;
  return 0;
}



/* -------------------------------------------------- */
/* Vertex iterators                                   */
/* -------------------------------------------------- */
/*
 * graph_vid_create
 */
int graph_vit_create(const graph_t *graph, graph_vs_t vs, graph_vit_t *vit)
{
	vector_int vec;
	int *seen;
	int i, j, n;

	switch (vs.type) {
	case GRAPH_VS_ALL:
		vit->type = GRAPH_VIT_SEQ;
		vit->pos = 0;
		vit->start = 0;
		vit->end = graph_vertices_count(graph);
        break;
	case GRAPH_VS_VECTORPTR:
		vit->type=GRAPH_VIT_VECTORPTR;
		vit->pos=0;
		vit->start=0;
		vit->vec=vs.data.vecptr;
		vit->end=vector_int_size(vit->vec);
		assert(vector_int_isininterval(vit->vec, 0, graph_vertices_count(graph)-1));
		break;
	default:
		assert(0);
		break;
	}
	return 0;
}

/*
 * graph_vit_destroy
 */
void graph_vit_destroy(graph_vit_t *vit)
{
	switch (vit->type) {
	case GRAPH_VIT_SEQ:
	case GRAPH_VIT_VECTORPTR:
		break;
	case GRAPH_VIT_VECTOR:
		//vector_int_destroy((igraph_vector_t*)vit->vec);
		//graph_free((igraph_vector_t*)vit->vec);
		break;
	default:
		assert(0);
		break;
  }
}

/* -------------------------------------------------- */
/* Edge Selectors                                     */
/* -------------------------------------------------- */
/*
 * graph_es_all
 */
int graph_es_all(graph_es_t *es, graph_edgeorder_type_t order)
{
	switch (order) {
	case GRAPH_EDGEORDER_ID:
		es->type=GRAPH_ES_ALL;
		break;
	case GRAPH_EDGEORDER_FROM:
		es->type=GRAPH_ES_ALLFROM;
		break;
	case GRAPH_EDGEORDER_TO:
		es->type=GRAPH_ES_ALLTO;
		break;
	default:
		assert(0);
		break;
	}
	return 0;
}

graph_es_t graph_ess_all(graph_edgeorder_type_t order)
{
	graph_es_t es;
	graph_es_all(&es, order); /* cannot fail */
	return es;  
}

/*
 * graph_es_none
 */
int graph_es_none(graph_es_t *es)
{
	es->type=GRAPH_ES_NONE;
	return 0;
}

graph_es_t graph_ess_none()
{
	graph_es_t es;
	es.type = GRAPH_ES_NONE;
	return es;
}

/*
 * graph_es_vector
 */
int graph_es_vector(graph_es_t *es, const vector_int *v)
{
	es->type = GRAPH_ES_VECTOR;
	es->data.vecptr = v;
	return 0;
}

graph_es_t graph_ess_vector(const vector_int *v)
{
	graph_es_t es;
	es.type = GRAPH_ES_VECTOR;
	es.data.vecptr = v;
	return es;
}
/*
int graph_es_vector_copy(graph_es_t *es, const vector_int *v)
{
	es->type = GRAPH_ES_VECTOR;
	es->data.vecptr = Calloc(1, vector_int);
	assert(0 != es->data.vecptr);
	vector_int_copy((vector_int*)es->data.vecptr,v);
	return 0;
}
*/
/* -------------------------------------------------- */
/* Edge Iterators                                     */
/* -------------------------------------------------- */
/*
 * graph_eit_create_allfromto
 */
int graph_eit_create_allfromto(const graph_t *graph, graph_eit_t *eit, graph_neimode_t mode)
{
	int ec = graph_edges_count(graph);
	int vc = graph_vertices_count(graph);
	vector_int *vec = Calloc(1, vector_int);

	assert(0 != vec);
	vector_int_init(vec, 0);
	vector_int_reserve(vec, ec);

	if (graph_is_directed(graph)) {
		vector_int adj;
		vector_int_init(&adj, 0);
		for (int i = 0; i < vc; i++) {
			graph_incident(graph, &adj, i, mode);
			vector_int_append(vec, &adj);
		}
		vector_int_destroy(&adj);
	} else {
		vector_int adj;
		vector_int_init(&adj, 0);
		int *added = Calloc(graph_edges_count(graph), int);
		assert(0 != added);
		for (int i = 0; i < vc; i++) {
			graph_incident(graph, &adj, i, GRAPH_ALL);
			for (int j = 0; j < vector_int_size(&adj); j++) {
				if (!added[VECTOR(adj)[j]]) {
					vector_int_push_back(vec, VECTOR(adj)[j]);
					added[VECTOR(adj)[j]] += 1;
				}
			}
		}
		vector_int_destroy(&adj);
		Free(added);
	}
 
	eit->type = GRAPH_EIT_VECTOR;
	eit->pos = 0;
	eit->start = 0;
	eit->vec = vec;
	eit->end = vector_int_size(eit->vec);
	return 0;
}

/*
 * graph_eit_create
 *
 * @graph: the graph
 * @es: the edge selector
 * @eit: the edge iterator
 */
int graph_eit_create(const graph_t *graph, graph_es_t es, graph_eit_t *eit)
{
	switch (es.type) {
	case GRAPH_ES_ALL:
		eit->type = GRAPH_EIT_SEQ;
		eit->pos = 0;
		eit->start = 0;
		eit->end = graph_edges_count(graph);
		break;
	case GRAPH_ES_ALLFROM:
		graph_eit_create_allfromto(graph, eit, GRAPH_OUT);
		break;
	case GRAPH_ES_ALLTO:
		graph_eit_create_allfromto(graph, eit, GRAPH_IN);
		break;
	case GRAPH_ES_VECTOR:
	case GRAPH_ES_VECTORPTR:
		eit->type = GRAPH_EIT_VECTORPTR;
		eit->pos=0;
		eit->start=0;
		eit->vec=es.data.vecptr;
		eit->end=vector_int_size(eit->vec);
		assert(vector_int_isininterval(eit->vec, 0, graph_edges_count(graph)-1));
		break;
	default:
		assert(0);
		break;
	}
	return 0;
}

/*
 * graph_eit_destroy
 */
void graph_eit_destroy(graph_eit_t *eit)
{
	switch (eit->type) {
		case GRAPH_EIT_SEQ:
		case GRAPH_EIT_VECTORPTR:
			break;
		case GRAPH_EIT_VECTOR:
			//igraph_vector_destroy((igraph_vector_t*)eit->vec);
			//igraph_free((igraph_vector_t*)eit->vec);
			break;
		default:
			assert(0);
		break;
	}
}
