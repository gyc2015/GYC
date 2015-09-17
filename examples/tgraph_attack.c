#include "graph_attack.h"
#include "graph_components.h"
#include <stdlib.h>
#include <time.h>
#include <assert.h>

int main()
{
    srandom(time(0));
	graph_t a;
	vector_int v1;
	vector_int_init_value_end(&v1, -1, 1,5, 2,5, 1,2, 2,1, 0,3, 0,2, 3,4, 3,6, 4,6, 6,4, -1,7, -1);
	new_graph(&a, &v1, 0, GRAPH_DIRECTED);
	print_graph_vectors(&a, stdout);
	printf("ec=%d\t",graph_edges_count(&a));
	printf("vc=%d\n",graph_vertices_count(&a));

	int vc = graph_vertices_count(&a);
	vector_int mem;
	vector_int_init(&mem, vc);
	vector_int_fill(&mem, -1);
	vector_int cs;
	vector_int_init(&cs, 0);
	int cc;
	graph_clusters_strong(&a,&mem,&cs,&cc);
	printf("mem:");
	print_vector_int(&mem, stdout);
	printf("<<<combine vertices\n");
	graph_t b;
	graph_combine_vertices(&a, &mem, &b);
	print_graph_vectors(&b, stdout);
	printf("ec=%d\t",graph_edges_count(&b));
	printf("vc=%d\n",graph_vertices_count(&b));

	double re;
	printf(">>>>randomly attack\n");
	re = graph_fault_propagation(&b, 0.3, 0.2, GRAPH_ATK_RANDOM);
	printf("random re=%f\n",re);
	printf(">>>>outgoing based attack\n");
	re = graph_fault_propagation(&b, 0.3, 0.2, GRAPH_ATK_OUTGOING);
	printf("outgoing re=%f\n",re);
	printf(">>>>incoming based attack\n");
	re = graph_fault_propagation(&b, 0.3, 0.2, GRAPH_ATK_INCOMING);
	printf("incoming re=%f\n",re);

	vector_int inf;
	vector_int_init(&inf, 0);
	int cascading_nodes_count = graph_cascading_nodes_count(&b, &inf, 0.3, 0.2, GRAPH_ATK_RANDOM);
	assert(cascading_nodes_count == vector_int_sum(&inf));
	vector_int_destroy(&inf);

	vector_int_destroy(&mem);
	vector_int_destroy(&cs);
	vector_int_destroy(&v1);
	graph_destroy(&b);
	graph_destroy(&a);
	return 0;
}
