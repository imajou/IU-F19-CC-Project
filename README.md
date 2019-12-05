# Object-oriented toy language LLVM compiler

Innopolis University, 2019 <br>
Compilers Construction <br>
Project <br>


## Team

* Gleb Petrakov
* Andrey Volkov
* Elvira Salikhova
* Alexander Solovyov


## Prerequisites

- *NIX OS (tested on Mac OS Catalina, Debian).
- Bison 3.2+.
- GCC or CLANG compiler for C++11.
- Make


## Makefile

- `make` - build all
- `make clean` - remove intermediate files
- `make reset` - remove build artifacts
- `make syntax` - build parser with Bison
- `make test` - run test run on language sources


## Structure

- `./lexer/` – handwritten lexical analyzer
- `./parser/` - Bison-based parser and AST builder
- `./test_src/` - example source files for test
