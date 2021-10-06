CC = gcc
CFLAGS = -Wall -g 
TESTS = test_01 test_02 test_03 test_04 test_05 test_06 test_07 test_08

all : $(TESTS)

clean :
	rm -f generacion.o test test.o test.asm test_01.o test_01.asm test_02.o test_02.asm test_03.o test_03.asm test_04.o test_04.asm test_05.o test_05.asm test_06.o test_06.asm test_07.o test_07.asm test_08.o test_08.asm $(TESTS)

test_01 : generacion.o test_01.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_01 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

test_02 : generacion.o test_02.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_02 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

test_03 : generacion.o test_03.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_03 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

test_04 : generacion.o test_04.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_04 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

test_05 : generacion.o test_05.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_05 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

test_06 : generacion.o test_06.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_06 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

test_07 : generacion.o test_07.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_07 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

test_08 : generacion.o test_08.o
	$(CC) $(CFLAGS) -o $@ $@.o generacion.o
	./test_08 test.asm
	nasm -g -o test.o -f elf32 test.asm
	gcc -Wall -g -m32 -o test test.o alfalib.o

generacion.o : generacion.c generacion.h
	$(CC) $(CFLAGS) -c $<

test_01.o : test_01.c
	$(CC) $(CFLAGS) -c $<

test_02.o : test_02.c
	$(CC) $(CFLAGS) -c $<

test_03.o : test_03.c
	$(CC) $(CFLAGS) -c $<

test_04.o : test_04.c
	$(CC) $(CFLAGS) -c $<

test_05.o : test_05.c
	$(CC) $(CFLAGS) -c $<

test_06.o : test_06.c
	$(CC) $(CFLAGS) -c $<

test_07.o : test_07.c
	$(CC) $(CFLAGS) -c $<

test_08.o : test_08.c
	$(CC) $(CFLAGS) -c $<


