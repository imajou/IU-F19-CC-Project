#include <iostream>

#include "parser/ast.hh"
#include "parser/driver.hh"
#include "parser/generator.hh"

extern Program *program_root;

int main(int argc, char *argv[]) {
    Driver driver;
    driver.parse(argv[1]);

    GeneratorContext context;
    context.generate_code(*program_root);

    return 0;
}
