#include "ast.hh"


Node::Node() {};


BodyEntry::BodyEntry() {};


Body::Body(std::vector<BodyEntry> entries) :
        entries(entries) {};


ClassDeclaration::ClassDeclaration(ClassName class_name, ClassName base_class,
                                   std::vector<MemberDeclaration> member_declarations) :
        class_name(class_name), base_class(base_class), member_declarations(member_declarations) {};


Program::Program(std::vector<ClassDeclaration> classes) :
        classes(classes) {};


Identifier::Identifier(std::string name) :
        name(name) {};


MemberDeclaration::MemberDeclaration() {};


Expression::Expression() {};


Statement::Statement() {};


VariableDeclaration::VariableDeclaration(Identifier name, Expression expression) :
        name(name), expression(expression) {};


Assignment::Assignment(Identifier name, Expression expression) :
        name(name), expression(expression) {};


WhileStatement::WhileStatement(Expression condition, Body body) :
        condition(condition), body(body) {};


ReturnStatement::ReturnStatement(Expression expression) :
        expression(expression) {};


IfStatement::IfStatement(Expression condition, Body main_body, Body alternative_body) :
        condition(condition), main_body(main_body), alternative_body(alternative_body) {};


CallableDeclaration::CallableDeclaration(std::vector<std::pair<Identifier, ClassName>> parameters, Body body) :
        parameters(parameters), body(body) {};

ConstructorDeclaration::ConstructorDeclaration(std::vector <std::pair<Identifier, ClassName>> parameters,
                                               Body body):
        parameters(parameters), body(body) {};


MethodDeclaration::MethodDeclaration( Identifier name, std::vector<std::pair<Identifier, ClassName>> parameters, ClassName return_type,
                                      Body body ) :
        CallableDeclaration(parameters, body), name(name), return_type(return_type) {};


MethodCall::MethodCall(Expression base_expression, Identifier method, std::vector<Expression> arguments) :
        base_expression(base_expression), method(method), arguments(arguments) {};


Attribute::Attribute(Expression base_expression, Identifier attribute) :
        base_expression(base_expression), attribute(attribute) {};


Primary::Primary() {};


IntegerLiteral::IntegerLiteral(int value) :
        value(value) {};


RealLiteral::RealLiteral(double value) :
        value(value) {};


BooleanLiteral::BooleanLiteral(bool value) :
        value(value) {};


ClassName::ClassName(Identifier class_name) :
        class_name(class_name) {};


SelfPointer::SelfPointer() {};

Parameter::Parameter(Identifier identifier, ClassName type) :
        value(value), type(type) {};

ParameterList::ParameterList(parameter, std::vector<Parameter>parameters):
        parameter(parameter), parameters(parameters){};

Call::Call(Identifier identifier, ArgumentList arguments)  :
        identifier(identifier), arguments(arguments){};

ArgumentList::ArgumentList():
        arguments(arguments){};



