#include "token.h"
#include <iostream>
#include <cstring>
using namespace std;



Token::Token(TokenType t, const char* v, int l, int c)
    : type(t), line(l), column(c)
{
    size_t i = 0;
    while (v[i] != '\0' && i < 255) {
        value[i] = v[i];
        ++i;
    }
    value[i] = '\0';
}

void Token::set_token(TokenType t, const char* v, int l, int c) {
    type = t;
    line = l;
    column = c;
    size_t i = 0;
    while (v[i] != '\0' && i < 255) {
        value[i] = v[i];
        ++i;
    }
    value[i] = '\0';
}

Token::Token(TokenType t, UniversalArray* arr, int l, int c)
    : type(t), arrayPtr(arr), line(l), column(c)
{
    value[0] = '\0'; // No text representation needed
}
