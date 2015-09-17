DIR_INC = ../include
DIR_LIB = ../lib

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o,$(notdir ${SRC}))

TARGET = ${DIR_LIB}/libgyc.a

CC = gcc
CFLAGS = -std=gnu99 -D_GNU_SOURCE -I${DIR_INC}

${TARGET}:${OBJ}
	ar crv $@ ${OBJ}

hehe: vector_int.c
	$(CC) $(CFLAGS) $(DEBUG) -E $< -o $@

%.o:%.c
	$(CC) $(CFLAGS) $(DEBUG) -c $< -o $@

clean:
	-rm *~ *.o *.exe *.ct
