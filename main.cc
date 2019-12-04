#include <iostream>
#include "parser/driver.hh"

int main(int argc, char *argv[]) {
    // TODO: define keywords
    Driver driver;
    driver.parse(argv[1]);
    return 0;
}
