#include "lexer.h"
#include "checkers.h"


yy::parser::symbol_type yylex(Driver& driver) {
    char c;

    while (driver.file.get(c)) {
        if (is_space(c)) {
            // Skip spaces and new line characters
        }
        else if (is_digit(c)) {
            return parse_number(driver, c);
        }
        else if (is_letter(c)) {
            return parse_identifier(driver);
        }
        else if (c == '"') {
            return parse_string(driver);
        }

        return parse_symbol(driver, c);
    }

    return yy::parser::make_END_OF_FILE(driver.location);
}


yy::parser::symbol_type parseString(Driver& driver) {
    char c;
    std::string value = "";

    while (driver.file.get(c) && c != '"') {
        value.push_back(c);
    }

    if (c != '"') {
        throw yy::parser::syntax_error(driver.location,
                "String literals should be enclosed");
    }

    return yy::parser::make_STRING(value, driver.location);
}


yy::parser::symbol_type parse_number(Driver& driver, char first) {
    char c;
    std::string str_value = "";

    str_value.push_back(first);

    while (driver.file.get(c) && is_digit(c)) {
        str_value.push_back(c);
    }

    // Parsing a real
    if (c == '.') {
        str_value.push_back(c);

        while (driver.file.get(c) && is_digit(c)) { str_value.push_back(c); }

        if (driver.file.get(c) && (c == 'E' || c == 'E')) {
            str_value.push_back(c);

            if (driver.file.get(c) && (c == '+' || c == '-')) {
                str_value.push_back(c);
            }

            if (driver.file.get(c) || !is_digit(c)) {
                throw yy::parser::syntax_error(driver.location,
                        "Exponents should be ended with a number");
            }

            while (driver.file.get(c) && is_digit(c)) {
                str_value.push_back(c);
            }
        }

        double value = std::stod(str_value);
        return yy::parser::make_REAL(value, driver.location);
    }

    int value = std::stoi(str_value);
    return yy::parser::make_INTEGER(value, driver.location);
}


yy::parser::symbol_type parse_identifier(Driver& driver) {
    Token token;
    std::string value = "";

    while (src_iter != src.end() && (isLetter(*src_iter) || isDigit(*src_iter))) {
        value += *src_iter;
        src_iter++;
    }

    if (src_iter != src.end() && *src_iter == '*') {
        value += *src_iter;
        src_iter++;
    }

    ClassName class_name = symbol_table.find(value);

    if (class_name == ClassName::None) {
        class_name = ClassName::Ident;
        symbol_table.insert(value, class_name);
    }

    token.class_name = class_name;
    token.value = value;

    return token;
}

yy::parser::symbol_type parse_symbol(Driver& driver, char first) {
    char c = first;

    if (c == '(') {
        return yy::parser::make_LEFT_PARENTHESIS(driver.location);
    }
    if (c == ')') {
        return yy::parser::make_RIGHT_PARENTHESIS(driver.location);
    }
    if (c == '[') {
        return yy::parser::make_LEFT_SQUARE_BRACKET(driver.location);
    }
    if (c == ']') {
        return yy::parser::make_RIGHT_SQUARE_BRACKET(driver.location);
    }
    if (c == ',') {
        return yy::parser::make_COMMA(driver.location);
    }
    if (c == '.') {
        return yy::parser::make_DOT(driver.location);
    }
    if (c == ':') {
        if (driver.file.get(c) && c == '=') {
            return yy::parser::make_ASSIGNMENT(driver.location);
        }
        return yy::parser::make_COLON(driver.location);
    }

    throw yy::parser::syntax_error(driver.location, "Unknown symbol");
}
