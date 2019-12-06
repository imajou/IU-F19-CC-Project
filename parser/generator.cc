#include "generator.hh"
#include "ast.hh"

#include <llvm/IR/Instructions.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/GenericValue.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/ADT/StringRef.h>

#include <iostream>
#include <vector>


GeneratorBlock::GeneratorBlock(llvm::BasicBlock *llvm_block) {
    this->llvm_block = llvm_block;
}


GeneratorContext::GeneratorContext(std::string file_in) {
    this->file_in = file_in;
    std::error_code EC;
    this->out = new llvm::raw_fd_ostream(llvm::StringRef(file_in + ".ll"), EC, llvm::sys::fs::F_None);
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
    root.generate_code(*this, nullptr);
    pop_block();

    llvm::ReturnInst::Create(llvm_context, entry_block);

    // Print the resulting program
    module->print(*out, nullptr);
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
