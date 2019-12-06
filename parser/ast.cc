#include "ast.hh"

#include <llvm/IR/Type.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Instructions.h>

#include <iostream>


BodyEntry::BodyEntry() {};


Body::Body(std::vector<BodyEntry*> entries) :
        entries(entries) {};

void* Body::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


ClassDeclaration::ClassDeclaration(ClassName* class_name, ClassName* base_class,
                                   std::vector<MemberDeclaration*> member_declarations) :
        class_name(class_name), base_class(base_class), member_declarations(member_declarations) {};

void* ClassDeclaration::generate_code(GeneratorContext& context, Node *caller) {
    std::string &name = class_name->class_name->name;
    std::cout << "Processing class " << name << std::endl;

    if (context.classes.count(name) > 0) {
        std::cerr << "Redefinition of class" << std::endl;
        exit(1);
    }

    // Define attributes
    std::vector<llvm::Type*> members;

    if (base_class) {
        std::string &base_name = base_class->class_name->name;
        if (context.classes.count(base_name) == 0) {
            std::cerr << "Base class " << base_name << " doesn't exist" << std::endl;
            exit(1);
        }

        ClassDeclaration *base_structure = context.classes[base_name];
        members.push_back(llvm::PointerType::getUnqual(base_structure->structure));
    }

    structure = llvm::StructType::create(context.llvm_context, class_name->class_name->name);
    context.classes[name] = this;

    // Define structure
    unsigned i = bool(base_class);
    for (auto member : member_declarations) {
        if (VariableDeclaration* variable = dynamic_cast<VariableDeclaration*>(member)) {
            std::string var_name = variable->name->name;
            std::cout << "Found variable declaration: " << var_name << std::endl;

            ClassDeclaration *type = (ClassDeclaration *)variable->generate_code(context, this);
            this->attributes[var_name] = {i, type};
            members.push_back(llvm::PointerType::getUnqual(type->structure));
        }
        else if (ConstructorDeclaration* constructor = dynamic_cast<ConstructorDeclaration*>(member)) {
            std::cout << "Found constructor declaration" << std::endl;
            constructor->generate_code(context, this);
        }
        i-=-1;  // ( ͡° ͜ʖ ͡°)
    }

    structure->setBody(members);

    return nullptr;
}


Program::Program(std::vector<ClassDeclaration*> classes) :
        classes(classes) {};

void* Program::generate_code(GeneratorContext& context, Node *caller) {
    llvm::BasicBlock *block = context.get_current_block();

    for (ClassDeclaration *class_decl : classes) {
        class_decl->generate_code(context, this);
    }

    for (auto class_type : context.classes) {
        new llvm::AllocaInst(class_type.second->structure, 0, class_type.first, block);
    }

    return nullptr;
}


Identifier::Identifier(std::string name) :
        name(name) {};

void* Identifier::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


MemberDeclaration::MemberDeclaration() {};

void* MemberDeclaration::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


Expression::Expression() {};


Statement::Statement() {};


VariableDeclaration::VariableDeclaration(Identifier* name, ClassName* type) :
        name(name), type(type) {};

void* VariableDeclaration::generate_code(GeneratorContext& context, Node *caller) {
    std::string type_name = type->class_name->name;
    if (context.classes.count(type_name) == 0) {
        std::cerr << "Class name " << type_name << " is undefined" << std::endl;
        exit(1);
    }

    return context.classes[type_name];
}


Assignment::Assignment(Identifier* name, Expression* expression) :
        name(name), expression(expression) {};

void* Assignment::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


WhileStatement::WhileStatement(Expression* condition, Body* body) :
        condition(condition), body(body) {};

void* WhileStatement::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


ReturnStatement::ReturnStatement(Expression* expression) :
        expression(expression) {};

void* ReturnStatement::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


IfStatement::IfStatement(Expression* condition, Body* main_body) :
        condition(condition), main_body(main_body) {};

IfStatement::IfStatement(Expression* condition, Body* main_body, Body* alternative_body) :
        condition(condition), main_body(main_body), alternative_body(alternative_body) {};

