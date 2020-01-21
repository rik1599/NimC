all: nimserver nimclient

nimserver: nimserver.o tools.o server.o game.o
	clang nimserver.o tools.o server.o game.o -o nimserver.out -lpthread

nimclient: nimclient.o tools.o client.o freadline.o game.o
	clang nimclient.o tools.o client.o freadline.o game.o -o nimclient.out

nimserver.o: nimserver.c server.h game.h
	clang -c -g nimserver.c server.h game.h -o nimserver.o

nimclient.o: nimclient.c client.h game.h
	clang -c -g nimclient.c client.h game.h -o nimclient.o

server.o: server.h server.c game.h tools.h
	clang -c -g server.h game.h tools.h game.c -o game.h

client.o: client.h client.c game.h tools.h
	clang -c -g client.h game.h tools.h client.c -o client.o

game.o: game.h game.c
	clang -c -g game.h game.c -o game.o

tools.o: tools.h tools.c
	clang -c -g tools.h tools.c -o tools.o

clean:
	rm -f *.out *.o