#include "graph.h"
#include "graph_centrality.h"
#include "graph_topological_properties.h"
#include <stdio.h>
#include <time.h>
#include <assert.h>

void print_edge(vector_int *edge, FILE *f)
{
    int nedge = vector_int_size(edge) / 2;
    for (int i = 0; i < nedge; i++) {
        fprintf(f, "%d\t%d\n", (int)VECTOR(*edge)[2*i+0],(int)VECTOR(*edge)[2*i+1]);
    }
}

int main()
{
	int vc = 9334;
	int ec = 26841;
	vector_int edges;
	vector_int_init(&edges, ec * 2);

	srand((int)time(0));
	int src = -1, dst = -1;
	for (int i = 0, ei = 0; i < ec; i++,ei+=2) {
		src = (int)(rand() % vc);
		dst = (int)(rand() % vc);
		while (src == dst) {
			src = (int)(rand() % vc);
			dst = (int)(rand() % vc);
		}
		VECTOR(edges)[ei] = src;
		VECTOR(edges)[ei+1] = dst;
	}

	graph_t a;
	new_graph(&a, &edges, vc, 1);
	assert(vc == graph_vertices_count(&a));
	assert(ec == graph_edges_count(&a));
	printf("reciprocal = %f \n", graph_reciprocal(&a));
	int min, max, sum;
	double ave;
	graph_degree_minmax_avesum(&a, &min, &max, &ave, &sum, GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("minout=%d\nmaxout=%d\n\n",min,max);
	printf("sum=%d\nave=%f\n\n\n",sum,ave);
	graph_degree_minmax_avesum(&a, &min, &max, &ave, &sum, GRAPH_IN, GRAPH_NO_LOOPS);
	printf("minin=%d\nmaxin=%d\n\n\n",min,max);
	printf("sum=%d\nave=%f\n\n\n",sum,ave);

	FILE * f = fopen("a.sif","w");
	print_edge(&edges, f);
    fclose(f);
	//print_graph_ct(&a, GRAPH_OUT, stdout);
}
