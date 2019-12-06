#ifndef AST_H_
#define AST_H_

#include <llvm/IR/Module.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

#include "generator.hh"


class Node {
public:
    Node();

    // virtual llvm::Value* generate_code(GeneratorContext& context);
};


class BodyEntry : public Node {
public:
    BodyEntry();

    // llvm::Value* generage_code();
};


class Body : public Node {
public:
    std::vector<BodyEntry*> entries;

    Body(std::vector<BodyEntry*> entries);
};


class Identifier : public Node {
public:
    std::string name;

    Identifier(std::string name);
};


class Expression : public BodyEntry {
public:
    Expression();
};


class Primary : public Expression {
public:
    Primary();
};


class ClassName : public Primary {
public:
    Identifier* class_name;

    ClassName(Identifier* class_name);
};


class MemberDeclaration : public Node {
public:
    MemberDeclaration();
};


class ClassDeclaration : public Node {
public:
    ClassName* class_name;
    ClassName* base_class;
    std::vector<MemberDeclaration*> member_declarations;

    ClassDeclaration(ClassName* class_name, ClassName* base_class, std::vector<MemberDeclaration*> member_declarations);

    llvm::Value *generate_code(GeneratorContext &context);
};


class Program : public Node {
public:
    std::vector<ClassDeclaration*> classes;

    Program(std::vector<ClassDeclaration*> classes);
    llvm::Value* generate_code(GeneratorContext &context);
};


class Statement : public BodyEntry {
public:
    Statement();
};


class VariableDeclaration : public BodyEntry, public MemberDeclaration {
public:
    Identifier* name;
    Expression* expression;

    VariableDeclaration(Identifier* name, Expression* expression);
};


class Assignment : public Statement {
public:
    Identifier* name;
    Expression* expression;

    Assignment(Identifier* name, Expression* expression);
};


class WhileStatement : public Statement {
public:
    Expression* condition;
    Body* body;

    WhileStatement(Expression* condition, Body* body);
};


class ReturnStatement : public Statement {
public:
    Expression* expression;

    ReturnStatement(Expression* expression);
};


class IfStatement : public Statement {
public:
    Expression *condition;
    Body *main_body;
    Body *alternative_body;

    IfStatement(Expression* condition, Body* main_body);
    IfStatement(Expression* condition, Body* main_body, Body* alternative_body);
};


class Parameter : public Node {
public:
    Identifier* identifier;
    ClassName* type;
    Parameter(Identifier* identifier, ClassName* type);
};


class CallableDeclaration : public MemberDeclaration {
public:
    std::vector<Parameter*> parameters;
    Body* body;

    CallableDeclaration(std::vector<Parameter*> parameters, Body* body);
};


class ConstructorDeclaration : public CallableDeclaration {
public:
    ConstructorDeclaration(std::vector<Parameter*> parameters, Body* body);
};


class MethodDeclaration : public CallableDeclaration {
public:
    Body* body;
    Identifier* name;
    ClassName *return_type;
    std::vector<Parameter*> parameters;

    MethodDeclaration(Identifier* name, std::vector<Parameter*> parameters, ClassName *return_type,
                      Body* body);
};


class ConstructorCall : public Expression {
public:
    ClassName* class_name;
    std::vector<Expression*> arguments;

    ConstructorCall(ClassName* class_name, std::vector<Expression*> arguments);
};


class ClassExpression : public Expression {
public:
    Expression* base_expression;
    Identifier* name;

    ClassExpression(Identifier* name);
    ClassExpression(Expression* base_expression, Identifier* name);
};


class MethodCall : public ClassExpression {
public:
    std::vector<Expression*> arguments;

    MethodCall(Identifier* name, std::vector<Expression*> arguments);
    MethodCall(Expression* base_expression, Identifier* name, std::vector<Expression*> arguments);
};


class Attribute : public ClassExpression {
public:
    Attribute(Identifier* name);
    Attribute(Expression* base_expression, Identifier* name);
};


class IntegerLiteral : public Primary {
public:
    int value;

    IntegerLiteral(int value);
};


class RealLiteral : public Primary {
public:
    double value;

    RealLiteral(double value);
};


class BooleanLiteral : public Primary {
public:
    bool value;

    BooleanLiteral(bool value);
};


class SelfPointer : public Primary {
public:
    SelfPointer();
};

#endif  // AST_H_
