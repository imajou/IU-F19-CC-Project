#include "Token.hpp"


Token::Token() {
    this->class_name = ClassName::None;
}


Token::Token(ClassName class_name, std::string value) {
    this->class_name = class_name;
    this->value = value;
}
