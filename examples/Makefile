DIR_INC = ../include
DIR_NET = ../network
DIR_LIB = ../lib

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o,$(notdir ${SRC}))
TAR = $(patsubst %.o, %.e,$(notdir ${SRC}))

CC = gcc
INC = -I${DIR_INC} -I${DIR_NET}
CFLAGS = -std=gnu99 -D_GNU_SOURCE ${INC}
LIB = -L${DIR_LIB} -L/usr/local/lib
LFLAGS = ${LIB} -lgycgraph -lgyc -lm

TARGET_GRAPH_PRO = tgraph_degree tgraph_betweenness tgraph_community_fastgreedy  tgraph_components tgraph_attack tgraph_topological_properties tgraph_subgraph tgraph_random_graph tgraph_trangle tgraph_clustering
TARGET = tdictionary tvector tqueue tstack tmatrix tgraph tnewgraph trandomgraph tgraph_iterator 
all: ${TARGET} ${TARGET_GRAPH_PRO}

tvector: tvector.o
	$(CC) $< $(LFLAGS) -o $@

tqueue: tqueue.o
	$(CC) $< $(LFLAGS) -o $@

tstack: tstack.o
	$(CC) $< $(LFLAGS) -o $@

tmatrix: tmatrix.o
	$(CC) $< $(LFLAGS) -o $@

tdictionary:tdictionary.o
	$(CC) $< $(LFLAGS) -o $@

tgraph: tgraph.o
	$(CC) $< $(LFLAGS) -o $@

tnewgraph:tnewgraph.o
	$(CC) $< $(LFLAGS) -o $@

trandomgraph:trandomgraph.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_degree: tgraph_degree.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_betweenness: tgraph_betweenness.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_community_fastgreedy: tgraph_community_fastgreedy.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_iterator: tgraph_iterator.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_components: tgraph_components.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_attack: tgraph_attack.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_topological_properties: tgraph_topological_properties.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_subgraph: tgraph_subgraph.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_random_graph: tgraph_random_graph.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_trangle: tgraph_trangle.o
	$(CC) $< $(LFLAGS) -o $@

tgraph_clustering: tgraph_clustering.o
	$(CC) $< $(LFLAGS) -o $@

hehe: tnewgraph.c
	$(CC) $(CFLAGS) $(DEBUG) -E $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

clean:
	-rm ${TARGET}
	-rm *~ *.o *.exe *.ct
