CC = gcc
CFLAGS = -Wall -g 

all : main

clean :
	rm -f main.o generacion.o $(EXE)

main : generacion.o main.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o

generacion.o : generacion.c generacion.h
	$(CC) $(CFLAGS) -c $<

main.o : main.c
	$(CC) $(CFLAGS) -c $<

