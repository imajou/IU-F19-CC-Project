CC=g++
CFLAGS=-Wall -fpermissive -std=c++11

BISON=bison --debug

all: reset main clean

clean:
	rm -rf lexer/*.gch parser/*.gch

reset:
	rm -rf parser/syntax.cc parser/syntax.hh parser/location.hh
	rm -rf *.o *.out

syntax: parser/syntax.yy
	$(BISON) parser/syntax.yy --output=parser/syntax.cc

main: syntax
	$(CC) $(CFLAGS) parser/syntax.cc parser/syntax.hh parser/location.hh \
	    parser/driver.cc parser/driver.hh \
	    lexer/lexer.cc lexer/lexer.hh \
	    lexer/checkers.cc lexer/checkers.hh \
	    main.cc
