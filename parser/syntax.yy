%require "3.2"
%language "c++"

// Request header file generation
%defines

// Request locations tracking
%locations

%define api.token.constructor
%define api.value.type variant

%code requires {
    #include <string>
    class Driver;
}

// The parsing context
%param { Driver& driver }


%code {
    #include "driver.hh"
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
    : ClassDeclarations
    ;


ClassDeclarations
    : /* empty */
    | AssertClass ClassDeclarations
    ;

AssertClass
    : CLASS IDENTIFIER Extension ClassBody
    ;

Extension
    : /* empty */
    | EXTENDS Type
    ;

ClassBody
    : IS              END
    | IS ClassMembers END
    ;

ClassMembers
    :              ClassMember
    | ClassMembers ClassMember
    ;

ClassMember
    : VariableDeclaration
    | MethodDeclaration
    | ConstructorDeclaration
    ;

VariableDeclaration
    : VAR IDENTIFIER COLON Expression
    ;


MethodDeclaration
    : METHOD IDENTIFIER Parameters ReturnType MethodBody
    ;

ConstructorDeclaration
    : THIS Parameters IS MethodBody END
    ;

ReturnType
    : /* empty */
    | COLON Type
    ;

Parameters
    : LEFT_PARENTHESIS               RIGHT_PARENTHESIS
    | LEFT_PARENTHESIS ParameterList RIGHT_PARENTHESIS
    ;

ParameterList
    :                     Parameter
    | ParameterList COMMA Parameter
    ;

Parameter
    : Type IDENTIFIER
    ;


MethodBody
    : IS               END
    | IS MethodMembers END
    ;


MethodMembers
    :               MethodMember
    | MethodMembers MethodMember
    ;

MethodMember
    : ASSIGNMENTment
    | IfMethodMember
    | WhileMethodMember
    | ReturnMethodMember
    | MethodCall
    | VariableDeclaration
    ;

ASSIGNMENTment
    : IDENTIFIER ASSIGNMENT Expression
    ;


Primary
    : IDENTIFIER
    | IntegerLiteral
    | RealLiteral
    | BooleanVal
    | THIS
    ;

Type
    : IDENTIFIER
    | IntegerLiteral
    | RealLiteral
    | BooleanVal
    ;

IfMethodMember
   : IF Expression THEN MethodMember END
   | IF Expression THEN MethodMember ELSE MethodMember END
   ;

WhileMethodMember
   : WHILE Expression LOOP MethodMember END
   ;

ReturnMethodMember
   : RETURN Expression
   | RETURN
   ;

MethodCalls
   : MethodCall
   | MethodCalls DOT MethodCall
   ;

MethodCall
   : IDENTIFIER LEFT_PARENTHESIS              RIGHT_PARENTHESIS
   | IDENTIFIER LEFT_PARENTHESIS ArgumentList RIGHT_PARENTHESIS
   ;

ArgumentList
   :                    Expression
   | ArgumentList COMMA Expression
   ;


Expression
   : Primary DOT MethodCalls
   | Primary
   ;

BooleanVal
   : TRUE
   | FALSE
   ;

IntegerLiteral
   : INTEGER
   ;

RealLiteral
   : REAL
   ;

%%

void yy::parser::error(const location_type& location, const std::string& message) {
    std::cerr << "Parse error (at " << location << "): " << message << std::endl;
    std::exit(EXIT_FAILURE);
}
