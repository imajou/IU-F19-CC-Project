CC=g++
CFLAGS=-Wall -std=c++11

BISON=bison --debug

all: main

bison/syntax.cc: bison/syntax.yy
	$(BISON) bison/syntax.yy --output=bison/syntax.cc --defines=bison/syntax.h

main: main.cc bison/driver.cc bison/syntax.cc bison/syntax.h lexer/checkers.cc lexer/lexer.cc
	$(CC) $(CFLAGS) main.cc bison/driver.cc bison/syntax.cc bison/syntax.h lexer/checkers.cc lexer/lexer.cc
