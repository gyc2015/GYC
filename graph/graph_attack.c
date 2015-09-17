#include "graph_attack.h"
#include "graph_components.h"
#include "graph_topological_properties.h"
#include "graph_centrality.h"
#include "vector_int.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>


/*
 * _update_node_infection_state
 *
 * @graph: the graph
 * @vid: the node
 * @p: the propagation probability
 * @I: the double vector storing infection status
 * @att: the int vector specifing which node has been attacked
 * @marks: the int vector specifing which node has been updated already.
 */
void _update_node_infection_state(const graph_t *graph, int vid, double p, vector_double *I, vector_int *att, vector_int *marks)
{
	vector_int neis;
	vector_int_init(&neis, 0);
	graph_neighbors(graph, &neis, vid, GRAPH_IN);

	int u;
	double term = 1;
	VECTOR(*marks)[vid] = 1;
	for (int i = 0; i < vector_int_size(&neis); i++) {
		u = VECTOR(neis)[i];
		if (0 == VECTOR(*marks)[u])
			_update_node_infection_state(graph, u, p, I, att, marks);
		term = term * (1.0 - p * VECTOR(*I)[u]);
	}
	term = 1 - term;
	VECTOR(*I)[vid] = VECTOR(*att)[vid] + (1 - VECTOR(*att)[vid]) * term;
}

/*
 * _update_infection_state
 *
 * @graph: the graph
 * @p: the propagation probabiliy
 * @I: the vector of infection state
 * @att: the vector specifing which node is attacked
 */
double _update_infection_state(const graph_t *graph, double p, vector_double *I, vector_int *att)
{
	int vc = graph_vertices_count(graph);

	vector_int marks;
	vector_int_init(&marks, vc);
	vector_int_fill(&marks, 0);

	for (int i = 0; i < vc; i++) {
		if (0 == VECTOR(marks)[i])
			_update_node_infection_state(graph, i, p, I, att, &marks);
	}
	double tmp = 0;
	for (int i = 0; i < vc; i++)
		tmp += VECTOR(*I)[i];
	double re = tmp / vc;
	return re;
}

/*
 * _update_node_failure_state
 *
 * @graph: the graph
 * @vid: the node
 * @p: the propagation probability
 * @I: the vector of failure status
 * @att: the int vector specifing which node has been attacked
 * @marks: the int vector specifing which node has been updated already.
 */
void _update_node_failure_state(const graph_t *graph, int vid, double p, vector_int *I, vector_int *att, vector_int *marks)
{
	vector_int neis;
	vector_int_init(&neis, 0);
	graph_neighbors(graph, &neis, vid, GRAPH_IN);

	int u;
	double term = 1.0;
	VECTOR(*marks)[vid] = 1;
	for (int i = 0; i < vector_int_size(&neis); i++) {
		u = VECTOR(neis)[i];
		if (0 == VECTOR(*marks)[u])
			_update_node_failure_state(graph, u, p, I, att, marks);
		term = term * (1.0 - p * VECTOR(*I)[u]);
	}
	term = 1 - term;
	double ran = 1.0 * random() / RAND_MAX;
	if (1 == VECTOR(*att)[vid])
		VECTOR(*I)[vid] = 1;
	else
		VECTOR(*I)[vid] = (ran < term) ? 1 : 0;
}

/*
 * _update_failure_state
 *
 * @graph: the graph
 * @p: the propagation probabiliy
 * @I: the vector of infection state
 * @att: the vector specifing which node is attacked
 */
int _update_failure_state(const graph_t *graph, double p, vector_int *I, vector_int *att)
{
	int vc = graph_vertices_count(graph);

	vector_int marks;
	vector_int_init(&marks, vc);
	vector_int_fill(&marks, 0);

	for (int i = 0; i < vc; i++) {
		if (0 == VECTOR(marks)[i])
			_update_node_failure_state(graph, i, p, I, att, &marks);
	}

	int tmp = 0;
	for (int i = 0; i < vc; i++)
		tmp += VECTOR(*I)[i];
	return tmp;
}

/*
 * _attack_nodes - specify which node is attacked
 *
 * @att: the vector storing the result
 * @f: the ratio of nodes to be attacked
 */
