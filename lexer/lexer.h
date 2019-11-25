#ifndef LEXER_H_
#define LEXER_H_

#include "../bison/syntax.h"
#include "../bison/driver.h"

yy::parser::symbol_type yylex(Driver& driver);

yy::parser::symbol_type parse_string(Driver& driver);
yy::parser::symbol_type parse_number(Driver& driver, char first);
yy::parser::symbol_type parse_identifier(Driver& driver);
yy::parser::symbol_type parse_symbol(Driver& driver, char first);

#endif  // LEXER_H_
