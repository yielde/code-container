abstime: abstime.o abstimelib.o
	gcc -o abstime abstime.o abstimelib.o

abstime.o: abstime.h abstime.c
	gcc -c abstime.c

abstimelib.o: abstime.h abstimelib.c
	gcc -c abstimelib.c

clean:
	rm abstime abstime.o abstimelib.o