DIR_LAB = ../lib
DIR_INC = ../include
DIR_BIN = ../bin

SRC = ${wildcard *.c}
OBJ = $(patsubst %.c, %.o,$(notdir ${SRC}))
LAB = -L${DIR_LAB} -lgyc -lgycparser -lm

TARGET = ${DIR_BIN}/gyc.exe

CC = gcc
CFLAGS = -std=gnu99 -D_GNU_SOURCE -I${DIR_INC}

$(TARGET): $(OBJ)
	$(CC) $(OBJ) $(LAB) -o $@

%.o:%.c
	gcc $(CFLAGS) $(DEBUG) -c $< -o $@

clean:
	-rm *~ *.o *.exe *.ct
