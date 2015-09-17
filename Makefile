ifeq ("$(origin G)","command line")
	DEBUG_STATE = $(G)
endif
ifndef DEBUG_STATE
	DEBUG_STATE = 0
endif

ifeq ($(DEBUG_STATE),1)
	export DEBUG='-g'
endif

DIR_BIN = ./bin
DIR_LIB = ./lib
DIR_INC = ./include

BIN_TARGET = ${DIR_BIN}/network.exe
GYC_TARGET = ${DIR_BIN}/gyc.exe
BASIC_TARGET = ${DIR_LIB}/libgyc.a
GRAPH_TARGET = ${DIR_LIB}/libgycgraph.a
PARSER_TARGET = ${DIR_LIB}/libgycparser.a

all: ${BIN_TARGET} ${GYC_TARGET} exm
	cp ${DIR_BIN}/*.exe .

${BIN_TARGET}:${BASIC_TARGET} ${PARSER_TARGET} ${GRAPH_TARGET} FORCE
	$(MAKE) -C network

${GYC_TARGET}: ${BASIC_TARGET} ${PARSER_TARGET} FORCE
	${MAKE} -C gyc

${GRAPH_TARGET} : ${BASIC_TARGET} FORCE
	$(MAKE) -C graph 

${PARSER_TARGET} : ${BASIC_TARGET} FORCE
	$(MAKE) -C parser 

exm: ${BIN_TARGET} FORCE
	${MAKE} -C examples

${BASIC_TARGET}: FORCE
	${MAKE} -C basic

FORCE:

.PHONY:clean

clean:
	-rm *~ *.exe *.o ${DIR_BIN}/*.exe ${DIR_LIB}/*.a ${DIR_INC}/*~
	-${MAKE} -C graph clean
	-${MAKE} -C gyc clean
	-${MAKE} -C network clean
	-${MAKE} -C basic clean
	-${MAKE} -C examples clean

