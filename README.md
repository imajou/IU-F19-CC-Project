# F19 Compilers Construction Project

## Team

* Gleb Petrakov
* Andrey Volkov
* Elvira Salikhova
* Alexander Solovyov

## Installation

The first step is to generate a source code from yacc-file.
This can be done with the following command:

```bash
# Bison binaries should be installaed in the system
bison syntax.yy --output=syntax.cc --defines=syntax.h
```
