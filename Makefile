CFLAGS=--std=gnu99 -Wall -ggdb3
LDFLAGS=-ldl

all:
	gcc -shared -fpic $(CFLAGS) -O0 -o test.so test.c
	gcc $(CFLAGS) -O0 -o main main.c -rdynamic $(LDFLAGS)
	gcc $(CFLAGS) -O0 -o hotpatch hotpatch.c -rdynamic $(LDFLAGS)

clean:
	rm -f test.so *~ main hotpatch
