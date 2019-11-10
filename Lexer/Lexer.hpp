#pragma once

#include <string>
#include <iostream>
#include "Token.hpp"
#include "SymbolTable.hpp"

class Lexer {
private:
    std::string src;
    std::string::iterator src_iter;

public:
    SymbolTable symbol_table;

public:
    Lexer(std::string src);
    Token next();

private:
    void createSymbolTable();

    bool isDigit(char c);
    bool isHexDigit(char c);
    bool isLetter(char c);
    bool isOtherSymbol(char c);

    void parseComment();
    Token parseString();
    Token parseOtherSymbol();
    Token parseNumber();
    Token parseIdentifier();
};
