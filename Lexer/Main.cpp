#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Token.hpp"
#include "SymbolTable.hpp"
#include "Lexer.hpp"


int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Invalid number of args" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <path_to_source>" << std::endl;
        return 1;
    }

    std::ifstream src_file(argv[1]);
    std::stringstream buffer;

    buffer << src_file.rdbuf();

    Lexer lexer = Lexer(buffer.str());
    Token token = lexer.next();

    while (token.class_name != ClassName::None) {
        std::cout << "Token\t" << token.class_name << "\t" << token.value << std::endl;
        token = lexer.next();
    }

    return 0;
}
