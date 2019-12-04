CC=g++
CFLAGS=-Wall -std=c++11

BISON=bison --debug

all: reset main clean

clean:
	rm -rf lexer/*.gch parser/*.gch

reset:
	rm -rf parser/syntax.cc parser/syntax.hh parser/location.hh
	rm -rf *.o *.out

parser/syntax.cc: parser/syntax.yy
	$(BISON) parser/syntax.yy --output=parser/syntax.cc

main: parser/syntax.cc parser/syntax.hh parser/driver.cc lexer/checkers.cc lexer/lexer.cc
	$(CC) $(CFLAGS) parser/syntax.cc parser/syntax.hh parser/location.hh \
	    parser/driver.cc parser/driver.hh \
	    lexer/checkers.cc lexer/lexer.cc \
	    main.cc
