#include "ast.hh"

#include <llvm/IR/Constants.h>


Node::Node() {};


BodyEntry::BodyEntry() {};


Body::Body(std::vector<BodyEntry*> entries) :
        entries(entries) {};


ClassDeclaration::ClassDeclaration(ClassName* class_name, ClassName* base_class,
                                   std::vector<MemberDeclaration*> member_declarations) :
        class_name(class_name), base_class(base_class), member_declarations(member_declarations) {};


Program::Program(std::vector<ClassDeclaration*> classes) :
        classes(classes) {};

llvm::Value* Program::generate_code(GeneratorContext &context) {
    llvm::Value *two = llvm::ConstantInt::get(llvm::Type::getInt32Ty(context.llvm_context), 2);
    return two;
}


Identifier::Identifier(std::string name) :
        name(name) {};


MemberDeclaration::MemberDeclaration() {};


Expression::Expression() {};


Statement::Statement() {};


VariableDeclaration::VariableDeclaration(Identifier* name, Expression* expression) :
        name(name), expression(expression) {};


Assignment::Assignment(Identifier* name, Expression* expression) :
        name(name), expression(expression) {};


WhileStatement::WhileStatement(Expression* condition, Body* body) :
        condition(condition), body(body) {};


ReturnStatement::ReturnStatement(Expression* expression) :
        expression(expression) {};


IfStatement::IfStatement(Expression* condition, Body* main_body) :
        condition(condition), main_body(main_body) {};

IfStatement::IfStatement(Expression* condition, Body* main_body, Body* alternative_body) :
        condition(condition), main_body(main_body), alternative_body(alternative_body) {};


CallableDeclaration::CallableDeclaration(std::vector<Parameter*> parameters, Body* body) :
        parameters(parameters), body(body) {};

Parameter::Parameter(Identifier* identifier, ClassName* type) :
        identifier(identifier), type(type) {};

ConstructorDeclaration::ConstructorDeclaration(std::vector<Parameter*> parameters, Body* body):
        CallableDeclaration(parameters, body) {};

MethodDeclaration::MethodDeclaration(Identifier* name, std::vector<Parameter*> parameters, ClassName* return_type,
                                     Body* body) :
        CallableDeclaration(parameters, body), name(name), return_type(return_type) {};


ConstructorCall::ConstructorCall(ClassName* class_name, std::vector<Expression*> arguments) :
        class_name(class_name), arguments(arguments) {};


ClassExpression::ClassExpression(Identifier* name) :
        name(name) {};

ClassExpression::ClassExpression(Expression* base_expression, Identifier* name) :
        base_expression(base_expression), name(name) {};


MethodCall::MethodCall(Identifier* name, std::vector<Expression*> arguments) :
        ClassExpression(name), arguments(arguments) {};

MethodCall::MethodCall(Expression* base_expression, Identifier* name, std::vector<Expression*> arguments) :
        ClassExpression(base_expression, name), arguments(arguments) {};


Attribute::Attribute(Identifier* name) :
        ClassExpression(name) {};

Attribute::Attribute(Expression* base_expression, Identifier* name) :
        ClassExpression(base_expression, name) {};


Primary::Primary() {};


IntegerLiteral::IntegerLiteral(int value) :
        value(value) {};


RealLiteral::RealLiteral(double value) :
        value(value) {};


BooleanLiteral::BooleanLiteral(bool value) :
        value(value) {};


ClassName::ClassName(Identifier* class_name) :
        class_name(class_name) {};


SelfPointer::SelfPointer() {};
