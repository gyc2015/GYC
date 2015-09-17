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
    //char *path = "/home/gyc/Sources/linux.doc/";
    if(2 != argc) {
        printf("Please specify the directory of ct-files\n");
        return 1;
    }

    char *path = argv[1];
    SetupLexer();
    dic_setup();
    struct dictionary *dict = new_dictionary(4);

    graph_t lkn;
    vector_int edges;
    catch_function_call_dir(path, dict, &edges);
    printf("capacity=%d,size=%d\n",dict_capacity(dict), dict_size(dict));
    new_graph(&lkn, &edges, 0, GRAPH_DIRECTED);
	graph_reverse(&lkn);
	printf("ec=%d\t",graph_edges_count(&lkn));
	printf("vc=%d\n",graph_vertices_count(&lkn));

	printf("global efficiency = %f\n",graph_global_efficiency(&lkn, GRAPH_OUT));
	printf("local efficiency = %f\n",graph_local_efficiency(&lkn, GRAPH_OUT));
	printf("\n");
	printf("clustering coefficient = %f\n",graph_ave_clustering_directed(&lkn, graph_vss_all(), GRAPH_TRIANGLE_ALL));
	printf("clustering circle = %f\n",graph_ave_clustering_directed(&lkn, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE));
	printf("clustering middleman = %f\n",graph_ave_clustering_directed(&lkn, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN));
	printf("clustering in = %f\n",graph_ave_clustering_directed(&lkn, graph_vss_all(), GRAPH_TRIANGLE_IN));
	printf("clustering out = %f\n",graph_ave_clustering_directed(&lkn, graph_vss_all(), GRAPH_TRIANGLE_OUT));

	vector_int_destroy(&edges);
	graph_destroy(&lkn);
	return 0;
}
