#ifndef AST_H_
#define AST_H_

#include <llvm/IR/Module.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <set>

#include "generator.hh"


class Node {
public:
    virtual ~Node() {};
    virtual void* generate_code(GeneratorContext& context, Node *caller) { return nullptr; };
};


class BodyEntry : public Node {
public:
    BodyEntry();
};


class Body : public Node {
public:
    std::vector<BodyEntry*> entries;

    Body(std::vector<BodyEntry*> entries);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class Identifier : public Node {
public:
    std::string name;

    Identifier(std::string name);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
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

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class ClassDeclaration : public Node {
public:
    llvm::StructType *structure;
    std::map<std::string, std::pair<unsigned, ClassDeclaration*>> attributes;

    ClassName* class_name;
    ClassName* base_class;
    std::vector<MemberDeclaration*> member_declarations;

    ClassDeclaration(ClassName* class_name, ClassName* base_class, std::vector<MemberDeclaration*> member_declarations);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class Program : public Node {
public:
    std::vector<ClassDeclaration*> classes;

    Program(std::vector<ClassDeclaration*> classes);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class Statement : public BodyEntry {
public:
    Statement();
};


class VariableDeclaration : public BodyEntry, public MemberDeclaration {
public:
    Identifier* name;
    ClassName* type;

    VariableDeclaration(Identifier* name, ClassName* type);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class Assignment : public Statement {
public:
    Identifier* name;
    Expression* expression;

    Assignment(Identifier* name, Expression* expression);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class WhileStatement : public Statement {
public:
    Expression* condition;
    Body* body;

    WhileStatement(Expression* condition, Body* body);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class ReturnStatement : public Statement {
public:
    Expression* expression;

    ReturnStatement(Expression* expression);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class IfStatement : public Statement {
public:
    Expression *condition;
    Body *main_body;
    Body *alternative_body;

    IfStatement(Expression* condition, Body* main_body);
    IfStatement(Expression* condition, Body* main_body, Body* alternative_body);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class Parameter : public Node {
public:
    Identifier* identifier;
    ClassName* type;

    Parameter(Identifier* identifier, ClassName* type);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
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

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class MethodDeclaration : public CallableDeclaration {
public:
    Body* body;
    Identifier* name;
    ClassName *return_type;
    std::vector<Parameter*> parameters;

    MethodDeclaration(Identifier* name, std::vector<Parameter*> parameters, ClassName *return_type,
                      Body* body);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class ConstructorCall : public Expression {
public:
    ClassName* class_name;
    std::vector<Expression*> arguments;

    ConstructorCall(ClassName* class_name, std::vector<Expression*> arguments);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
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

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class Attribute : public ClassExpression {
public:
    Attribute(Identifier* name);
    Attribute(Expression* base_expression, Identifier* name);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class IntegerLiteral : public Primary {
public:
    int value;

    IntegerLiteral(int value);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class RealLiteral : public Primary {
public:
    double value;

    RealLiteral(double value);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class BooleanLiteral : public Primary {
public:
    bool value;

    BooleanLiteral(bool value);

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};


class SelfPointer : public Primary {
public:
    SelfPointer();

    virtual void* generate_code(GeneratorContext& context, Node *caller);
};

#endif  // AST_H_
