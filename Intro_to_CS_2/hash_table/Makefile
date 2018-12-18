# Makefile for CS152 lab4 Winter 2018

hset: hset.[hc] bucket.c main.c
	clang -lm -g -Wall -o hset bucket.c hset.c main.c

.PHONY clean:
	rm -rf hset *.dSYM
