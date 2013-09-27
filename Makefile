CC=cc
CFLAGS=-lcurses
all:
	$(CC) -c syntax.c -o syntax.o $(CFLAGS)
	$(CC) -c main.c -o main.o $(CFLAGS)
	$(CC) syntax.o main.o -o syntaxhi $(CFLAGS)
clean:
	rm -f *.o syntaxhi
