#include "Lexer.hpp"


Lexer::Lexer(std::string src) {
    this->src = src;
    this->src_iter = this->src.begin();

    createSymbolTable();
}


Token Lexer::next() {
    Token token;

    while (src_iter != src.end()) {
        if (*src_iter == ' ' || *src_iter == '\n' || *src_iter == '\t') {
            // Skip spaces and new line characters
        }
        else if (*src_iter == '(' && (src_iter + 1) != src.end()
                 && *(src_iter + 1) == '*') {
            parseComment();
        }
        else if (isDigit(*src_iter)) {
            return parseNumber();
        }
        else if (isLetter(*src_iter)) {
            return parseIdentifier();
        }
        else if (*src_iter == '"') {
            return parseString();
        }
        else if (isOtherSymbol(*src_iter)) {
            return parseOtherSymbol();
        }
        else {
            std::cerr << "Invalid symbol: " << *src_iter << std::endl;
            exit(1);
        }

        src_iter++;
    }

    return token;
}


void Lexer::createSymbolTable() {
    // Feel the table keywords
    std::pair<std::string, ClassName> keywords[] = {
        {"ARRAY", ClassName::Arr}, {"OF", ClassName::Of},
        {"PROCEDURE", ClassName::Proc}, {"BEGIN", ClassName::Begin},
        {"END", ClassName::End}, {"RETURN", ClassName::Ret},
        {"RECORD", ClassName::Rec},
        {"POINTER", ClassName::Pointer}, {"TO", ClassName::To},
        {"IN", ClassName::In}, {"IS", ClassName::Is}, {"OR", ClassName::Or},
        {"DIV", ClassName::Div}, {"MOD", ClassName::Mod},
        {"FALSE", ClassName::False}, {"TRUE", ClassName::True},
        {"NIL", ClassName::Nil},
        {"IF", ClassName::If}, {"THEN", ClassName::Then},
        {"ELSIF", ClassName::Elif}, {"ELSE", ClassName::Else},
        {"CASE", ClassName::Case}, {"OF", ClassName::Of},
        {"WHILE", ClassName::While}, {"DO", ClassName::Do},
        {"REPEAT", ClassName::Rep}, {"UNTIL", ClassName::Until},
        {"FOR", ClassName::For}, {"BY", ClassName::By},
        {"CONST", ClassName::Const}, {"TYPE", ClassName::Type},
        {"VAR", ClassName::Var},
        {"MODULE", ClassName::Module}, {"IMPORT", ClassName::Import},
    };

    std::string identifiers[] = {
        "ABS", "ASR", "ASSERT", "BOOLEAN", "BYTE", "CHAR", "CHR",
        "DEC", "EXCL", "FLOOR", "FLT", "INC", "INCL", "INTEGER",
        "LEN", "LSL", "NEW", "ODD", "ORD", "PACK", "REAL", "ROR",
        "SET", "UNPK",
    };

    for (auto keyword : keywords)
        symbol_table.insert(keyword.first, keyword.second);

    for (std::string identifier : identifiers)
        symbol_table.insert(identifier, ClassName::Ident);
}


bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}


bool Lexer::isHexDigit(char c) {
    return isDigit(c) || (c >= 'A' && c <= 'F');
}

bool Lexer::isLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}


bool Lexer::isOtherSymbol(char c) {
    return (c >= '!' && c <= '/') || (c >= ':' && c <= '>') ||
           (c >= '[' && c <= '^') || (c >= '{' && c <= '~');
}


void Lexer::parseComment() {
    if (src_iter + 2 == src.end()) {
        std::cerr << "Comment section should be enclosed" << std::endl;
        exit(1);
    }

    char last_char = ' ';
    while (last_char != '*' && *src_iter != ')') {
        src_iter++;

        if (src_iter == src.end()) {
            std::cerr << "Comment section should be enclosed" << std::endl;
            exit(1);
        }
    }
}


Token Lexer::parseString() {
    Token token;
    std::string value = "";

    src_iter++;

    while (src_iter != src.end() && *src_iter != '"') {
        value += *src_iter;
        src_iter++;
    }

    if (src_iter == src.end()) {
        std::cerr << "std::strings should have quotes in the end" << std::endl;
        exit(1);
    }

    src_iter++;

    token.class_name = ClassName::Str;
    token.value = value;
    return token;
}


