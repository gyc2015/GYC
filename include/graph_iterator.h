#ifndef GRAPH_ITERATURE_H
#define GRAPH_ITERATURE_H

#include "vector_int.h"
#include "graph_constant.h"
#include "graph_datatype.h"

/* -------------------------------------------------- */
/* Vertex selectors                                   */
/* -------------------------------------------------- */

#define GRAPH_VS_ALL       0
#define GRAPH_VS_ADJ       1
#define GRAPH_VS_NONE      2
#define GRAPH_VS_1         3
#define GRAPH_VS_VECTORPTR 4
#define GRAPH_VS_VECTOR    5
#define GRAPH_VS_SEQ       6
#define GRAPH_VS_NONADJ    7

/* adjacent vertices */
struct graph_vs_t_data_adj
{
	int vid;
	graph_neimode_t mode;
};

/* sequence of vertices from:to */
struct graph_vs_t_data_seq
{
	int from;
	int to;
};

union graph_vs_t_data
{
	
	int vid;						/* single vertex */
	const vector_int *vecptr;				/* vector of vertices */
	struct graph_vs_t_data_adj adj;	/* adjacent vertices */
	struct graph_vs_t_data_seq seq;	/* sequence of vertices from:to */
};

typedef struct graph_vs_t
{
	int type;
	union graph_vs_t_data data;
} graph_vs_t;
 

int graph_vs_all(graph_vs_t *vs);
graph_vs_t graph_vss_all();

int graph_vs_vector(graph_vs_t *vs, const vector_int *v);
graph_vs_t graph_vss_vector(const vector_int *v);


/* -------------------------------------------------- */
/* Vertex iterators                                   */
/* -------------------------------------------------- */

#define GRAPH_VIT_SEQ       0
#define GRAPH_VIT_VECTOR    1
#define GRAPH_VIT_VECTORPTR 2

typedef struct graph_vit_t {
  int type;
  long int pos;
  long int start;
  long int end;
  const vector_int *vec;
} graph_vit_t;

#define GRAPH_VIT_NEXT(vit)  (++((vit).pos))
#define GRAPH_VIT_END(vit)   ((vit).pos >= (vit).end)
#define GRAPH_VIT_SIZE(vit)  ((vit).end - (vit).start)
#define GRAPH_VIT_RESET(vit) ((vit).pos = (vit).start)
#define GRAPH_VIT_GET(vit)  \
  ((((vit).type == GRAPH_VIT_SEQ) ? (vit).pos :  VECTOR(*(vit).vec)[(vit).pos]))

int graph_vit_create(const graph_t *graph, graph_vs_t vs, graph_vit_t *vit);
void graph_vit_destroy(graph_vit_t *vit); 
//int graph_vit_as_vector(graph_vit_t *vit, graph_vector_t *v);

/* -------------------------------------------------- */
/* Edge Selectors                                     */
/* -------------------------------------------------- */

#define GRAPH_ES_ALL       0
#define GRAPH_ES_ALLFROM   1
#define GRAPH_ES_ALLTO     2
#define GRAPH_ES_INCIDENT  3
#define GRAPH_ES_NONE      4
#define GRAPH_ES_1         5
#define GRAPH_ES_VECTORPTR 6
#define GRAPH_ES_VECTOR    7
#define GRAPH_ES_SEQ       8
#define GRAPH_ES_PAIRS     9
#define GRAPH_ES_PATH      10
#define GRAPH_ES_MULTIPAIRS 11

struct graph_es_t_data_incident
{
	int vid;
	graph_neimode_t mode;
};

struct graph_es_t_data_seq
{
	int from;
	int to;
};

struct graph_es_t_data_path
{
	vector_int *ptr;
	int mode;
};

union graph_es_t_data
{
	int vid;
	int eid;
	const vector_int *vecptr;
	struct graph_es_t_data_incident incident;
	struct graph_es_t_data_seq seq;
	struct graph_es_t_data_path path;
};

typedef struct graph_es_t
{
	int type;
	union graph_es_t_data data;
} graph_es_t;


int graph_es_all(graph_es_t *es, graph_edgeorder_type_t order);
graph_es_t graph_ess_all(graph_edgeorder_type_t order);
int graph_es_none(graph_es_t *es);
graph_es_t graph_ess_none();
int graph_es_vector(graph_es_t *es, const vector_int *v);
graph_es_t graph_ess_vector(const vector_int *v);

/* -------------------------------------------------- */
/* Edge Iterators                                     */
/* -------------------------------------------------- */

#define GRAPH_EIT_SEQ       0
#define GRAPH_EIT_VECTOR    1
#define GRAPH_EIT_VECTORPTR 2

typedef struct graph_eit_t
{
	int type;
	int pos;
	int start;
	int end;
	const vector_int *vec;
} graph_eit_t;

#define GRAPH_EIT_NEXT(eit)  (++((eit).pos))
#define GRAPH_EIT_END(eit)   ((eit).pos >= (eit).end)
#define GRAPH_EIT_SIZE(eit)  ((eit).end - (eit).start)
#define GRAPH_EIT_RESET(eit) ((eit).pos = (eit).start)
#define GRAPH_EIT_GET(eit)  \
  ((((eit).type == GRAPH_EIT_SEQ) ? (eit).pos : VECTOR(*(eit).vec)[(eit).pos]))

int graph_eit_create(const graph_t *graph, graph_es_t es, graph_eit_t *eit);
void graph_eit_destroy(graph_eit_t *eit); 
//int graph_eit_as_vector(graph_eit_t *eit, vector_int *v);


#endif
