#include "checkers.h"


bool is_newline(char c) {
    return c == '\n';
}


bool is_space(char c) {
    return c == ' ' || c == '\t';
}


bool is_digit(char c) {
    return c >= '0' && c <= '9';
}


bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
