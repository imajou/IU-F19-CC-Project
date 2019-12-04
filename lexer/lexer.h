#ifndef LEXER_H_
#define LEXER_H_

#include <map>
#include <string>

#include "../parser/syntax.hh"
#include "../parser/driver.hh"

using namespace yy;

std::map<std::string, int> keywords = {
    {"true", parser::token::T_TRUE},
    {"false", parser::token::T_FALSE},
    {"class", parser::token::T_CLASS},
    {"extends", parser::token::T_EXTENDS},
    {"is", parser::token::T_IS},
    {"end", parser::token::T_END},
    {"var", parser::token::T_VAR},
    {"method", parser::token::T_METHOD},
    {"this", parser::token::T_THIS},
    {"while", parser::token::T_WHILE},
    {"loop", parser::token::T_LOOP},
    {"if", parser::token::T_IF},
    {"then", parser::token::T_THEN},
    {"else", parser::token::T_ELSE},
    {"return", parser::token::T_RETURN}
};

parser::symbol_type yylex(Driver& driver);

parser::symbol_type parse_string(Driver& driver);
parser::symbol_type parse_number(Driver& driver);
parser::symbol_type parse_identifier(Driver& driver);
parser::symbol_type parse_symbol(Driver& driver);

#endif  // LEXER_H_
