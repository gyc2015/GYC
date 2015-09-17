#include "dictionary.h"
#include "graph_attack.h"
#include "graph_components.h"
#include "graph_topological_properties.h"
#include "graph_centrality.h"
#include "graph_fastcommunity.h"
#include "graph_community.h"
#include "lex.h"
#include "ctparser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>


void fault_propagation_infection_f(graph_t *graph, vector_double *res, double f, double plength, int times, graph_attack_mode mode)
{
	vector_double inf;
	vector_double_init(&inf, 0);

	int n = 0;
	for (double p = 0.0; p < 1; p+=plength)
		n++;
	vector_double_resize(res, n);
	vector_double_fill(res, 0.0);

	n = 0;
	for (double p = 0.0; p < 1; p+= plength, n++) {
		for (int i = 0; i < times; i++) {
			VECTOR(*res)[n] += graph_fault_propagation_infection(graph, &inf,f,p, mode);
		}
		VECTOR(*res)[n] = VECTOR(*res)[n] / times;
	}

	vector_double_destroy(&inf);
}

void fault_propagation_infection_p(graph_t *graph, vector_double *res, double p, double flength, int times, graph_attack_mode mode)
{
	vector_double inf;
	vector_double_init(&inf, 0);

	int n = 0;
	for (double f = 0.0; f < 1; f+=flength)
		n++;
	vector_double_resize(res, n);
	vector_double_fill(res, 0.0);

	n = 0;
	for (double f = 0.0; f < 1; f+= flength, n++) {
		for (int i = 0; i < times; i++) {
			VECTOR(*res)[n] += graph_fault_propagation_infection(graph, &inf,f,p, mode);
		}
		VECTOR(*res)[n] = VECTOR(*res)[n] / times;
	}

	vector_double_destroy(&inf);

}

void cascading_G_f(graph_t *graph, vector_double *res, double f, double plength, int times, graph_attack_mode mode)
{
	vector_int inf;
	vector_int_init(&inf, 0);

	int n = 0;
	for (double p = 0.0; p < 1; p+=plength)
		n++;
	vector_double_resize(res, n);
	vector_double_fill(res, 0.0);

	n = 0;
	for (double p = 0.0; p < 1; p+= plength, n++) {
		for (int i = 0; i < times; i++) {
			VECTOR(*res)[n] += graph_cascading_G(graph, &inf, f, p, mode);
		}
		VECTOR(*res)[n] = VECTOR(*res)[n] / times;
	}

	vector_int_destroy(&inf);
}

void cascading_G_p(graph_t *graph, vector_double *res, double p, double flength, int times, graph_attack_mode mode)
{
	vector_int inf;
	vector_int_init(&inf, 0);

	int n = 0;
	for (double f = 0.0; f < 1; f+=flength)
		n++;
	vector_double_resize(res, n);
	vector_double_fill(res, 0.0);

	n = 0;
	for (double f = 0.0; f < 1; f+= flength, n++) {
		for (int i = 0; i < times; i++) {
			VECTOR(*res)[n] += graph_cascading_G(graph, &inf, f, p, mode);
		}
		VECTOR(*res)[n] = VECTOR(*res)[n] / times;
	}

	vector_int_destroy(&inf);

}

int main(int argc, char *argv[])
{
	srandom(time(0));
    /* setup */
    char *path = "./data/ct/";
	char *path2 = "/home/gyc/Sources/linux.doc/kernel/";
	vector_char str;
	vector_char_init(&str,100);
	VECTOR(str)[0] = '\0';

    SetupLexer();
    dic_setup();
    struct dictionary *dict = new_dictionary(10000);

    graph_t lkn;
    vector_int edges;
    catch_function_call_dir(path, dict, &edges);
    printf("capacity=%d,size=%d\n",dict_capacity(dict), dict_size(dict));
    new_graph(&lkn, &edges, 0, GRAPH_DIRECTED);
	graph_reverse(&lkn);
	printf("ec=%d\t",graph_edges_count(&lkn));
	printf("vc=%d\n",graph_vertices_count(&lkn));

	int vc = graph_vertices_count(&lkn);
	vector_int mem;
	vector_int_init(&mem, vc);
	vector_int_fill(&mem, -1);
	vector_int cs;
	vector_int_init(&cs, 0);
	int cc;
	graph_clusters_strong(&lkn,&mem,&cs,&cc);
	printf("<<<combine vertices\n");
	graph_t b;
	graph_combine_vertices(&lkn, &mem, &b);
	printf("ec=%d\t",graph_edges_count(&b));
	printf("vc=%d\n",graph_vertices_count(&b));

	double p = 0.2, f= 0.1, length = 0.05;
	vector_double inf;
	vector_double_init(&inf, 0);
	printf(">>>>I:[random,in,out][p=0.2,f=0.1]length=0.1\n");
	fault_propagation_infection_p(&b, &inf, p, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', stdout);
	fault_propagation_infection_f(&b, &inf, f, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', stdout);
	fault_propagation_infection_p(&b, &inf, p, length, 1, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', stdout);
	fault_propagation_infection_f(&b, &inf, f, length, 1, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', stdout);
	fault_propagation_infection_p(&b, &inf, p, length, 1, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', stdout);
	fault_propagation_infection_f(&b, &inf, f, length, 1, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', stdout);

	printf(">>>>G:[random,in,out][p=0.2,f=0.1]length=0.1\n");
	cascading_G_p(&b, &inf, p, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', stdout);
	cascading_G_f(&b, &inf, f, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', stdout);
	cascading_G_p(&b, &inf, p, length, 10, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', stdout);
	cascading_G_f(&b, &inf, f, length, 10, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', stdout);
	cascading_G_p(&b, &inf, p, length, 10, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', stdout);
	cascading_G_f(&b, &inf, f, length, 10, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', stdout);
	vector_double_destroy(&inf);
	/*
	printf(">>>>randomly attack\n");
	re = graph_fault_propagation(&b, 0.1, 0.2, GRAPH_ATK_RANDOM);
	printf("random re=%f\n",re);
	printf(">>>>outgoing based attack\n");
	re = graph_fault_propagation(&b, 0.1, 0.2, GRAPH_ATK_OUTGOING);
	printf("outgoing re=%f\n",re);
	printf(">>>>incoming based attack\n");
	re = graph_fault_propagation(&b, 0.1, 0.2, GRAPH_ATK_INCOMING);
	printf("incoming re=%f\n",re);
	*/

	graph_destroy(&b);
	vector_int_destroy(&cs);
	vector_int_destroy(&mem);
	vector_int_destroy(&edges);
	graph_destroy(&lkn);
	return 0;
}
