CFLAGS=--std=gnu99 -Wall -ggdb3
LDFLAGS=-ldl

all:
	gcc -shared -fpic $(CFLAGS) -o test.so test.c
	gcc $(CFLAGS) -o main main.c -rdynamic $(LDFLAGS)
	gcc $(CFLAGS) -o hotpatch hotpatch.c -rdynamic $(LDFLAGS)

clean:
	rm -f test.so *~ main hotpatch
