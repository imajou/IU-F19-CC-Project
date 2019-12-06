cmake_minimum_required(VERSION 3.15)
project(IU_F19_CC_Project)

set(CMAKE_CXX_STANDARD 11)

find_package(bison)

bison_target(PARSER parser/syntax.yy parser/syntax.cc DEFINES_FILE parser/syntax.hh)

message(${BISON_PARSER_OUTPUTS})

add_executable(IU_F19_CC_Project
        ${BISON_PARSER_OUTPUTS}
        parser/driver.cc parser/driver.hh
        lexer/lexer.cc lexer/lexer.hh
        lexer/checkers.cc lexer/checkers.hh
        parser/ast.cc parser/ast.hh
        parser/generator.cc parser/generator.hh
        main.cc)