void* IfStatement::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


CallableDeclaration::CallableDeclaration(std::vector<Parameter*> parameters, Body* body) :
        parameters(parameters), body(body) {};

Parameter::Parameter(Identifier* identifier, ClassName* type) :
        identifier(identifier), type(type) {};

void* Parameter::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }

ConstructorDeclaration::ConstructorDeclaration(std::vector<Parameter*> parameters, Body* body):
        CallableDeclaration(parameters, body) {};

void* ConstructorDeclaration::generate_code(GeneratorContext& context, Node *caller) {
    ClassDeclaration* class_decl = dynamic_cast<ClassDeclaration*>(caller);
    std::string& class_name = class_decl->class_name->class_name->name;

    std::vector<llvm::Type*> argument_types;
    std::map<std::string, unsigned> argument_order;

    argument_types.push_back(llvm::PointerType::getUnqual(class_decl->structure));

    for (auto parameter : parameters) {
        std::string& type_name = parameter->type->class_name->name;

        if (context.classes.count(type_name) == 0) {
            std::cerr << "Unknown parameter type: " << type_name << std::endl;
            exit(1);
        }

        argument_types.push_back(context.classes[type_name]->structure);
    }

    llvm::ArrayRef<llvm::Type*> args_ref(argument_types);

    llvm::Type *return_type = llvm::Type::getVoidTy(context.llvm_context);
    llvm::FunctionType *func_type = llvm::FunctionType::get(return_type, args_ref, false);
    llvm::Function *function = llvm::Function::Create(
            func_type, llvm::GlobalValue::InternalLinkage, class_name, context.module);

    llvm::BasicBlock *block = llvm::BasicBlock::Create(context.llvm_context, "entry", function, 0);
    context.push_block(block);

    new llvm::AllocaInst(llvm::PointerType::getUnqual(class_decl->structure), 0, "this", block);

    for (auto parameter : parameters) {
        std::string& parameter_name = parameter->identifier->name;
        std::string& type_name = parameter->type->class_name->name;

        new llvm::AllocaInst(context.classes[type_name]->structure, 0, parameter_name, block);
    }

    for (auto entry : body->entries) {
        entry->generate_code(context, this);
    }

    context.pop_block();

    llvm::ReturnInst::Create(context.llvm_context, block);

    return function;
}

MethodDeclaration::MethodDeclaration(Identifier* name, std::vector<Parameter*> parameters, ClassName* return_type,
                                     Body* body) :
        CallableDeclaration(parameters, body), name(name), return_type(return_type) {};

void* MethodDeclaration::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


ConstructorCall::ConstructorCall(ClassName* class_name, std::vector<Expression*> arguments) :
        class_name(class_name), arguments(arguments) {};

void* ConstructorCall::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


ClassExpression::ClassExpression(Identifier* name) :
        name(name) {};

ClassExpression::ClassExpression(Expression* base_expression, Identifier* name) :
        base_expression(base_expression), name(name) {};


MethodCall::MethodCall(Identifier* name, std::vector<Expression*> arguments) :
        ClassExpression(name), arguments(arguments) {};

MethodCall::MethodCall(Expression* base_expression, Identifier* name, std::vector<Expression*> arguments) :
        ClassExpression(base_expression, name), arguments(arguments) {};

void* MethodCall::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


Attribute::Attribute(Identifier* name) :
        ClassExpression(name) {};

Attribute::Attribute(Expression* base_expression, Identifier* name) :
        ClassExpression(base_expression, name) {};

void* Attribute::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


Primary::Primary() {};


IntegerLiteral::IntegerLiteral(int value) :
        value(value) {};

void* IntegerLiteral::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


RealLiteral::RealLiteral(double value) :
        value(value) {};

void* RealLiteral::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


BooleanLiteral::BooleanLiteral(bool value) :
        value(value) {};

void* BooleanLiteral::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }


ClassName::ClassName(Identifier* class_name) :
        class_name(class_name) {};


SelfPointer::SelfPointer() {};

void* SelfPointer::generate_code(GeneratorContext& context, Node *caller) { return nullptr; }
