#include <iostream>

#include "parser/driver.hh"
#include "parser/ast.hh"

extern Program *program_root;


int main(int argc, char *argv[]) {
    Driver driver;
    driver.parse(argv[1]);
    return 0;
}