Token Lexer::parseOtherSymbol() {
    Token token = Token(ClassName::None, "");
    
    if (*src_iter == '+') {
        token.value = "+";
        token.class_name = ClassName::Add;
    }
    else if (*src_iter == '-') {
        token.value = "-";
        token.class_name = ClassName::Sub;
    }
    else if (*src_iter == '*') {
        token.value = "*";
        token.class_name = ClassName::Mul;
    }
    else if (*src_iter == '/') {
        token.value = "/";
        token.class_name = ClassName::Div;
    }
    else if (*src_iter == '#') {
        token.value = "#";
        token.class_name = ClassName::NoEq;
    }
    else if (*src_iter == '=') {
        token.value = '=';
        token.class_name = ClassName::Eq;
    }
    else if (*src_iter == '<') {
        if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
            src_iter++;
            token.value = "<=";
            token.class_name = ClassName::LessOrEq;
        }
        else {
            token.value = "<";
            token.class_name = ClassName::Less;
        }
    }
    else if (*src_iter == '>') {
        if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
            src_iter++;
            token.value = ">=";
            token.class_name = ClassName::GreatOrEq;
        }
        else {
            token.value = ">";
            token.class_name = ClassName::Great;
        }
    }
    else if (*src_iter == '~') {
        token.value = "~";
        token.class_name = ClassName::Not;
    }
    else if (*src_iter == '[') {
        token.value = "[";
        token.class_name = ClassName::LefSqBr;
    }
    else if (*src_iter == '(') {
        token.value = "(";
        token.class_name = ClassName::LefBr;
    }
    else if (*src_iter == '{') {
        token.value = "{";
        token.class_name = ClassName::LefFigBr;
    }
    else if (*src_iter == ']') {
        token.value = "]";
        token.class_name = ClassName::RightSqBr;
    }
    else if (*src_iter == ')') {
        token.value = ")";
        token.class_name = ClassName::RightBr;
    }
    else if (*src_iter == '}') {
        token.value = "}";
        token.class_name = ClassName::RightFigBr;
    }
    else if (*src_iter == '&') {
        token.value = "&";
        token.class_name = ClassName::And;
    }
    else if (*src_iter == ':') {
        if (src_iter + 1 != src.end() && *(src_iter + 1) == '=') {
            src_iter++;
            token.value = ":=";
            token.class_name = ClassName::Assign;
        }
        else {
            token.value = ":";
            token.class_name = ClassName::Colon;
        }
    }
    else if (*src_iter == ';') {
        token.value = ";";
        token.class_name = ClassName::Semicolon;
    }
    else if (*src_iter == ',') {
        token.value = ",";
        token.class_name = ClassName::Comma;
    }
    else if (*src_iter == '.') {
        if (src_iter + 1 != src.end() && *(src_iter + 1) == '.') {
            src_iter++;
            token.value = "..";
            token.class_name = ClassName::Range;
        }
        else {
            token.value = ".";
            token.class_name = ClassName::Dot;
        }
    }
    else if (*src_iter == '^') {
        token.value = "^";
        token.class_name = ClassName::Caret;
    }

    src_iter++;
    return token;
}


Token Lexer::parseNumber() {
    Token token;
    std::string value = "";    

    // Skip zeros
    while (src_iter != src.end() && *src_iter == '0') { src_iter++; }

    while (true) {
        if (isDigit(*src_iter)) {
            value += *src_iter;
        }
        else if (*src_iter >= 'A' && *src_iter <= 'F') {
            while (src_iter != src.end() && isHexDigit(*src_iter)) {
                value += *src_iter;
                src_iter++;
            }

            if (src_iter == src.end() || (*src_iter != 'H' && *src_iter != 'X')) {
                std::cerr << "Hex number should be ended with an 'H' or an 'X' symbol" << std::endl;
                exit(1);
            }

            value += *src_iter;
            src_iter++;

            if (value.back() == 'H') {
                token.class_name = ClassName::IntHex;
            }
            else if (value.back() == 'X') {
                token.class_name = ClassName::StrHex;
            }
            token.value = value;
            return token;
        }
        else if (*src_iter == 'H') {
            value += *src_iter;
            src_iter++;

            token.class_name = ClassName::IntHex;
            token.value = value;
            return token;
        }
        else if (*src_iter == 'X') {
            value += *src_iter;
            src_iter++;

            token.class_name = ClassName::StrHex;
            token.value = value;
            return token;
        }
        else if (*src_iter == '.') {
            value += *src_iter;
            src_iter++;

            while (src_iter != src.end() && isDigit(*src_iter)) {
                value += *src_iter;
                src_iter++;
            }

            if (src_iter != src.end() && *src_iter == 'E') {
                value += *src_iter;
                src_iter++;

                if (src_iter != src.end() && (*src_iter == '+' || *src_iter == '-')) {
                    value += *src_iter;
                    src_iter++;
                }

                if (src_iter == src.end() || !isDigit(*src_iter)) {
                    std::cerr << "An exponent notation should be ended with a number" << std::endl;
                    exit(1);
                }

                while (src_iter != src.end() && isDigit(*src_iter)) {
                    value += *src_iter;
                    src_iter++;
                }
            }

            token.class_name = ClassName::Real;
            token.value = value;
            return token;
        }
        else {
            break;
        }

        src_iter++;
    }

    if (value == "")
        value = "0";

    token.class_name = ClassName::IntDec;
    token.value = value;
    return token;
}


Token Lexer::parseIdentifier() {
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
