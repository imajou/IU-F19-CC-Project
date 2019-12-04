#include "lexer.hh"
#include "checkers.hh"

using namespace yy;


parser::symbol_type yylex(Driver &driver) {
    char c;

    while (driver.file.get(c)) {
        if (is_newline(c)) {
            std::move(driver.location).lines();
            continue;
        } else if (is_space(c)) {
            // Skip spaces and new line characters
            continue;
        } else if (is_digit(c)) {
            driver.file.unget();
            return parse_number(driver);
        } else if (is_letter(c)) {
            driver.file.unget();
            return parse_identifier(driver);
        } else if (c == '"') {
            return parse_string(driver);
        }

        return parse_symbol(driver);
    }

    std::cout << "End of file" << std::endl;
    return parser::make_END_OF_FILE(std::move(driver.location));
}


parser::symbol_type parse_string(Driver &driver) {
    char c;
    std::string value = "";

    while (driver.file.get(c) && c != '"') {
        value.push_back(c);
    }

    if (c != '"') {
        throw parser::syntax_error(std::move(driver.location),
                                   "String literals should be enclosed");
    }

    std::cout << "Parsed string value: " << value << std::endl;

    return parser::make_STRING(value, std::move(driver.location));
}


parser::symbol_type parse_number(Driver &driver) {
    char c;
    std::string str_value = "";

    while (driver.file.get(c) && is_digit(c)) {
        str_value.push_back(c);
    }

    std::cout << "Parsed number value: " << str_value << std::endl;

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
                throw parser::syntax_error(std::move(driver.location),
                                           "Exponents should be ended with a number");
            }

            while (driver.file.get(c) && is_digit(c)) {
                str_value.push_back(c);
            }
        }

        driver.file.unget();

        double value = std::stod(str_value);
        std::cout << "Number is double: " << value << std::endl;
        return parser::make_REAL(value, std::move(driver.location));
    }

    driver.file.unget();

    int value = std::stoi(str_value);
    std::cout << "Number is integer: " << value << std::endl;
    return parser::make_INTEGER(value, std::move(driver.location));
}


parser::symbol_type parse_identifier(Driver &driver) {
    char c;
    int token;
    std::string value = "";

    while (driver.file.get(c) && (is_letter(c) || is_digit(c))) {
        value.push_back(c);
    }
    driver.file.unget();

    std::cout << "Parsed identifier value: " << value << std::endl;

    try {
        token = keywords.at(value);
    } catch (std::out_of_range &) {
        // If keyword not found then it is an identifier
        return parser::make_IDENTIFIER(value, std::move(driver.location));
    }

    return parser::symbol_type(token, std::move(driver.location));
}

parser::symbol_type parse_symbol(Driver &driver) {
    char c;
    driver.file.get(c);

    if (c == '(') {
        return parser::make_LEFT_PARENTHESIS(std::move(driver.location));
    }
    if (c == ')') {
        return parser::make_RIGHT_PARENTHESIS(std::move(driver.location));
    }
    if (c == '[') {
        return parser::make_LEFT_SQUARE_BRACKET(std::move(driver.location));
    }
    if (c == ']') {
        return parser::make_RIGHT_SQUARE_BRACKET(std::move(driver.location));
    }
    if (c == ',') {
        return parser::make_COMMA(std::move(driver.location));
    }
    if (c == '.') {
        return parser::make_DOT(std::move(driver.location));
    }
    if (c == ':') {
        if (driver.file.get(c) && c == '=') {
            return parser::make_ASSIGNMENT(std::move(driver.location));
        }
        driver.file.unget();
        return parser::make_COLON(std::move(driver.location));
    }

    throw parser::syntax_error(std::move(driver.location), "Unknown symbol");
}
