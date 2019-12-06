#include <iostream>

#include "parser/ast.hh"
#include "parser/driver.hh"
#include "parser/generator.hh"

extern Program *program_root;

int main(int argc, char *argv[]) {
    Driver driver;
    std::string file_in = argv[1];

    driver.parse(file_in);

    GeneratorContext context(file_in);
    context.generate_code(*program_root);

    return 0;
}
