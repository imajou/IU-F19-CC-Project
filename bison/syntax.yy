%require "3.2"
%language "c++"

%define api.token.constructor
%define api.value.type variant

%code requires {
    #include <string>
    class Driver;
}

// The parsing context
%param { Driver& driver }

// Request locations tracking
%locations

%code {
    #include "driver.h"
    yy::parser::symbol_type yylex(Driver& driver);
}

// Define a token prefix to avoid collisions
// with the standard library
%define api.token.prefix {T_}

// Keyword tokens
%token CLASS EXTENDS IS END VAR METHOD THIS
%token WHILE LOOP IF THEN ELSE RETURN 

// Identifier
%token <std::string> IDENTIFIER

// Symbol tokens
%token LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%token LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token COMMA DOT COLON ASSIGNMENT 

// Literal tokens
%token <std::string> STRING
%token <int> INTEGER
%token <float> REAL
%token <bool> BOOLEAN
%token TRUE FALSE

// The EOF token
%token END_OF_FILE 0

%start compilation_unit

%%

compilation_unit
    : VAR INTEGER STRING TRUE
    ;

%%

void yy::parser::error(const location_type& location, const std::string& message) {
    std::cerr << "Parse error (at " << location << "): " << message << std::endl;
    std::exit(EXIT_FAILURE);
}
