#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <llvm/IR/Module.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/LLVMContext.h>

#include <map>
#include <stack>
#include <string>

class Program;
class ClassDeclaration;

class GeneratorBlock {
public:
    llvm::BasicBlock *llvm_block;
    std::map<std::string, llvm::Value*> locals;

    GeneratorBlock(llvm::BasicBlock *llvm_block);
};


class GeneratorContext {
    llvm::Function *main_function;

    std::string file_in;

    std::stack<GeneratorBlock> blocks;

    llvm::raw_fd_ostream *out;

public:
    std::map<std::string, ClassDeclaration*> classes;
    llvm::LLVMContext llvm_context;
    llvm::Module *module;

    GeneratorContext(std::string file_in);

    void generate_code(Program& root);

    llvm::BasicBlock* get_current_block();
    void push_block(llvm::BasicBlock *block);
    void pop_block();
};


#endif
