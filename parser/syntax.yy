%require "3.2"
%language "c++"

// Request header file generation
%defines

// Request locations tracking
%locations

%define api.token.constructor
%define api.value.type variant

%code requires {
    #include <vector>
    #include <string>

    class Driver;
    class ClassDeclaration;
    class ClassName;
    class MemberDeclaration;
    class VariableDeclaration;
    class MethodDeclaration;
    class ConstructorDeclaration;
    class Parameter;
    class Body;
    class BodyEntry;
    class Assignment;
    class Primary;
    class Identifier;
    class IntegerLiteral;
    class RealLiteral;
    class BooleanLiteral;
    class SelfPointer;
    class IfStatement;
    class WhileStatement;
    class ReturnStatement;
    class ConstructorCall;
    class Expression;
    class ClassExpression;
    class Attribute;
    class MethodCall;
}

// The parsing context
%param { Driver& driver }


%code {
    #include "driver.hh"
  	#include "ast.hh"

  	Program *program_root;
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

%type <std::vector<ClassDeclaration*>> ClassDeclarations
%type <ClassDeclaration*> AssertClass
%type <ClassName*> Extension
%type <ClassName*> ClassName
%type <std::vector<MemberDeclaration*>> ClassBody
%type <std::vector<MemberDeclaration*>> ClassMembers
%type <MemberDeclaration*> ClassMember
%type <VariableDeclaration*> VariableDeclaration
%type <MethodDeclaration*> MethodDeclaration
%type <ConstructorDeclaration*> ConstructorDeclaration
%type <ClassName*> ReturnType
%type <std::vector<Parameter*>> Parameters
%type <std::vector<Parameter*>> ParameterList
%type <Parameter*> Parameter
%type <Body*> MethodBody
%type <Body*> Body
%type <std::vector<BodyEntry*>> BodyMembers
%type <BodyEntry*> BodyMember
%type <Assignment*> ValueAssignment
%type <Primary*> Primary
%type <Identifier*> Identifier
%type <IntegerLiteral*> IntegerLiteral
%type <RealLiteral*> RealLiteral
%type <BooleanLiteral*> BooleanVal
%type <SelfPointer*> ThisLiteral
%type <IfStatement*> IfStatement
%type <WhileStatement*> WhileStatement
%type <ReturnStatement*> ReturnStatement
%type <ConstructorCall*> ConstructorCall
%type <ClassExpression*> ClassExpressions
%type <ClassExpression*> ClassExpression
%type <std::vector<Expression*>> Call
%type <std::vector<Expression*>> ArgumentList
%type <Expression*> Expression

%start CompilationUnit

%%

CompilationUnit
    : ClassDeclarations { program_root = new Program($1); }
    ;


ClassDeclarations
    : /* empty */ { $$ = std::vector<ClassDeclaration*>(); }
    | AssertClass ClassDeclarations  { $2.push_back($1); $$ = $2; }
    ;

AssertClass
    : CLASS ClassName Extension ClassBody { $$ = new ClassDeclaration($2, $3, $4); }
    ;

Extension
    : /* empty */ { $$ = nullptr; }
    | EXTENDS ClassName { $$ = $2; }
    ;

ClassName
    : Identifier { $$ = new ClassName($1); }
    ;

ClassBody
    : IS ClassMembers END { $$ = $2; }
    ;

ClassMembers
    : /* empty */ { $$ = std::vector<MemberDeclaration*>(); }
    | ClassMember ClassMembers { $2.push_back($1); $$ = $2; }
    ;

ClassMember
    : VariableDeclaration { $$ = $1; }
    | MethodDeclaration { $$ = $1; }
    | ConstructorDeclaration { $$ = $1; }
    ;

VariableDeclaration
    : VAR Identifier COLON Expression { $$ = new VariableDeclaration($2, $4); }
    ;


MethodDeclaration
    : METHOD Identifier Parameters ReturnType MethodBody { $$ = new MethodDeclaration($2, $3, $4, $5); }
    ;

ConstructorDeclaration
    : THIS Parameters MethodBody { $$ = new ConstructorDeclaration($2, $3); }
    ;

ReturnType
    : /* empty */ { $$ = nullptr; }
    | COLON ClassName { $$ = $2; }
    ;

Parameters
    : LEFT_PARENTHESIS               RIGHT_PARENTHESIS { $$ = std::vector<Parameter*>(); }
    | LEFT_PARENTHESIS ParameterList RIGHT_PARENTHESIS { $$ = $2; }
    ;

ParameterList
    :                     Parameter { $$ = std::vector<Parameter*>{$1}; }
    | ParameterList COMMA Parameter { $1.push_back($3); $$ = $1; }
    ;

Parameter
    : Identifier COLON ClassName { $$ = new Parameter($1, $3); }
    ;


MethodBody
    : IS Body END { $$ = $2; }
    ;


Body
    : BodyMembers { $$ = new Body($1); }
    ;


BodyMembers
    : /* empty */ { $$ = std::vector<BodyEntry*>(); }
    | BodyMember BodyMembers { $2.push_back($1); $$ = $2; }
    ;

BodyMember
    : ValueAssignment { $$ = $1; }
    | IfStatement { $$ = $1; }
    | WhileStatement { $$ = $1; }
    | ReturnStatement { $$ = $1; }
    | Expression { $$ = $1; }
    | VariableDeclaration { $$ = $1; }
    ;

ValueAssignment
    : Identifier ASSIGNMENT Expression { $$ = new Assignment($1, $3); }
    ;


Primary
    : ClassName { $$ = $1; }
    | IntegerLiteral { $$ = $1; }
    | RealLiteral { $$ = $1; }
    | BooleanVal { $$ = $1; }
    | ThisLiteral { $$ = $1; }
    ;

ThisLiteral
    : THIS { $$ = new SelfPointer(); }
    ;

IfStatement
    : IF Expression THEN Body END { $$ = new IfStatement($2, $4); }
    | IF Expression THEN Body ELSE Body END { $$ = new IfStatement($2, $4, $6); }
    ;

WhileStatement
    : WHILE Expression LOOP Body END { $$ = new WhileStatement($2, $4); }
    ;

ReturnStatement
    : RETURN Expression { $$ = new ReturnStatement($2); }
    | RETURN { $$ = nullptr; }
    ;

ConstructorCall
    : ClassName Call { $$ = new ConstructorCall($1, $2); }
    ;

ClassExpressions
    : ClassExpression { $$ = $1; }
    | ClassExpressions DOT ClassExpression { $3->base_expression = $1; $$ = $3; }
    ;

ClassExpression
    : Identifier { $$ = new Attribute($1); }
    | Identifier Call { $$ = new MethodCall($1, $2); }
    ;

Call
    : LEFT_PARENTHESIS              RIGHT_PARENTHESIS { $$ = std::vector<Expression*>(); }
    | LEFT_PARENTHESIS ArgumentList RIGHT_PARENTHESIS { $$ = $2; }
    ;


ArgumentList
    :                    Expression { $$ = std::vector<Expression*>{$1}; }
    | ArgumentList COMMA Expression { $1.push_back($3); $$ = $1; }
    ;


Expression
    : Primary { $$ = $1; }
    | Primary DOT ClassExpressions { $3->base_expression = $1; $$ = $3; }
    | ConstructorCall { $$ = $1; }
    ;

BooleanVal
    : TRUE { $$ = new BooleanLiteral(true); }
    | FALSE { $$ = new BooleanLiteral(false); }
    ;

IntegerLiteral
    : INTEGER { $$ = new IntegerLiteral($1); }
    ;

RealLiteral
    : REAL { $$ = new RealLiteral($1); }
    ;

Identifier
    : IDENTIFIER { $$ = new Identifier($1); }
    ;

%%

void yy::parser::error(const location_type& location, const std::string& message) {
    std::cerr << "Parse error (at " << location << "): " << message << std::endl;
    std::exit(EXIT_FAILURE);
}
