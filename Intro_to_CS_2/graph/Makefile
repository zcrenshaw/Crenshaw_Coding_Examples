# Makefile for CS152 Lab 9

.PHONY : clean

all : il_ev iq_ev is_ev vx_ev graph_ev bfs dfs

il_ev : il.h il.c il_ev.c
	clang -g -Wall -o il_ev il.c il_ev.c -lm

iq_ev : il.h il.c iq.h iq.c iq_ev.c
	clang -g -Wall -o iq_ev il.c iq.c iq_ev.c -lm

is_ev : il.h il.c is.h is.c is_ev.c
	clang -g -Wall -o is_ev il.c is.c is_ev.c -lm

vx_ev : vx.h vx.c vx_ev.c
	clang -g -Wall -o vx_ev vx.c vx_ev.c -lm

graph_ev : il.h il.c iq.h iq.c vx.h vx.c graph.h graph.c graph_ev.c
	clang -g -Wall -o graph_ev il.c iq.c vx.c graph.c graph_ev.c -lm

bfs : il.h il.c iq.h iq.c vx.h vx.c graph.h graph.c bfs.h bfs.c
	clang -g -Wall -o bfs il.c iq.c vx.c graph.c bfs.c -lm

dfs : il.h il.c is.h is.c vx.h vx.c graph.h graph.c dfs.h dfs.c
	clang -g -Wall -o dfs il.c is.c vx.c graph.c dfs.c -lm

clean :
	rm -rf a.out *_ev bfs dfs *.dSYM a.out
