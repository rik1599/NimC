all: nimserver nimclient

nimserver: nimserver.o tools.o server.o game.o
	clang nimserver.o tools.o server.o game.o -o nimserver.out -lpthread

nimclient: nimclient.o tools.o client.o game.o
	clang nimclient.o tools.o client.o game.o -o nimclient.out

nimserver.o: nimserver.c server.h game.h tools.h
	clang -c -g nimserver.c server.h game.h tools.h

nimclient.o: nimclient.c client.h game.h tools.h
	clang -c -g nimclient.c client.h game.h

server.o: server.h server.c game.h tools.h
	clang -c -g server.h game.h tools.h server.c

client.o: client.h client.c game.h tools.h
	clang -c -g client.h game.h tools.h client.c

game.o: game.h game.c
	clang -c -g game.h game.c

tools.o: tools.h tools.c
	clang -c -g tools.h tools.c

clean:
	rm -f *.out *.o *.h.gch