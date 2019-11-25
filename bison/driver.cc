#include "driver.h"


Driver::Driver() { }


int Driver::parse(const std::string& filename) {
    // Read the file
    file = std::ifstream(filename);
    // Run the Bison parser
    yy::parser parse(*this);
    int result = parse();
    // Close the file
    file.close();
    return result;
}
