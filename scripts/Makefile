LABS = -lm
OBJS = lex.o input.o error.o stack.o list.o rbt.o
GYCOBJS = preprocessor.o sym.o keyw.o
NTWOBJS = ctparser.o dictionary.o geodesic_path.o dfs.o maxheap.o

OP = -std=gnu99 -D_GNU_SOURCE

ALL: gyc.c network.c $(GYCOBJS) $(NTWOBJS) $(OBJS)
	gcc $(OP) gyc.c $(GYCOBJS) $(OBJS) $(LABS) -o gyc.exe
	gcc $(OP) network.c $(NTWOBJS) $(OBJS) $(LABS) -o network.exe

test: maxheap_test.c maxheap.o maxheap.h
	gcc $(OP) maxheap_test.c maxheap.o $(LABS) -o test.exe

GYC: gyc.c  $(GYCOBJS) $(OBJS)
	gcc $(OP) gyc.c $(GYCOBJS) $(OBJS) $(LABS) -o gyc.exe

network: network.c network.h $(NTWOBJS) $(OBJS)
	gcc $(OP) network.c $(NTWOBJS) $(OBJS) $(LABS) -o network.exe

dfs.o: dfs.c network.h
	gcc $(OP) -c dfs.c

geodesic_path.o: geodesic_path.c network.h maxheap.h
	gcc $(OP) -c geodesic_path.c

preprocessor.o: preprocessor.c preprocessor.h list.h
	gcc $(OP) -c preprocessor.c

ctparser.o: ctparser.c ctparser.h dictionary.h dictionary.o
	gcc $(OP) -c ctparser.c

lex.o: lex.c lex.h input.h keyword.h error.h
	gcc $(OP) -c lex.c

keyw.o: sym.h keyw.c
	gcc $(OP) -c keyw.c

sym.o: sym.c sym.h
	gcc $(OP) -c sym.c

stack.o: stack.c stack.h list.h gyc.h
	gcc $(OP) -c stack.c

dictionary.o: dictionary.c dictionary.h gyc.h
	gcc $(OP) -c dictionary.c

maxheap.o: maxheap.c maxheap.h
	gcc $(OP) -c maxheap.c

list.o: list.c list.h gyc.h
	gcc $(OP) -c list.c

rbt.o: rbt.c rbt.h
	gcc $(OP) -c rbt.c

input.o: input.c input.h
	gcc $(OP) -c input.c

error.o: error.c error.h
	gcc $(OP) -c error.c

INSTALL:
	cp *.exe ../linux.doc/

clean2:
	rm ../linux.doc/*.exe

clean:
	rm *.o *.exe
