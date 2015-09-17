DIR_LAB = ../lib
DIR_INC = ../include

SRC = ${wildcard *.c}
OBJ = $(patsubst %.c, %.o,$(notdir ${SRC}))

TARGET = ${DIR_LAB}/libgycparser.a

CC = gcc
CFLAGS = -std=gnu99 -D_GNU_SOURCE -I${DIR_INC} 

${TARGET}:${OBJ}
	ar crv $@ ${OBJ}

%.o:%.c %.h
	gcc $(CFLAGS) $(DEBUG) -c $< -o $@

clean:
	-rm *~ *.o *.exe *.ct
