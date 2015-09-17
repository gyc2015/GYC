#include "graph_fastcommunity.h"
#include "graph_iterator.h"
#include "graph_centrality.h"
#include "assert.h"
#include <math.h>

/*
 * [1] M.E.J. Newman. Fast Algorithm for Detecting Community Structure
 * in Networks. Phys. Rev. E69(6)(2004)066133.
 * [2] A. Clauset, M.E.J. Newman, C.Moore. Finding Community Structure
 * in Very Large Networks. Phys. Rev. E70(6)(2004)066111.
 */

#define T fc_pair 
#define TEMPLATE_TYPE(a) a ##_fc_pair
#define TEMPLATE_FUNCTION(a,b) a ##_fc_pair_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

#define T fc_community 
#define TEMPLATE_TYPE(a) a ##_fc_community
#define TEMPLATE_FUNCTION(a,b) a ##_fc_community_## b
#include "vector.tmp.h"
#undef TEMPLATE_FUNCTION
#undef TEMPLATE_TYPE
#undef T

int _fc_pair_compair(void **a, int i, int j)
{
	fc_pair *af = a[i];
	fc_pair *bf = a[j];
	
	if (af->dQ > bf->dQ)
		return 1;
	else if (af->dQ == bf->dQ)
		return 0;
	else
		return -1;
}

void print_fc_pair(fc_pair *pair)
{
	printf("ci=%d,cj=%d,eij=%f,eji=%f,dQ=%f,flag=%d\n",
			pair->ci,pair->cj,pair->eij,pair->eji,pair->dQ,pair->flag);
}

void print_fc_community(fc_community *com)
{
	printf("id=%d,ld=%d,rd=%d,size=%d,a_in=%f,a_out=%f,nneis=%d\n",
			com->id,com->ld,com->rd,com->size,com->a_in,com->a_out,vector_ptr_size(&(com->neis)));
	for (int i = 0; i < vector_ptr_size(&(com->neis)); i++) {
		printf("\t");
		fc_pair *pair = VECTOR(com->neis)[i];
		print_fc_pair(pair);
	}
}

void print_fc_communities(vector_fc_community *communities)
{
	printf("....communities=%d.....\n",vector_fc_community_size(communities));
	for (int i = 0; i < vector_fc_community_size(communities); i++)
		print_fc_community(&(VECTOR(*communities)[i]));
}

void print_fc_pairs(vector_fc_pair *pairs)
{
	printf("....pid=%d.....\n",vector_fc_pair_size(pairs));
	for (int i = 0; i < vector_fc_pair_size(pairs); i++)
		print_fc_pair(&VECTOR(*pairs)[i]);
}

void print_candidates(vector_ptr *cans, int i)
{
	printf("....cans=%d:%d....\n",i,vector_ptr_size(cans));
	for (; i < vector_ptr_size(cans); i++) {
		fc_pair *pair = VECTOR(*cans)[i];
		print_fc_pair(pair);
	}
}

void _destroy_fc_communities(vector_fc_community *communities)
{
	int vc = vector_fc_community_size(communities);
	for (int i = 0; i < vc; i++)
		vector_ptr_destroy(&(VECTOR(*communities)[i].neis));
}

double _build_fc_communities(vector_fc_community *communities, const graph_t *graph)
{
	int vc = graph_vertices_count(graph);
	int ec = graph_edges_count(graph);

	vector_int indegree;
	vector_int outdegree;
	vector_int_init(&indegree, 0);
	vector_int_init(&outdegree, 0);
	graph_degree(graph, &indegree, graph_vss_all(), GRAPH_IN, GRAPH_NO_LOOPS);
	graph_degree(graph, &outdegree, graph_vss_all(), GRAPH_OUT, GRAPH_NO_LOOPS);

	vector_fc_community_resize(communities, vc);
	for (int i = 0; i < vc; i++) {
		VECTOR(*communities)[i].id = i;
		VECTOR(*communities)[i].ld = -1;
		VECTOR(*communities)[i].rd = -1;
		VECTOR(*communities)[i].size = 1;
		VECTOR(*communities)[i].a_in = (double)VECTOR(indegree)[i] / (1.0 * ec);
		VECTOR(*communities)[i].a_out = (double)VECTOR(outdegree)[i] / (1.0 * ec);
		//VECTOR(*communities)[i].a_out = (double)VECTOR(indegree)[i] / (1.0 * ec);
		vector_ptr_init(&(VECTOR(*communities)[i].neis), 0);
	}
	vector_int_destroy(&indegree);
	vector_int_destroy(&outdegree);

	double Q0 = 0;
	for (int i = 0; i < vc; i++)
		Q0 -= VECTOR(*communities)[i].a_in * VECTOR(*communities)[i].a_out;
	return Q0;
}

