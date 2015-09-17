#include "dictionary.h"
#include "graph_topological_properties.h"
#include "graph_centrality.h"
#include "graph_fastcommunity.h"
#include "graph_community.h"
#include "lex.h"
#include "ctparser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>


void print_edge(vector_int *edge, FILE *f)
{
    int nedge = vector_int_size(edge) / 2;
    for (int i = 0; i < nedge; i++) {
        fprintf(f, "%d\t%d\n", (int)VECTOR(*edge)[2*i+0],(int)VECTOR(*edge)[2*i+1]);
    }
}

void output_com_filename(const vector_funcP *funcfile, const vector_int *ass, int nfunc, int ncom,
		struct dictionary *filedict, FILE *f)
{
	vector_char str;
	vector_char_init(&str,100);
	VECTOR(str)[0] = '\0';

	vector_funcP clist;
	vector_funcP_init_(&clist, ncom);
	for (int funcID = 0; funcID < nfunc; funcID++) {
		int comID = VECTOR(*ass)[funcID];
		vector_ffpair *ffps = &(VECTOR(*funcfile)[funcID].ffps);
		vector_ffpair *cfps = &(VECTOR(clist)[comID].ffps);
		for (int i = 0; i < vector_ffpair_size(ffps); i++) {
			int fileID = VECTOR(*ffps)[i].fileID;
			ffpair *cfp = _scan_func_for_file(&(VECTOR(clist)[comID]),fileID);
			if (0 == cfp) {
				cfp = vector_ffpair_new_item(cfps);	
				cfp->funcID = funcID;
				cfp->fileID = fileID;
				cfp->ndef = 0;
				cfp->ncal = 0;
			}
			cfp->ndef += VECTOR(*ffps)[i].ndef;
			cfp->ncal += VECTOR(*ffps)[i].ncal;
		}
	}
	for (int comID = 0; comID < ncom; comID++) {
		vector_ffpair *cfps = &(VECTOR(clist)[comID].ffps);
		for (int i = 0; i < vector_ffpair_size(cfps); i++) {
			fprintf(f, "%d\t",comID);
			int fileID = VECTOR(*cfps)[i].fileID;
			struct rb_node* e = dict_ele(filedict, fileID);
			dic_traceback_string(e, &str);
			fprintf(f, "%s\t",VECTOR(str));
			fprintf(f, "%d\t",VECTOR(*cfps)[i].ndef);
			fprintf(f, "%d\t",VECTOR(*cfps)[i].ncal);
			fprintf(f, "%d\n",VECTOR(*cfps)[i].ndef + VECTOR(*cfps)[i].ncal);
		}
	}
	vector_char_destroy(&str);
}

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

	struct dictionary *filedict = new_dictionary(4);
	vector_funcP flist;
	vector_funcP_init_(&flist, dict_size(dict));
	get_function_filename(path2, dict, filedict, &flist);
    printf("filedict: capacity=%d,size=%d\n",dict_capacity(filedict), dict_size(filedict));

	/* reciprocal */
	printf("reciprocal = %f \n", graph_reciprocal(&lkn));
	vector_double res;
	vector_double_init(&res, 0);
	graph_betweenness(&lkn, &res, graph_vss_all(), GRAPH_DIRECTED);
	printf("betweenness directed:"); print_vector_double(&(res),stdout);
	vector_double_destroy(&res);

	/* degree */
    graph_degree(&lkn, &edges, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
    printf(">>>out, no loops");
	int min, max, sum;
	double ave;
	graph_degree_minmax_avesum(&lkn, &min, &max, &ave, &sum, GRAPH_OUT, GRAPH_NO_LOOPS);
	printf("minout=%d\nmaxout=%d\nsumout=%d\naveout=%f\n",min,max,sum,ave);
	graph_degree_minmax_avesum(&lkn, &min, &max, &ave, &sum, GRAPH_IN, GRAPH_NO_LOOPS);
	printf("minin=%d\nmaxin=%d\nsumin=%d\navein=%f\n",min,max,sum,ave);

	/* fast community */
	graph_reverse(&lkn);
	vector_int v1;
	vector_int_init(&v1,0);
	int ncom = 0;
	double modularity = graph_community_fastgreedy(&lkn, &v1, &ncom);
	
	printf("modularity = %f,ncom = %d\n",modularity,ncom);
	FILE *f = fopen("funccom.fc.xlsx","w");
	fprintf(f, "comID\tname\n");
	for (int i = 0; i < dict_size(dict);i++) {
		fprintf(f, "%d\t", VECTOR(v1)[i]);
		struct rb_node* e = dict_ele(dict, i);
		dic_traceback_string(e, &str);
		fprintf(f, "%s\n",VECTOR(str));
	}
	fclose(f);
	f = fopen("comID.fc.xlsx","w");
	output_com_filename(&flist, &v1, graph_vertices_count(&lkn), ncom, filedict, f);
	fclose(f);

	//print_vector_int(&v1, stdout);

	print_communities(&lkn, &v1, "community.fc.xlsx", "comedge.fc.xlsx");

	vector_funcP_destroy(&flist);
	vector_int_destroy(&v1);
	vector_char_destroy(&str);
    vector_int_destroy(&edges);
    graph_destroy(&lkn);
    return 0;
}
