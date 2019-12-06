#include "generator.hh"
#include "ast.hh"

#include <llvm/IR/DerivedTypes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>

#include <iostream>
#include <vector>


GeneratorBlock::GeneratorBlock(llvm::BasicBlock *llvm_block) {
    this->llvm_block = llvm_block;
}


GeneratorContext::GeneratorContext() {
    module = new llvm::Module("main", llvm_context);
}


void GeneratorContext::generate_code(Program &root) {
    // Define the main function type
    llvm::FunctionType *main_type = llvm::FunctionType::get(
        llvm::Type::getVoidTy(llvm_context), false
    );
    // Define main function
    main_function = llvm::Function::Create(
        main_type, llvm::Function::ExternalLinkage, "main", module
    );
    // Define entry block
    llvm::BasicBlock *entry_block = llvm::BasicBlock::Create(
        llvm_context, "entry", main_function, 0
    );

    push_block(entry_block);
    root.generate_code(*this);
    pop_block();

    // Print the resulting program
    std::cout << "\n-----------------\n" << std::endl;
    module->print(llvm::errs(), nullptr);
}


llvm::BasicBlock* GeneratorContext::get_current_block() {
    return blocks.top().llvm_block;
}


void GeneratorContext::push_block(llvm::BasicBlock *block) {
    blocks.push(GeneratorBlock(block));
}


void GeneratorContext::pop_block() {
    blocks.pop();
}
