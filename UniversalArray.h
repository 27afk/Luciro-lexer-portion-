#include <cstddef>
#include "token.h"

#pragma once
struct UniversalArray;
struct Value {
    TokenType type;
    union {
        int i;
        bool b;
        double d;
        char c;
        UniversalArray* arr; // for classes
    } data;
};

struct UniversalArray {
    TokenType ListType;    // type of elements
    Value* elements;       // pointer to array of Values
    size_t size;           // current number of elements
    size_t capacity;       // maximum allocated elements

    // Constructor
    UniversalArray(TokenType type, size_t initial_capacity = 4);

    // Destructor
    ~UniversalArray();

    // Add a new element
    void push_back(const Value& v);
};
