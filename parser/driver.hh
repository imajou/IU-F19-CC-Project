#ifndef DRIVER_H_
#define DRIVER_H_

#include <map>
#include <fstream>
#include "syntax.hh"

class Driver {
private:
    std::string filename;

public:
    std::ifstream file;
    yy::location location;
    std::map<std::string, int> identifiers;

    Driver();

    int parse(const std::string &filename);
};

#endif  // DRIVER_H_
