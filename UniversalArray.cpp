#include <iostream>
#include "token.h"
#include "UniversalArray.h"


void UniversalArray::push_back(const Value& v) {
    if (size >= capacity) {
        // Resize array
        size_t new_capacity = capacity * 2;
        Value* new_elements = new Value[new_capacity];
        for (size_t i = 0; i < size; ++i) {
            new_elements[i] = elements[i];
        }
        delete[] elements;
        elements = new_elements;
        capacity = new_capacity;
    }
    elements[size++] = v;
}
UniversalArray::UniversalArray(TokenType type, size_t initial_capacity = 4)
    : ListType(type), size(0), capacity(initial_capacity)
{
    elements = new Value[capacity];
}
UniversalArray::~UniversalArray() {
    delete[] elements;
}