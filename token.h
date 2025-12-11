#pragma once

#include <vector>
#include "UniversalArray.h"
#include <cstddef> // for size_t

enum class TokenType {
    Identifier,
    // primitive data types
    DecNumber,
    Number,
    Char,
    Bool,
    List,
    // --- WORD-BASED OPERATORS ---
    OpAssign,        // 'assign' for =
    OpIsEqual,       // 'is' or 'equal' for ==
    OpIsNotEqual,    // 'is_not' for !=
    OpPlus,          // 'plus' for +
    OpMinus,         // 'minus' for -
    OpStar,          // 'times' for *
    OpSlash,         // 'div' for /
    OpMod,           // 'mod' for %
    OpLess,          // 'less' for <
    OpGreater,       // 'greater' for >
    OpIsLessEqual,   // 'less_equal' for <=
    OpIsGreaterEqual,// 'greater_equal' for >=

    // key words and symbols (parentheses, braces are kept as symbols for clarity)
    LParen, RParen, LBrace, RBrace,
    Def, Class, Import, Return, If, Else, While, For,
    Eof, UNKNOWN
};

class Token {
private:
    TokenType type;
    char value[256];
    int line;
    int column;
    UniversalArray* arrayPtr; // for List tokens
public:
    // Constructors
    Token(TokenType t = TokenType::UNKNOWN, const char* v = "", int l = 0, int c = 0);
    Token(TokenType t, UniversalArray* arr, int l, int c);
    // Set token values
    void set_token(TokenType t, const char* v, int l, int c);
    // Getters
    TokenType get_type() const { return type; }
    const char* get_value() const { return value; }
    int get_line() const { return line; }
    int get_column() const { return column; }
    // Getter for arrayPtr
    UniversalArray* get_array_ptr() const { return arrayPtr; }
};