void _attack_nodes(const graph_t *graph, vector_int *att, double f, graph_attack_mode mode)
{
	int n = vector_int_size(att);
	double natt = n * f;
	assert(natt <= n);

	vector_int candidates;
	vector_int_init(&candidates, n);

	int tmp,id;
	if (GRAPH_ATK_RANDOM == mode) {
		for (int i = 0; i < n; i++)
			VECTOR(candidates)[i] = i;
		for (int i = 0; i < natt; i++) {
			tmp = (1 < vector_int_size(&candidates)) ? (random() % (vector_int_size(&candidates) - 1)) : 0;
			id = VECTOR(candidates)[tmp];
			VECTOR(*att)[id] = 1;
			vector_int_remove(&candidates, tmp);
		}
	} else {
		vector_int deglist;
		vector_int_init(&deglist, n);
		if (GRAPH_ATK_OUTGOING == mode)
			graph_degree(graph, &deglist, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
		else if (GRAPH_ATK_INCOMING == mode)
			graph_degree(graph, &deglist, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
		vector_int_order_dec(&deglist, &candidates, vector_int_max(&deglist));
		for (int i = 0; i < natt; i++) {
			id = VECTOR(candidates)[i];
			VECTOR(*att)[id] = 1;
		}
		vector_int_destroy(&deglist);
	}
	vector_int_destroy(&candidates);
}

/*
 * graph_fault_propagation_infection
 *
 * @graph: the graph
 * @inf: the vector used to store the infection of each node
 * @f: the rate of nodes to be attacked
 * @p: the propagation probability
 * @mode: the attack mode
 * return the infection the whole graph
 */
double graph_fault_propagation_infection(const graph_t *graph, vector_double *inf, double f, double p, graph_attack_mode mode)
{
	int vc = graph_vertices_count(graph);

	vector_int att;
	vector_int_init(&att, vc);
	vector_int_fill(&att, 0);

	vector_double_resize(inf, vc);
	vector_double_fill(inf, 0.0);

	_attack_nodes(graph, &att, f, mode);
	double re = _update_infection_state(graph, p, inf, &att);

	vector_int_destroy(&att);
	return re;
}

/*
 *
 */
int graph_cascading_nodes_count(const graph_t *graph, vector_int *inf, double f, double p, graph_attack_mode mode)
{
	int vc = graph_vertices_count(graph);

	vector_int att;
	vector_int_init(&att, vc);
	vector_int_fill(&att, 0);

	vector_int_resize(inf, vc);
	vector_int_fill(inf, 0);

	_attack_nodes(graph, &att, f, mode);
	int re = _update_failure_state(graph, p, inf, &att);

	vector_int_destroy(&att);
	return re;
}


double graph_cascading_G(const graph_t *graph, vector_int *inf, double f, double p, graph_attack_mode mode)
{
	int vc = graph_vertices_count(graph);

	vector_int att;
	vector_int_init(&att, vc);
	vector_int_fill(&att, 0);

	vector_int_resize(inf, vc);
	vector_int_fill(inf, 0);

	_attack_nodes(graph, &att, f, mode);
	int atvc = _update_failure_state(graph, p, inf, &att);

	vector_int rev;
	vector_int_init(&rev, vc - atvc);
	for (int i = 0, j = 0; i < vc; i++) {
		if (1 == VECTOR(*inf)[i])
			continue;
		VECTOR(rev)[j] = i;
		j++;
	}

	int re = 0;
	if (atvc >= vc) {
		re = 0;
	} else {
		int no;
		graph_t b;
		graph_subgraph(graph, &b, graph_vss_vector(&rev));
		int bvc = graph_vertices_count(&b);
		vector_int membership, csize;
		vector_int_init(&membership, bvc);
		vector_int_fill(&membership, -1);
		vector_int_init(&csize, 0);
		graph_clusters_weak(&b, &membership, &csize, &no);
		re = vector_int_max(&csize);
		vector_int_destroy(&csize);
		vector_int_destroy(&membership);
		graph_destroy(&b);
	}

	vector_int_destroy(&rev);
	vector_int_destroy(&att);
	return 1.0 * re / vc;
}

double graph_fault_propagation(const graph_t *graph, double f, double p, graph_attack_mode mode)
{
	int vc = graph_vertices_count(graph);

	vector_int att;
	vector_int_init(&att, vc);
	vector_int_fill(&att, 0);

	vector_double I;
	vector_double_init(&I, vc);
	vector_double_fill(&I, 0.0);

	_attack_nodes(graph, &att, f, mode);
	double re = _update_infection_state(graph, p, &I, &att);
	printf("infection status:re=%f\n",re);
	print_vector_double(&I, stdout);

	int min, max, deg;
	vector_int deglist, degdist;
	vector_double idist;

	vector_int_init(&deglist, 0);
	graph_degree(graph, &deglist, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	vector_int_minmax(&deglist, &min, &max);

	vector_int_init(&degdist, max - min + 1);
	vector_double_init(&idist, max - min + 1);
	vector_int_fill(&degdist, 0);
	vector_double_fill(&idist, 0.0);
	for (int i = 0; i < vc; i++) {
		deg = VECTOR(deglist)[i];
		VECTOR(degdist)[deg]++;
		VECTOR(idist)[deg] += VECTOR(I)[i]; 
	}
	for (int i = 0; i < vector_int_size(&degdist); i++)
		VECTOR(idist)[i] = (0 == VECTOR(degdist)[i]) ? -1 : (VECTOR(idist)[i] / (double)(VECTOR(degdist)[i]));
	printf("Ii V.S. outdeg:\n");
	print_vector_double(&idist, stdout);

	graph_degree(graph, &deglist, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	vector_int_minmax(&deglist, &min, &max);
	vector_int_resize(&degdist, max - min + 1);
	vector_double_resize(&idist, max - min + 1);
	vector_int_fill(&degdist, 0);
	vector_double_fill(&idist, 0.0);
	for (int i = 0; i < vc; i++) {
		deg = VECTOR(deglist)[i];
		VECTOR(degdist)[deg]++;
		VECTOR(idist)[deg] += VECTOR(I)[i]; 
	}
	for (int i = 0; i < vector_int_size(&degdist); i++)
		VECTOR(idist)[i] = (0 == VECTOR(degdist)[i]) ? -1 : (VECTOR(idist)[i] / (double)(VECTOR(degdist)[i]));
	printf("Ii V.S. indeg:\n");
	print_vector_double(&idist, stdout);

	vector_double_destroy(&idist);
	vector_int_destroy(&degdist);
	vector_int_destroy(&deglist);
	vector_double_destroy(&I);
	vector_int_destroy(&att);
	return re;
}
