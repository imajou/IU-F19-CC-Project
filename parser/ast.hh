#ifndef AST_H_
#define AST_H_

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>


class Node {
public:
    Node();
};


class BodyEntry : public Node {
public:
    BodyEntry();
};


class Body : public Node {
public:
    std::vector<BodyEntry> entries;

    Body(std::vector<BodyEntry> entries);
};


class Identifier : public Node {
public:
    std::string name;

    Identifier(std::string name);
};


class Expression : public Node {
public:
    Expression();
};


class Primary : public Expression {
public:
    Primary();
};


class ClassName : public Primary {
public:
    Identifier class_name;

    ClassName(Identifier class_name);
};


class MemberDeclaration : public Node {
public:
    MemberDeclaration();
};


class ClassDeclaration : public Node {
public:
    ClassName class_name;
    ClassName base_class;
    std::vector<MemberDeclaration> member_declarations;

    ClassDeclaration(ClassName class_name, ClassName base_class, std::vector<MemberDeclaration> member_declarations);
};


class Program : public Node {
public:
    std::vector<ClassDeclaration> classes;

    Program(std::vector<ClassDeclaration> classes);
};


class Statement : public BodyEntry {
public:
    Statement();
};


class VariableDeclaration : public BodyEntry, public MemberDeclaration {
public:
    Identifier name;
    Expression expression;

    VariableDeclaration(Identifier name, Expression expression);
};


class Assignment : public Statement {
public:
    Identifier name;
    Expression expression;

    Assignment(Identifier name, Expression expression);
};


class WhileStatement : public Statement {
public:
    Expression condition;
    Body body;

    WhileStatement(Expression condition, Body body);
};


class ReturnStatement : public Statement {
public:
    Expression expression;

    ReturnStatement(Expression expression);
};


class IfStatement : public Statement {
public:
    Expression condition;
    Body main_body;
    Body alternative_body;

    IfStatement(Expression condition, Body main_body, Body alternative_body);
};


class CallableDeclaration : public MemberDeclaration {
public:
    std::vector<std::pair<Identifier, ClassName>> parameters;
    Body body;

    CallableDeclaration(std::vector<std::pair<Identifier, ClassName>> parameters, Body body);
};


class ConstructorDeclaration : public CallableDeclaration {
public:
};


class MethodDeclaration : public CallableDeclaration {
public:
    Identifier name;
    ClassName return_type;

    MethodDeclaration(std::vector<std::pair<Identifier, ClassName>> parameters, Body body, Identifier name,
                      ClassName return_type);
};


class MethodCall : public Expression {
public:
    Expression base_expression;
    Identifier method;
    std::vector<Expression> arguments;

    MethodCall(Expression base_expression, Identifier method, std::vector<Expression> arguments);
};


class Attribute : public Expression {
public:
    Expression base_expression;
    Identifier attribute;

    Attribute(Expression base_expression, Identifier attribute);
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
