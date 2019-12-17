all: nimserver nimclient

nimserver: nimserver.o
	clang nimserver.o -o nimserver.out -lpthread

nimclient: nimclient.o
	clang nimclient.o -o nimclient.out

