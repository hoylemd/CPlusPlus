all : List

list : src/list.c src/list.h
	cc src/list.cpp -c -o list.o -Wall -pedantic

test : test/listTest.c src/list.h list
	cc test/listTest.cpp list.o -c -o bin/Listtest -Wall -pedantic
	bin/Listtest

clean :
	rm -f bin/* src/*~ *~