DIR_LAB = ../lib
DIR_INC = ../include
DIR_BIN = ../bin

CC = gcc
SRC = ${wildcard *.c}
OBJ = $(patsubst %.c, %.o,$(notdir ${SRC}))
LAB = -L${DIR_LAB}
INC = -I${DIR_INC}
LFLAGS = ${LAB} -lgycgraph -lgycparser -lgyc -lm
CFLAGS = -std=gnu99 -D_GNU_SOURCE ${INC}

TARGET := ${DIR_BIN}/network.gyc.exe
TARGET := ${TARGET} ${DIR_BIN}/network.exe
TARGET := ${TARGET} ${DIR_BIN}/network_attack.exe
TARGET := ${TARGET} ${DIR_BIN}/network_properties.exe
TARGET := ${TARGET} ${DIR_BIN}/network_centrality.exe

all: $(TARGET)

../bin/network.gyc.exe: $(OBJ)
	$(CC) network.gyc.o $(LFLAGS) -o $@

../bin/network_attack.exe: $(OBJ)
	$(CC) ctparser.o network_attack.o $(LFLAGS) -o $@

../bin/network.exe: $(OBJ)
	$(CC) ctparser.o network.o $(LFLAGS) -o $@

../bin/network_properties.exe: $(OBJ)
	$(CC) ctparser.o network_properties.o $(LFLAGS) -o $@

../bin/network_centrality.exe: $(OBJ)
	$(CC) ctparser.o network_centrality.o $(LFLAGS) -o $@

%.o:%.c %.h
	gcc $(CFLAGS) $(DEBUG) -c $< -o $@

clean:
	-rm *~ *.o *.exe *.ct
