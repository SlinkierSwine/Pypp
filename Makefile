all: pypp

pypp: lexer.o pypp.o
	g++ pypp.o lexer.o -o pypp

pypp.o: pypp.cpp
	g++ -c pypp.cpp

lexer.o: lexer.cpp
	g++ -c lexer.cpp lexer.hpp

clean:
	rm -rf *.o output