fc_pair* _scan_neis(const fc_community *com, int vid)
{
	int neisc = vector_ptr_size(&(com->neis));

	for (int i = 0; i < neisc; i++) {
		if (vid == ((fc_pair*)VECTOR(com->neis)[i])->ci || vid == ((fc_pair*)VECTOR(com->neis)[i])->cj)
			return VECTOR(com->neis)[i];
	}
	return 0;
}

void _build_fc_pairs(vector_fc_pair *pairs, vector_fc_community *communities, const graph_t *graph)
{
	int vc = graph_vertices_count(graph);
	int ec = graph_edges_count(graph);
	fc_pair *tmp;

	vector_fc_pair_resize(pairs, ec);

	graph_eit_t edgeit;
	graph_eit_create(graph, graph_ess_all(GRAPH_EDGEORDER_ID), &edgeit); 
	int i, j, pid = 0;
	for (GRAPH_EIT_RESET(edgeit); !GRAPH_EIT_END(edgeit); GRAPH_EIT_NEXT(edgeit)) {
		int eidx = GRAPH_EIT_GET(edgeit);
        /* Create the pairs themselves */
		graph_edge(graph, eidx, &i, &j); 
		if (i == j)
			continue;
		tmp = _scan_neis(&(VECTOR(*communities)[i]),j);
		if (0 != tmp) {
			if (i == tmp->ci)
				tmp->eij = 1.0 / (1.0 * ec);
			else
				tmp->eji = 1.0 / (1.0 * ec);
			continue;
		}
		fc_pair newpair;
		VECTOR(*pairs)[pid].ci = i;
		VECTOR(*pairs)[pid].cj = j;
		VECTOR(*pairs)[pid].eij = 1.0 / (1.0 * ec);
		VECTOR(*pairs)[pid].eji = 0;
		vector_ptr_push_back(&(VECTOR(*communities)[i].neis), &VECTOR(*pairs)[pid]);
		vector_ptr_push_back(&(VECTOR(*communities)[j].neis), &VECTOR(*pairs)[pid]);
		VECTOR(*pairs)[pid].dQ = 0;
		VECTOR(*pairs)[pid].flag = 0;
		pid++;
	}
	graph_eit_destroy(&edgeit);

	double dQ = 0;
	vector_fc_pair_resize(pairs, pid);
	for (pid = 0; pid < vector_fc_pair_size(pairs); pid++) {
		fc_pair* pair = &VECTOR(*pairs)[pid];
		fc_community* ci = &(VECTOR(*communities)[pair->ci]);
		fc_community* cj = &(VECTOR(*communities)[pair->cj]);
		dQ = pair->eij + pair->eji - ci->a_in * cj->a_out - ci->a_out * cj->a_in;
		pair->dQ = dQ;
	}
}

void _build_cans(vector_ptr *cans, vector_fc_pair *pairs)
{
	int pc = vector_fc_pair_size(pairs);
	vector_ptr_resize(cans, pc);
	for (int i = 0; i < pc; i++) {
		VECTOR(*cans)[i] = &VECTOR(*pairs)[i];
	}
}

void _update_dQ(fc_community *u, vector_fc_community *communities)
{
	int nuneis = vector_ptr_size(&(u->neis));
	for (int i = 0; i < nuneis; i++) {
		fc_pair* pair = VECTOR(u->neis)[i];
		fc_community* ci = &(VECTOR(*communities)[pair->ci]);
		fc_community* cj = &(VECTOR(*communities)[pair->cj]);
		double dQ = pair->eij + pair->eji - ci->a_in * cj->a_out - ci->a_out * cj->a_in;
		pair->dQ = dQ;
	}
}

void _add_neighbors(fc_community *u, fc_community *v, vector_fc_community *com)
{
	/* prepare neighbors */
	int nuneis = vector_ptr_size(&(u->neis));
	int nvneis = vector_ptr_size(&(v->neis));
	for (int i = 0; i < nvneis; i++) {
		fc_pair *vpair = VECTOR(v->neis)[i];
		int *cid = (vpair->ci == v->id) ? &(vpair->ci) : &(vpair->cj);
		int *cjd = (vpair->ci != v->id) ? &(vpair->ci) : &(vpair->cj);
		if (*cjd == u->ld || *cjd == u->rd)
			continue;
		fc_pair *upair = _scan_neis(u, *cjd);
		if (0 == upair) {
			*cid = u->id;
			vector_ptr_push_back(&(u->neis),vpair);
		} else {
			fc_community *w = &(VECTOR(*com)[*cjd]);
			vector_ptr_remove(&(w->neis),vpair);
			vpair->flag = 1;
			upair->eij += vpair->eij;
			upair->eji += vpair->eji;
		}
	}
}

