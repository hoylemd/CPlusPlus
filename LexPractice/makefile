all : scanner

scanner : lex.yy.c util.cpp token.cpp stack.cpp main.cpp
	g++ lex.yy.c util.cpp token.cpp stack.cpp main.cpp -o scanner -pedantic

lex.yy.c : warmup.l
	flex warmup.l
	
clean : 
	rm -f lex.yy.c Warmup *~ *.o