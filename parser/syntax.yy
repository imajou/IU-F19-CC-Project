, $5, $3; %%require "3.2"
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
  	#include "ast.hh"
  	Node *programRoot;
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
    : ClassDeclarations {programRoot = $1;}
    ;


ClassDeclarations
    : /* empty */ {$$ = nullptr}
    | AssertClass ClassDeclarations  { std::vector<ClassDeclaration> a; $$ = Program(a.push_back($1);)}
    ;

AssertClass
    : CLASS IDENTIFIER Extension ClassBody { $$ = ClassDeclaration($2, $3, $4);}
    ;

Extension
    : /* empty */
    | EXTENDS Type {$$ = ClassName($2);}
    ;

ClassBody
    : IS              END {$$ = nullptr}
    | IS ClassMembers END {$$ = $2;}
    ;

ClassMembers
    :              ClassMember {ClassDeclaration c = ClassDeclaration() ; $$ = c.member_declarations.push_back($<ClassMember>1)}
    | ClassMembers ClassMember {$1.push_back($<ClassMember>2)}
    ;

ClassMember
    : VariableDeclaration {$$ = $1}
    | MethodDeclaration {$$ = $1}
    | ConstructorDeclaration {$$ = $1}
    ;

VariableDeclaration
    : VAR IDENTIFIER COLON Expression {$$ = VariableDeclaration($2, $4);}
    ;


MethodDeclaration
    : METHOD IDENTIFIER Parameters ReturnType MethodBody {$$ = MethodDeclaration($2, $3, $4);}
    ;

ConstructorDeclaration
    : THIS Parameters MethodBody {$$ = ConstructorDeclaration($2, $3)}
    ;

ReturnType
    : /* empty */
    | COLON Type {$$ = $1;}
    ;

Parameters
    : LEFT_PARENTHESIS               RIGHT_PARENTHESIS {$$ = nullptr}
    | LEFT_PARENTHESIS ParameterList RIGHT_PARENTHESIS {$$ = $2}
    ;

ParameterList
    :                     Parameter {$$ = ParameterList($1);}
    | ParameterList COMMA Parameter {$$ = ParameterList($1, $3);}
    ;

Parameter
    : IDENTIFIER COLON Type {$$ = Parameter($1, $3);}
    ;


MethodBody
    : IS               END {$$ = nullptr}
    | IS MethodMembers END {$$ = $1}
    ;


MethodMembers
    :               MethodMember {Body b = Body(); $$ = b.entries.push_back($<MethodMember>1)}
    | MethodMembers MethodMember {$1.push_back($<MethodMember>2)}
    ;

MethodMember
    : ValueAssignment {$$ = $1}
    | IfMethodMember {$$ = $1}
    | WhileMethodMember {$$ = $1}
    | ReturnMethodMember {$$ = $1}
    | Expression {$$ = $1}
    | VariableDeclaration {$$ = $1}
    ;

ValueAssignment
    : IDENTIFIER ASSIGNMENT Expression {$$ = Assignment($1, $3);}
    ;


Primary
    : IDENTIFIER
    | IntegerLiteral {$$ = IntegerLiteral($1);}
    | RealLiteral {$$ = RealLiteral($1);}
    | BooleanVal {$$ = BooleanLiteral($1);}
    | THIS
    ;

Type
    : IDENTIFIER
    | IntegerLiteral {$$ = IntegerLiteral($1);}
    | RealLiteral {$$ = RealLiteral($1);}
    | BooleanVal {$$ = BooleanVal($1);}
    ;

IfMethodMember
   : IF Expression THEN MethodMember END {$$ = IfStatement($2, $4);}
   | IF Expression THEN MethodMember ELSE MethodMember END {$$ = IfStatement($2, $4, $6);}
   ;

WhileMethodMember
   : WHILE Expression LOOP MethodMember END {$$ = WhileStatement($2, $4);}
   ;

ReturnMethodMember
   : RETURN Expression {$$ = ReturnStatement($2);}
   | RETURN
   ;

ConstructorCall
   : IDENTIFIER LEFT_PARENTHESIS              RIGHT_PARENTHESIS {$$ = nullptr;}
   | IDENTIFIER LEFT_PARENTHESIS ArgumentList RIGHT_PARENTHESIS {$$ = Call($1, $3;}
   ;

MethodCalls
   : MethodCall {$$ = $1;}
   | MethodCalls DOT MethodCall {$$ = MethodCalls($1, $3);}
   ;

MethodCall
   : IDENTIFIER LEFT_PARENTHESIS              RIGHT_PARENTHESIS {$$ = nullptr;}
   | IDENTIFIER LEFT_PARENTHESIS ArgumentList RIGHT_PARENTHESIS {$$ = Call($1, $3;}
   ;

ArgumentList
   :                    Expression {$$ =$1;}
   | ArgumentList COMMA Expression {$$ = ArgumentList($1, $3)}
   ;


Expression
   : Primary DOT MethodCalls
   | Primary
   | ConstructorCall
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
