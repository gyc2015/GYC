#ifndef GRAPH_ATTACK_H
#define GRAPH_ATTACK_H

#include "graph.h"

int graph_cascading_nodes_count(const graph_t *graph, vector_int *inf, double f, double p, graph_attack_mode mode);
double graph_cascading_G(const graph_t *graph, vector_int *inf, double f, double p, graph_attack_mode mode);

double graph_fault_propagation_infection(const graph_t *graph, vector_double *inf, double f, double p, graph_attack_mode mode);
double graph_fault_propagation(const graph_t *graph, double f, double p, graph_attack_mode mode);
#endif
