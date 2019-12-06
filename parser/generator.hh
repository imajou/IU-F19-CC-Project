#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>

#include <map>
#include <stack>

class Program;

class GeneratorBlock {
public:
    llvm::BasicBlock *llvm_block;
    std::map<std::string, llvm::Value*> locals;

    GeneratorBlock(llvm::BasicBlock *llvm_block);
};


class GeneratorContext {
    llvm::Function *main_function;
    std::unique_ptr<llvm::Module> module_owner;

    std::stack<GeneratorBlock> blocks;

public:
    llvm::LLVMContext llvm_context;
    llvm::Module *module;

    GeneratorContext();

    void generate_code(Program& root);

    llvm::BasicBlock* get_current_block();
    void push_block(llvm::BasicBlock *block);
    void pop_block();
};


#endif
