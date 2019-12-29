all: nimserver nimclient

nimserver: nimserver.o tools.o server.o game.o
	clang nimserver.o tools.o server.o game.o -o nimserver.out -lpthread

nimclient: nimclient.o tools.o client.o freadline.o game.o
	clang nimclient.o tools.o client.o freadline.o game.o -o nimclient.out

%.o: %.c tools.h
	clang -c -g -o $@ $<

clean:
	rm -f *.out *.o