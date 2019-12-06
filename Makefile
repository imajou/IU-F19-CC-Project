.PHONY=all

CC := g++
CFLAGS := -Wall -fpermissive -std=c++11

LLVM_CFLAGS := $(shell llvm-config --cxxflags)
LLVM_LIBS := $(shell llvm-config --ldflags --libs)

BISON := bison --debug

BINARY := compiler

all: reset syntax compile post-compile link clean

clean:
	rm -rf lexer/*.gch parser/*.gch

reset:
	rm -rf parser/syntax.cc parser/syntax.hh parser/location.hh
	rm -rf **/*.o **/*.out

syntax: parser/syntax.yy
	$(BISON) parser/syntax.yy --output=parser/syntax.cc

compile:
	$(CC) $(CFLAGS) $(LLVM_CFLAGS) -fexceptions -c -g \
		parser/syntax.cc parser/syntax.hh parser/location.hh \
	    parser/driver.cc parser/driver.hh \
	    lexer/lexer.cc lexer/lexer.hh \
	    lexer/checkers.cc lexer/checkers.hh \
	    parser/ast.cc parser/ast.hh \
	    parser/generator.cc parser/generator.hh \
	    main.cc

post-compile:
	mkdir -p out && mv *.o out/

link:
	$(CC) out/*.o $(LLVM_LIBS) -lpthread -ltinfo -o $(BINARY)

include Makefile.test
