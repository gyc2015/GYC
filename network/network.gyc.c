#include "dictionary.h"
#include "graph_attack.h"
#include "graph_components.h"
#include "graph_topological_properties.h"
#include "graph_centrality.h"
#include "graph_fastcommunity.h"
#include "graph_community.h"
#include "lex.h"
#include "parser.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <dirent.h>
#include <sys/stat.h>

/* --degree_centrality the degree of each vertex. */
char *deg_cen_file_name = NULL;
/* --degree_distribution the degree distribution. */
char *deg_dis_file_name = NULL;
/* --in_out_relation */
char *in_out_file_name = NULL;
/* --clustering_coefficient_centrality */
char *cc_cen_file_name = NULL;
/* --fast_community */
char *fc_file_name = NULL;
/* --fast_community_edges */
char *fc_edge_file_name = NULL;
/* --fast_community_vertices */
char *fc_vertex_file_name = NULL;
/* --fast_community_com_file */
char *fc_com_file_name = NULL;
/* --robustness */
char *att_info_file_name = NULL;

unsigned int out_flag = 0x0000;
/* --ct format output for network. */
#define CT_FLAG 0x0001
/* --dot format output for graphviz. */
#define DOT_FLAG 0x0002
/* --in the input file or directory. */
char *inPath = NULL;
/* --out the output file. */
char *outFileName = NULL;
/* --extension	the file's extension. */
char *extension = NULL;

struct dictionary *func_dict = NULL;
struct dictionary *file_dict = NULL;
vector_ffpair fflist; 

void network_properties(graph_t *lkn);
void network_degree_centrality(graph_t *lkn);
void network_clustering_coefficient_centrality(graph_t *lkn);
void network_fast_community(graph_t *lkn);
void network_robustness(graph_t *lkn);
void fault_propagation_infection_f(graph_t *graph, vector_double *res, double f, double plength, int times, graph_attack_mode mode);
void fault_propagation_infection_p(graph_t *graph, vector_double *res, double p, double flength, int times, graph_attack_mode mode);
void cascading_G_f(graph_t *graph, vector_double *res, double f, double plength, int times, graph_attack_mode mode);
void cascading_G_p(graph_t *graph, vector_double *res, double p, double flength, int times, graph_attack_mode mode);

void usage(int i)
{
    fprintf(stderr, "Usage:  gyc.exe (directory|file) [options]\n");
    fprintf(stderr, "Options:\n");
    fprintf(stderr, "===================================\n");
	fprintf(stderr, "\t--out\t\tthe output file.\n");
	fprintf(stderr, "\t--extension\tthe file's extension.\n");
	fprintf(stderr, "\t--ct\t\tformat output for network.\n");
	fprintf(stderr, "\t--dot\t\tformat output for graphviz.\n");
    fprintf(stderr, "\t--help\t\tdisplay this help and exit.\n");
    fprintf(stderr, "===================================\n");
	fprintf(stderr, "\t--degree_centrality\n");
	fprintf(stderr, "\t\tthe degree of each vertex.\n");
	fprintf(stderr, "\t--degree_distribution\n");
	fprintf(stderr, "\t\tthe degree distribution.\n");
	fprintf(stderr, "\t--in_out_relation\n");
	fprintf(stderr, "\t\tthe relationship between indegrees and outdegrees.\n");
	fprintf(stderr, "\t--clustering_coefficient_centrality\n");
	fprintf(stderr, "\t\tthe clustering coefficient of each vertex.\n");
    fprintf(stderr, "===================================\n");
	fprintf(stderr, "\t--fast_community\n");
	fprintf(stderr, "\t\tmapping each vertex into a community.\n");
	fprintf(stderr, "\t--fast_community_edges\n");
	fprintf(stderr, "\t\tthe edges among communities.\n");
	fprintf(stderr, "\t--fast_community_vertices\n");
	fprintf(stderr, "\t\tthe information of communities.\n");
	fprintf(stderr, "\t--fast_community_com_file.\n");
	fprintf(stderr, "\t\tcommunity files.\n");
    fprintf(stderr, "===================================\n");
	fprintf(stderr, "\t--robustness.\n");
	fprintf(stderr, "\t[I,G]:[random,in,out][p=0.2,f=0.1]length=0.1\t\n");
    fprintf(stderr, "end.\n");
    exit(i);
}

