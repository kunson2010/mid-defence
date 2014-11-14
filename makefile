all: a.out t.out

a.out: main.c
	cc -g main.c -o a.out

t.out: main.test.c
	cc -g main.test.c -o t.out

clean:
	rm a.out
	rm t.out
