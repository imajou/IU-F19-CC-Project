cmake_minimum_required(VERSION 3.15)
project(IU_F19_CC_Project)

set(CMAKE_CXX_STANDARD 11)

find_package(bison)

bison_target(PARSER parser/syntax.yy parser/syntax.cc)

add_executable(IU_F19_CC_Project
        ${BISON_PARSER_OUTPUTS}
        parser/driver.cc parser/driver.hh
        lexer/lexer.cc lexer/checkers.cc
        main.cc)