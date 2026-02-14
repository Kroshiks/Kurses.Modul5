all: integ

integ: main.o numerics.o funcs.o
	gcc -o integ main.o numerics.o funcs.o -lm

main.o: main.c numerics.h funcs.h
	gcc -c -o main.o main.c

numerics.o: numerics.c numerics.h funcs.h
	gcc -c -o numerics.o numerics.c

funcs.o: funcs.c funcs.h
	gcc -c -o funcs.o funcs.c

clean:
	del *.o integ.exe
