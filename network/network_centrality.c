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

int main(int argc, char *argv[])
{
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

	/* degree centrality */
    vector_int iv1;
	vector_int_init(&iv1, 0);
/*
	graph_degree(&lkn, &iv1, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("out degs:\n");
	print_vector_int3(&iv1, '\n',stdout);
	graph_degree(&lkn, &iv1, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	printf("in degs:\n");
	print_vector_int3(&iv1, '\n',stdout);
	graph_degree(&lkn, &iv1, graph_vss_all(), GRAPH_ALL, GRAPH_NO_LOOPS);
	printf("all degs:\n");
	print_vector_int(&iv1, stdout);
	graph_degree(&lkn, &iv1, graph_vss_all(), GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
	printf("all nobidir degs:\n");
	print_vector_int(&iv1, stdout);
*/
	/* degree distribution */
	int min, max, sum;
	double ave;
	vector_double fv2;
	vector_double_init(&fv2, 0);
/*
	graph_degree_distribution(&lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	graph_degree_minmax_avesum(&lkn, &min, &max, &ave, &sum, GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("out-degree: minout=%d maxout=%d sumout=%d aveout=%f\n",min,max,sum,ave);
	print_vector_double(&fv2, stdout);

	graph_degree_distribution(&lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	graph_degree_minmax_avesum(&lkn, &min, &max, &ave, &sum, GRAPH_IN, GRAPH_NO_LOOPS);
	printf("in-degree: minin=%d maxin=%d sumin=%d avein=%f\n",min,max,sum,ave);
	print_vector_double(&fv2, stdout);

	graph_degree_distribution(&lkn, &fv2, graph_vss_all(), GRAPH_ALL, GRAPH_NO_LOOPS);
	graph_degree_minmax_avesum(&lkn, &min, &max, &ave, &sum, GRAPH_ALL, GRAPH_NO_LOOPS);
	printf("all-degree: minout=%d maxout=%d sumout=%d aveout=%f\n",min,max,sum,ave);
	print_vector_double(&fv2, stdout);

	graph_degree_distribution(&lkn, &fv2, graph_vss_all(), GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
	graph_degree_minmax_avesum(&lkn, &min, &max, &ave, &sum, GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
	printf("all_nobidir-degree: minin=%d maxin=%d sumin=%d avein=%f\n",min,max,sum,ave);
	print_vector_double(&fv2, stdout);

	printf("clustering: all circle middleman in out\n");
	graph_clustering_directed(&lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_ALL);
	print_vector_double(&fv2, stdout);
	graph_clustering_directed(&lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE);
	print_vector_double(&fv2, stdout);
	graph_clustering_directed(&lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN);
	print_vector_double(&fv2, stdout);
	graph_clustering_directed(&lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_IN);
	print_vector_double(&fv2, stdout);
	graph_clustering_directed(&lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_OUT);
	print_vector_double(&fv2, stdout);

	printf("clustering distribution: out-> all circle middleman in out\n");
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_ALL, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_CIRCLE, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_MIDDLEMAN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_IN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_OUT, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);

	printf("clustering distribution: in-> all circle middleman in out\n");
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_ALL, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_CIRCLE, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_MIDDLEMAN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_IN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
	graph_ave_clustering_directed_degree(&lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_OUT, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, stdout);
*/

	printf("the relationship between in-degree and out-degree\n");
	vector_double inout, outin;
	vector_double_init(&inout, 0);
	vector_double_init(&outin, 0);
	graph_relation_out_dev_in_degree(&lkn, &inout, &outin, graph_vss_all(), GRAPH_NO_LOOPS);
	/*
	printf("in -> dev\n");
	print_vector_double3(&inout, '\n',stdout);
	*/
	printf("out -> dev\n");
	print_vector_double3(&outin, '\n', stdout);

	vector_double_destroy(&fv2);
	vector_int_destroy(&iv1);
	vector_int_destroy(&edges);
	graph_destroy(&lkn);
	return 0;
}
