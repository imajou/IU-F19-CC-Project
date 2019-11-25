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
}

// Define a token prefix to avoid collisions
// with the standard library
%define api.token.prefix {T_}

// Keyword tokens
%token CLASS EXTENDS IS END VAR METHOD THIS
%token WHILE LOOP IF THEN ELSE RETURN 

// Symbol tokens
%token LEFT_SQUARE_BRACKET RIGHT_SQUARE_BRACKET
%token LEFT_PARENTHESIS RIGHT_PARENTHESIS
%token COMMA DOT COLON ASSIGNMENT 

// Literal tokens
%token <std::string> STRING
%token <int> INTEGER
%token <float> REAL
%token <bool> BOOLEAN

// The EOF token
%token END_OF_FILE 0

%start compilation_unit

%%

compilation_unit
    : /* empty */
    ;

%%

void yyerror(const char* message) {
    std::cerr << "Parse error: " << message << std::endl;
    std::exit(EXIT_FAILURE);
}
