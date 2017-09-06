CFLAGS=-g -O0 -fPIC
CC=gcc
all: check

clean: 
	rm -rf libmalloc.so malloc.o test1 t-test1 test1.o t-test1.o calloc.o free.o helpers.o realloc.o 
	rm -rf calloc.h.gch free.h.gch helpers.h.gch realloc.h.gch

build:  clean
	gcc --shared -fPIC malloc.c malloc.h -o libmalloc.so -pthread
	gcc t-test1.c -o t-test1 -pthread

libmalloc.so: malloc.o calloc.o helpers.o free.o realloc.o
	$(CC) $(CFLAGS) -shared -o libmalloc.so malloc.o helpers.o free.o realloc.o calloc.o -lpthread
	$(CC) $(CFLAGS) -shared -c helpers.c helpers.h -pthread
	$(CC) $(CFLAGS) -shared -c free.c free.h -pthread
	$(CC) $(CFLAGS) -shared -c realloc.c realloc.h -pthread
	$(CC) $(CFLAGS) -shared -c calloc.c calloc.h -pthread

test1: test1.o
	$(CC) $(CFLAGS) $< -o $@

t-test1: t-test1.o
	$(CC) $(CFLAGS) $< -o $@ -lpthread

malloc.o: malloc.c malloc.h
	$(CC) -c malloc.c malloc.h -pthread $(CFLAGS)

free.o: free.c free.h
	$(CC) -c free.c free.h -pthread $(CFLAGS)

helpers.o: helpers.c helpers.h
	$(CC) -c helpers.c helpers.h -pthread $(CFLAGS)

realloc.o: realloc.c realloc.h
	$(CC) -c realloc.c realloc.h -pthread $(CFLAGS)

calloc.o: calloc.c calloc.h
	$(CC) -c calloc.c calloc.h -pthread $(CFLAGS)

check: libmalloc.so t-test1
	LD_PRELOAD=`pwd`/libmalloc.so ./t-test1