int _combine_pair(fc_pair *pair, vector_fc_community *communities)
{
	int ncom = vector_fc_community_size(communities);
	fc_community *u = &VECTOR(*communities)[pair->ci];
	fc_community *v = &VECTOR(*communities)[pair->cj];
	fc_community uv;
	uv.id = ncom;
	uv.ld = pair->ci;
	uv.rd = pair->cj;
	uv.size = u->size + v->size;
	uv.a_in = u->a_in + v->a_in;
	uv.a_out = u->a_out + v->a_out;
	vector_ptr_init(&uv.neis,0);
	_add_neighbors(&uv, u, communities);
	_add_neighbors(&uv, v, communities);
	vector_fc_community_push_back(communities, uv);
	_update_dQ(&uv, communities);
	return uv.id;
}

/*
 * graph_community_fastgreedy
 */
int graph_community_merge_communities_fastgreedy(const graph_t *graph, vector_fc_community *communities, vector_double *modularity)
{
	assert(graph_is_directed(graph));
	assert(!graph_has_multiple(graph));
	assert(0 != communities);
	assert(0 != modularity);

	double Q0 = _build_fc_communities(communities, graph);
	vector_double_push_back(modularity, Q0);

	vector_fc_pair pairs;
	vector_fc_pair_init(&pairs,0);
	_build_fc_pairs(&pairs, communities, graph);

	vector_ptr cans;
	vector_ptr_init(&cans, 0);
	_build_cans(&cans, &pairs);

	int ncans = vector_ptr_size(&cans);
	double Q = Q0;
	for (int i = 0; i < ncans; i++) {
		vector_ptr_build_max_heap(&cans, i, ncans-i, _fc_pair_compair);
		fc_pair *pair = VECTOR(cans)[i];
		if (1 == pair->flag)
			continue;
		Q += pair->dQ;
		vector_double_push_back(modularity, Q);
		_combine_pair(pair, communities);
	}

	vector_fc_pair_destroy(&pairs);
	return 0;
}

void graph_community_collapse(int mid, vector_fc_community *com, vector_int *ass, int mmid)
{
	int vc = vector_int_size(ass);
	fc_community *tmpcom = &(VECTOR(*com)[mid]);
	tmpcom->flag = 1;
	assert(mid == tmpcom->id);
	int ld = tmpcom->ld;
	int rd = tmpcom->rd;
	if (-1 == ld) {
		assert(-1 == rd || -2 == rd);
		if (-1 == rd) {
			tmpcom->rd = -2;
			VECTOR(*ass)[mid] = mmid;
		}
		return;
	}
	graph_community_collapse(ld, com, ass, mmid);
	graph_community_collapse(rd, com, ass, mmid);
}

void graph_community_reset_vertices(int vc, vector_fc_community *com)
{
	fc_community *tmpcom;
	for (int i = 0; i < vector_fc_community_size(com); i++) {
		tmpcom = &(VECTOR(*com)[i]);
		tmpcom->flag = 0;
		if (i < vc)
			tmpcom->rd = -1;
	}
}

/*
 * graph_community_fastgreedy
 *
 * @graph: the graph
 * @ass: the assignment of vertices
 * @ncom: number of communities
 * return: modularity
 */
double graph_community_fastgreedy(const graph_t *graph, vector_int *ass, int *ncom)
{
	int vc = graph_vertices_count(graph);
	vector_int_resize(ass, vc);
	for (int i = 0; i < vc; i++)
		VECTOR(*ass)[i] = i;
	*ncom = 0;

	vector_fc_community communities;
	vector_fc_community_init(&communities, 0);

	vector_double modularity;
	vector_double_init(&modularity, 0);

	graph_community_merge_communities_fastgreedy(graph, &communities, &modularity);

	graph_community_reset_vertices(vc, &communities);
	int id = vector_double_which_max(&modularity);
	double max_modularity = VECTOR(modularity)[id];
	fc_community *tmp;
	for (int mmid = id+vc;id > 0; id--, mmid--) {
		tmp = &(VECTOR(communities)[mmid]);
		if (0 == tmp->flag) {
			graph_community_collapse(mmid, &communities, ass, mmid);
			(*ncom)++;
		}
	}

	vector_int_rank(ass);

	vector_double_destroy(&modularity);
	_destroy_fc_communities(&communities);
	vector_fc_community_destroy(&communities);
	return max_modularity;
}

