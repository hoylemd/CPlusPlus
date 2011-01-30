all : Terminal.o

Terminal.o : Terminal.cpp Terminal.h
	g++ -c Terminal.cpp -o Terminal.o -pedantic -Wall
	
test : main.cpp Terminal.o
	g++ main.cpp Terminal.o -o TestTerminal -pedantic -Wall
	./TestTerminal > test.log
    
clean :
	rm -f Terminal TestTerminal *~ *.o *.log