void setup_option(int argc, char* argv[])
{
    if (0 != argc) {
        char *tmp = argv[0];
        int len = strlen(tmp);
        int p = 1;

		if (0 == strcmp("--help", tmp))
			usage(0);
		else if (0 == strcmp("--ct", tmp))
			out_flag = CT_FLAG;
		else if (0 == strcmp("--dot", tmp))
			out_flag = DOT_FLAG;
		else if (0 == strcmp("--degree_centrality", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			deg_cen_file_name = argv[0];
		} else if (0 == strcmp("--degree_distribution", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			deg_dis_file_name = argv[0];
		} else if (0 == strcmp("--in_out_relation", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			in_out_file_name = argv[0];
		} else if (0 == strcmp("--clustering_coefficient_centrality", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			cc_cen_file_name = argv[0];
		} else if (0 == strcmp("--extension", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			else if (0 != strcmp(".ct", argv[0]) && 0 != strcmp(".c", argv[0]))
				usage(1);
			extension = argv[0];
		} else if (0 == strcmp("--out", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			outFileName = argv[0];
		} else if (0 == strcmp("--fast_community", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			fc_file_name = argv[0];
		} else if (0 == strcmp("--fast_community_edges", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			fc_edge_file_name = argv[0];
		} else if (0 == strcmp("--fast_community_vertices", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			fc_vertex_file_name = argv[0];
		} else if (0 == strcmp("--fast_community_com_file", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			fc_com_file_name = argv[0];
		} else if (0 == strcmp("--robustness", tmp)) {
			argc--; argv++;
			if (argc == 0)
				usage(1);
			att_info_file_name = argv[0];
		} else
			usage(1);

		argc--; argv++;
        setup_option(argc, argv);
    }
}

int main(int argc, char *argv[])
{
    if(2 > argc) {
        printf("Please specify the directory of ct-files\n");
        return 1;
    }

	/*
	 * setups
	 */
	extension = ".c";
	setup_option(argc-2, argv+2);

    SetupLexer();
    dic_setup();
	func_dict = new_dictionary(4);
	file_dict = new_dictionary(4);
	vector_ffpair_init(&fflist, 0);

	/*
	 * parser the file(s) into dictionary.
	 */
	int nedges = 0;
	struct stat info;
	stat(argv[1], &info);
	if (S_ISDIR(info.st_mode)) {
		char *path_name = argv[1];
		nedges = parse_dir(path_name, func_dict, file_dict, &fflist, extension);
	} else if (S_ISREG(info.st_mode)) {
		char *file_name = argv[1];
		dict_add_str(file_dict, file_name);
		nedges = parse_file(file_name, func_dict, file_dict, &fflist);
	} else {
        fprintf(stderr, "Please specify the file's name or directory's name\n");
		usage(1);
	}

	/*
	 * create graph and calculate properties
	 */
	vector_int edges;
	vector_int_init(&edges, nedges);
	dict_2_edges(func_dict, &edges, nedges);
    graph_t lkn;
    new_graph(&lkn, &edges, 0, GRAPH_DIRECTED);

	network_properties(&lkn);
	network_clustering_coefficient_centrality(&lkn);
	network_degree_centrality(&lkn);
	network_fast_community(&lkn);
	if (NULL != att_info_file_name)
		network_robustness(&lkn);

	graph_destroy(&lkn);
	vector_int_destroy(&edges);

	/*
	 * output the network in various forms.
	 */
	FILE *out_file = (NULL != outFileName) ? fopen(outFileName,"w") : stdout;

	if (out_flag & CT_FLAG)
		print_funcs_ct(func_dict, out_file);

	if (out_flag & DOT_FLAG)
		print_funcs_dot(func_dict, out_file);

	if (NULL != outFileName)
		fclose(out_file);

	vector_ffpair_destroy(&fflist);
	return 0;
}

void network_robustness(graph_t *lkn)
{
	if (NULL == att_info_file_name)
		return;
	FILE *fatt = fopen(att_info_file_name,"w");

	int vc = graph_vertices_count(lkn);
	vector_int mem;
	vector_int_init(&mem, vc);
	vector_int_fill(&mem, -1);
	vector_int cs;
	vector_int_init(&cs, 0);
	int cc;
	graph_clusters_strong(lkn,&mem,&cs,&cc);
	fprintf(fatt,"<<<combine vertices\n");
	graph_t b;
	graph_combine_vertices(lkn, &mem, &b);
	fprintf(fatt, "ec=%d\t",graph_edges_count(&b));
	fprintf(fatt, "vc=%d\n",graph_vertices_count(&b));

	double p = 0.2, f= 0.1, length = 0.05;
	vector_double inf;
	vector_double_init(&inf, 0);
	fprintf(fatt, ">>>>I:[random,in,out][p=0.2,f=0.1]length=0.1\n");
	fault_propagation_infection_p(&b, &inf, p, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', fatt);
	fault_propagation_infection_f(&b, &inf, f, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', fatt);
	fault_propagation_infection_p(&b, &inf, p, length, 1, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', fatt);
	fault_propagation_infection_f(&b, &inf, f, length, 1, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', fatt);
	fault_propagation_infection_p(&b, &inf, p, length, 1, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', fatt);
	fault_propagation_infection_f(&b, &inf, f, length, 1, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', fatt);

	fprintf(fatt,">>>>G:[random,in,out][p=0.2,f=0.1]length=0.1\n");
	cascading_G_p(&b, &inf, p, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', fatt);
	cascading_G_f(&b, &inf, f, length, 10, GRAPH_ATK_RANDOM);
	print_vector_double3(&inf, '\t', fatt);
	cascading_G_p(&b, &inf, p, length, 10, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', fatt);
	cascading_G_f(&b, &inf, f, length, 10, GRAPH_ATK_INCOMING);
	print_vector_double3(&inf, '\t', fatt);
	cascading_G_p(&b, &inf, p, length, 10, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', fatt);
	cascading_G_f(&b, &inf, f, length, 10, GRAPH_ATK_OUTGOING);
	print_vector_double3(&inf, '\t', fatt);
	vector_double_destroy(&inf);

	graph_destroy(&b);
	vector_int_destroy(&cs);
	vector_int_destroy(&mem);
	fclose(fatt);
}

void network_properties(graph_t *lkn)
{
	printf("\n==== counts ====\n");
	printf("edges:\t%d\n",graph_edges_count(lkn));
	printf("vertices:\t%d\n",graph_vertices_count(lkn));

	printf("\nreciprocal:\t%f \n", graph_reciprocal(lkn));

	printf("\n==== efficiency ====\n");
	printf("global:\t%f\n",graph_global_efficiency(lkn, GRAPH_OUT));
	printf("local:\t%f\n",graph_local_efficiency(lkn, GRAPH_OUT));
}

void output_com_filename(vector_int *v, int ncom, FILE *f)
{
	int nffp = vector_ffpair_size(&fflist);
	int vc = dict_size(func_dict);
	int fc = dict_size(file_dict);
	vector_int funcList,funcOrder,funcTie;

	vector_int_init(&funcList, nffp);
	for (int i = 0; i < nffp; i++) {
		VECTOR(funcList)[i] = VECTOR(fflist)[i].funcID;
	}

	vector_int_init(&funcOrder, nffp);
	vector_int_order_inc(&funcList, &funcOrder, vc);
	vector_int_init(&funcTie, vc);
	vector_int_scan_tie(&funcTie, &funcList, &funcOrder, vc);


	vector_int ndefs, ncals;
	vector_int_init(&ndefs, ncom*fc);
	vector_int_init(&ncals, ncom*fc);

	for (int i = 0; i < vc; i++) {
		int nf = VECTOR(funcTie)[i+1] - VECTOR(funcTie)[i];
		for (int j = 0; j < nf; j++) {
			int order = VECTOR(funcOrder)[VECTOR(funcTie)[i] + j];
			int funcID = VECTOR(fflist)[order].funcID;
			int fileID = VECTOR(fflist)[order].fileID;
			int comID = VECTOR(*v)[funcID];
			(VECTOR(ndefs)[comID*fc + fileID]) += VECTOR(fflist)[order].ndef;
			(VECTOR(ncals)[comID*fc + fileID]) += VECTOR(fflist)[order].ncal;
		}
	}

	vector_char str;
	vector_char_init(&str, 100);
	for (int i = 0; i < ncom; i++) {
		for (int j = 0; j < fc; j++) {
			int ncal = VECTOR(ncals)[i*fc + j];
			int ndef = VECTOR(ndefs)[i*fc + j];
			if (0 != ndef || 0 != ncal) {
				fprintf(f,"%d\t%s\t%d\t%d\n", i, dic_get_name(file_dict, j, &str), ndef, ncal);
			}	
		}
	}
	fprintf(stderr,"\n");

	vector_char_destroy(&str);
	vector_int_destroy(&ndefs);
	vector_int_destroy(&ncals);
	vector_int_destroy(&funcList);
	vector_int_destroy(&funcOrder);
	vector_int_destroy(&funcTie);
}


void network_fast_community(graph_t *lkn)
{
	int ncom = 0;
	vector_int v1;
	vector_int_init(&v1,0);
	vector_char str;
	vector_char_init(&str, 100);


	double modularity = graph_community_fastgreedy(lkn, &v1, &ncom);
	ncom = vector_int_max(&v1) + 1;
	printf("\n==== community ====\n");
	printf("modularity:\t%f\nncom:\t%d\n", modularity, ncom);

	FILE *f, *fe, *fv;
	if (NULL != fc_com_file_name) {
		f = fopen(fc_com_file_name, "w");
		output_com_filename(&v1, ncom, f);
		fclose(f);
	}

	if (NULL != fc_file_name && NULL != fc_edge_file_name && NULL != fc_vertex_file_name) {
		f = fopen(fc_file_name, "w");
		fe = fopen(fc_edge_file_name, "w");
		fv = fopen(fc_vertex_file_name, "w");

		fprintf(f, "comID\tname\n");
		for (int i = 0; i < dict_size(func_dict);i++) {
			fprintf(f, "%d\t", VECTOR(v1)[i]);
			fprintf(f, "%s\n", dic_get_name(func_dict, i, &str));
		}

		output_community_graph(lkn, &v1, fv, fe);

		fclose(f);
		fclose(fe);
		fclose(fv);
	}

	vector_char_destroy(&str);
	vector_int_destroy(&v1);
}

void network_clustering_coefficient_centrality(graph_t *lkn)
{
	printf("\nclustering coefficient:\n");
	printf("all:\t%f\n",graph_ave_clustering_directed(lkn, graph_vss_all(), GRAPH_TRIANGLE_ALL));
	printf("in:\t%f\n",graph_ave_clustering_directed(lkn, graph_vss_all(), GRAPH_TRIANGLE_IN));
	printf("out:\t%f\n",graph_ave_clustering_directed(lkn, graph_vss_all(), GRAPH_TRIANGLE_OUT));
	printf("circle:\t%f\n",graph_ave_clustering_directed(lkn, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE));
	printf("middleman:\t%f\n",graph_ave_clustering_directed(lkn, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN));

	if (NULL == cc_cen_file_name)
		return;

	FILE *f = fopen(cc_cen_file_name, "w");

	vector_double fv2;
	vector_double_init(&fv2, 0);

	fprintf(f, "clustering: all circle middleman in out\n");
	graph_clustering_directed(lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_ALL);
	print_vector_double(&fv2, f);
	graph_clustering_directed(lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_CIRCLE);
	print_vector_double(&fv2, f);
	graph_clustering_directed(lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_MIDDLEMAN);
	print_vector_double(&fv2, f);
	graph_clustering_directed(lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_IN);
	print_vector_double(&fv2, f);
	graph_clustering_directed(lkn, &fv2, graph_vss_all(), GRAPH_TRIANGLE_OUT);
	print_vector_double(&fv2, f);

	fprintf(f, "clustering distribution: out-> all circle middleman in out\n");
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_ALL, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_CIRCLE, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_MIDDLEMAN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_IN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_TRIANGLE_OUT, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);

	fprintf(f, "clustering distribution: in-> all circle middleman in out\n");
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_ALL, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_CIRCLE, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_MIDDLEMAN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_IN, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);
	graph_ave_clustering_directed_degree(lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_TRIANGLE_OUT, GRAPH_NO_LOOPS);
	print_vector_double(&fv2, f);

	fclose(f);
	vector_double_destroy(&fv2);
}

void network_degree_centrality(graph_t *lkn)
{
	int min, max, sum, n, minid, maxid;
	double ave;
	FILE *f;
	vector_char str;
	vector_char_init(&str, 100);

	vector_int iv1, iv2, iv3, iv4;
	vector_int_init(&iv1, 0);
	vector_int_init(&iv2, 0);
	vector_int_init(&iv3, 0);
	vector_int_init(&iv4, 0);

	graph_degree(lkn, &iv1, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
	vector_int_which_minmax(&iv1, &minid, &maxid);
	min = VECTOR(iv1)[minid]; max = VECTOR(iv1)[maxid];
	sum = vector_int_sum(&iv1); n = graph_vertices_count(lkn);
	ave = (double)sum / (double)n;
	printf("\nout-degree:\n");
	printf("minout:\t%d\t%s\n", min, dic_get_name(func_dict, minid, &str));
	printf("maxout:\t%d\t%s\n", max, dic_get_name(func_dict, maxid, &str));
	printf("sumout:\t%d\naveout:\t%f\n",sum,ave);

	graph_degree(lkn, &iv2, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	vector_int_which_minmax(&iv2, &minid, &maxid);
	min = VECTOR(iv2)[minid]; max = VECTOR(iv2)[maxid];
	sum = vector_int_sum(&iv2); n = graph_vertices_count(lkn);
	ave = (double)sum / (double)n;
	printf("\nin-degree:\n");
	printf("minin:\t%d\t%s\n", min, dic_get_name(func_dict, minid, &str));
	printf("maxin:\t%d\t%s\n", max, dic_get_name(func_dict, maxid, &str));
	printf("sumin:\t%d\navein:\t%f\n",sum,ave);

	graph_degree(lkn, &iv3, graph_vss_all(), GRAPH_ALL, GRAPH_NO_LOOPS);
	vector_int_which_minmax(&iv3, &minid, &maxid);
	min = VECTOR(iv3)[minid]; max = VECTOR(iv3)[maxid];
	sum = vector_int_sum(&iv3); n = graph_vertices_count(lkn);
	ave = (double)sum / (double)n;
	printf("\nall-degree:\n");
	printf("minall:\t%d\t%s\n", min, dic_get_name(func_dict, minid, &str));
	printf("maxall:\t%d\t%s\n", max, dic_get_name(func_dict, maxid, &str));
	printf("sumall:\t%d\naveall:\t%f\n",sum,ave);

	graph_degree(lkn, &iv4, graph_vss_all(), GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
	vector_int_which_minmax(&iv4, &minid, &maxid);
	min = VECTOR(iv4)[minid]; max = VECTOR(iv4)[maxid];
	sum = vector_int_sum(&iv4); n = graph_vertices_count(lkn);
	ave = (double)sum / (double)n;
	printf("\nall_nobidir-degree:\n");
	printf("minall_nb:\t%d\t%s\n", min, dic_get_name(func_dict, minid, &str));
	printf("maxall_nb:\t%d\t%s\n", max, dic_get_name(func_dict, maxid, &str));
	printf("sumall_nb:\t%d\naveall_nb:\t%f\n",sum,ave);

	if (NULL != deg_cen_file_name) {
		f = fopen(deg_cen_file_name,"w");
		fprintf(f,"function_name\tout_degree\tin_degree\tall_degree\tall_degree_nb\n");
		for (int i = 0; i < n; i++) {
			fprintf(f, "%s\t",dic_get_name(func_dict, i, &str));
			fprintf(f, "%d\t%d\t%d\t%d\n", VECTOR(iv1)[i], VECTOR(iv2)[i], VECTOR(iv3)[i], VECTOR(iv4)[i]);
		}
		fclose(f);
	}

	vector_int_destroy(&iv1);
	vector_int_destroy(&iv2);
	vector_int_destroy(&iv3);
	vector_int_destroy(&iv4);
	vector_char_destroy(&str);

	if (NULL != deg_dis_file_name) {
		f = fopen(deg_dis_file_name, "w");

		vector_double fv2;
		vector_double_init(&fv2, 0);

		graph_degree_distribution(lkn, &fv2, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);
		graph_degree_minmax_avesum(lkn, &min, &max, &ave, &sum, GRAPH_OUT, GRAPH_NO_LOOPS);
		fprintf(f, "out-degree: min=%d max=%d sum=%d ave=%f\n",min,max,sum,ave);
		print_vector_double(&fv2, f);

		graph_degree_distribution(lkn, &fv2, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
		graph_degree_minmax_avesum(lkn, &min, &max, &ave, &sum, GRAPH_IN, GRAPH_NO_LOOPS);
		fprintf(f, "in-degree: min=%d max=%d sum=%d ave=%f\n",min,max,sum,ave);
		print_vector_double(&fv2, f);

		graph_degree_distribution(lkn, &fv2, graph_vss_all(), GRAPH_ALL, GRAPH_NO_LOOPS);
		graph_degree_minmax_avesum(lkn, &min, &max, &ave, &sum, GRAPH_ALL, GRAPH_NO_LOOPS);
		fprintf(f, "all-degree: min=%d max=%d sum=%d ave=%f\n",min,max,sum,ave);
		print_vector_double(&fv2, f);

		graph_degree_distribution(lkn, &fv2, graph_vss_all(), GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
		graph_degree_minmax_avesum(lkn, &min, &max, &ave, &sum, GRAPH_ALL_NOBIDIR, GRAPH_NO_LOOPS);
		fprintf(f, "all_nobidir-degree: min=%d max=%d sum=%d ave=%f\n",min,max,sum,ave);
		print_vector_double(&fv2, f);

		vector_double_destroy(&fv2);
		fclose(f);
	}
}

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
