#pragma once

#include <string>


enum ClassName {
    None,  // Special class meaning that token is empty
    Ident,  // Identificator
    Not, And, Or,  // Logical operators
    Import, Module,  // Module keywords
    True, False, Nil,  // Special values
    Proc, Begin, End, Ret,  // Procedure keywords
    If, Elif, Else, Case, Then,  // Condition keywords
    Add, Sub, Mul, Div, DivInt, Mod,  // Math operations
    While, For, To, By, Do, Rep, Until,  // Loop keywords
    Type, Pointer, Var, Const, Arr, Of, Rec,  // Type keywords
    IntDec, IntHex, IntExp, Real, Str, StrHex,  // Literals
    Colon, Semicolon, Assign, Comma, Dot, Caret, Range,  // Special symbols
    Eq, NoEq, Less, LessOrEq, Great, GreatOrEq, Is, In,  // Comparison operators
    LefBr, RightBr, LefSqBr, RightSqBr, LefFigBr, RightFigBr,  // Brackets
    BoolType, CharType, IntType, RealType, ByteType, SetType,  // Basic types
};


class Token {
public:
    ClassName class_name;
    std::string value;

public:
    Token();
    Token(ClassName class_name, std::string value);
};
