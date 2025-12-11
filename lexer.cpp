#include "lexer.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <cctype> // For isalpha, isdigit
#include "UniversalArray.h" // Assuming this defines Value

using namespace std;

// --- Existing Setup/Helper Functions (omitted for brevity) ---

lexer::lexer(const char* user_code, size_t length) :
    source_code(user_code),
    current_index(0),
    current_line(1),
    current_column(1), current(source_code), start(source_code) {
}
bool lexer::isSpace(char c) {
    switch (c) {
    case ' ':
    case '\n':
    case '\r':
    case '\t':
        return true;
    default:
        return false;
    }
}
char lexer::peek() const {
    return *(current + 1);
}
void lexer::advance() {
    if (*current == '\n') {
        current_line++;
        current_column = 1;
    }
    else {
        current_column++;
    }
    current++;
}
void lexer::skip_whitespace() {
    while (isSpace(*current) == true) {
        current++;
    }
}

// make_token: creates a token of the given type and text at the current position
Token lexer::make_token(TokenType type, const char* text) {
    return Token(type, text, current_line, current_column);
}

// number_literal: reads a sequence of digits and returns a Number token
Token lexer::number_literal() {
    char buffer[256] = { 0 };
    int i = 0;

    int start_line = current_line;
    int start_column = current_column;

    // Read digits
    while (*current >= '0' && *current <= '9') {
        if (i < 255) {
            buffer[i++] = *current;
        }
        advance();
    }

    buffer[i] = '\0'; // null-terminate

    return Token(TokenType::Number, buffer, start_line, start_column);
}

// single_char_literal
Token lexer::single_char_literal() {
    int start_line = current_line;
    int start_column = current_column;

    advance(); // Skip opening quote '

    char buffer[2] = { 0 };
    buffer[0] = *current;

    advance(); // Skip character
    advance(); // Skip closing quote '

    return Token(TokenType::Char, buffer, start_line, start_column);
}

// --- UPDATED: identifier_literal now handles ALL word-based keywords and operators ---
Token lexer::identifier_literal() {
    char buffer[256] = { 0 };
    int i = 0;

    int start_line = current_line;
    int start_column = current_column;

    // Read first character
    buffer[i++] = *current;
    advance();

    // Continue gathering valid identifier characters
    while ((isalpha(*current) || isdigit(*current) || *current == '_'))
    {
        if (i < 255)
            buffer[i++] = *current;

        advance();
    }

    buffer[i] = '\0';

    // -------- CORE KEYWORDS (Logic) --------
    if (strcmp(buffer, "def") == 0)       return Token(TokenType::Def, buffer, start_line, start_column);
    if (strcmp(buffer, "class") == 0)     return Token(TokenType::Class, buffer, start_line, start_column);
    if (strcmp(buffer, "import") == 0)    return Token(TokenType::Import, buffer, start_line, start_column);
    if (strcmp(buffer, "return") == 0)    return Token(TokenType::Return, buffer, start_line, start_column);
    if (strcmp(buffer, "if") == 0)        return Token(TokenType::If, buffer, start_line, start_column);
    if (strcmp(buffer, "else") == 0)      return Token(TokenType::Else, buffer, start_line, start_column);
    if (strcmp(buffer, "while") == 0)     return Token(TokenType::While, buffer, start_line, start_column);
    if (strcmp(buffer, "for") == 0)       return Token(TokenType::For, buffer, start_line, start_column);

    // -------- DATA TYPES & LITERALS --------
    if (strcmp(buffer, "true") == 0)      return Token(TokenType::Bool, "true", start_line, start_column);
    if (strcmp(buffer, "false") == 0)     return Token(TokenType::Bool, "false", start_line, start_column);
    if (strcmp(buffer, "List") == 0)      return Token(TokenType::List, buffer, start_line, start_column);


    // -------- WORD-BASED OPERATORS --------
    if (strcmp(buffer, "assign") == 0)         return Token(TokenType::OpAssign, buffer, start_line, start_column);
    if (strcmp(buffer, "is") == 0)             return Token(TokenType::OpIsEqual, buffer, start_line, start_column);
    if (strcmp(buffer, "is_not") == 0)         return Token(TokenType::OpIsNotEqual, buffer, start_line, start_column);

    if (strcmp(buffer, "plus") == 0)           return Token(TokenType::OpPlus, buffer, start_line, start_column);
    if (strcmp(buffer, "minus") == 0)          return Token(TokenType::OpMinus, buffer, start_line, start_column);
    if (strcmp(buffer, "times") == 0)          return Token(TokenType::OpStar, buffer, start_line, start_column);
    if (strcmp(buffer, "div") == 0)            return Token(TokenType::OpSlash, buffer, start_line, start_column);
    if (strcmp(buffer, "mod") == 0)            return Token(TokenType::OpMod, buffer, start_line, start_column);

    if (strcmp(buffer, "less") == 0)           return Token(TokenType::OpLess, buffer, start_line, start_column);
    if (strcmp(buffer, "greater") == 0)        return Token(TokenType::OpGreater, buffer, start_line, start_column);
    if (strcmp(buffer, "less_equal") == 0)     return Token(TokenType::OpIsLessEqual, buffer, start_line, start_column);
    if (strcmp(buffer, "greater_equal") == 0)  return Token(TokenType::OpIsGreaterEqual, buffer, start_line, start_column);

    // -------- DEFAULT IDENTIFIER --------
    return Token(TokenType::Identifier, buffer, start_line, start_column);
}


// list_literal (No change needed, as it only skips symbols not words)
Token lexer::list_literal() {
    // ... (content of list_literal remains the same as previous step, without comma handling)
    int start_line = current_line;
    int start_column = current_column;
    advance(); // Skip the opening '['

    TokenType listType = TokenType::UNKNOWN;
    UniversalArray* arr = nullptr;

    while (*current != ']') {
        skip_whitespace();

        if (*current == '\0') {
            break;
        }

        Value v;

        // ----- NUMBER (0-9) -----
        if (isdigit(*current)) {
            Token num = number_literal();
            v.type = TokenType::Number;
            v.data.i = atoi(num.get_value());
        }
        // ----- CHAR (') -----
        else if (*current == '\'') {
            advance(); // skip opening '
            char c = *current;
            advance(); // onto closing '
            advance(); // skip closing '

            v.type = TokenType::Char;
            v.data.c = c;
        }
        // ----- NESTED LIST ([) -----
        else if (*current == '[') {
            Token nested_list_token = list_literal();
            v.type = TokenType::List;
            v.data.arr = nested_list_token.get_array_ptr();
        }
        // ----- IDENTIFIER/KEYWORD (includes 'true', 'false', and now *words*) -----
        else if (isalpha(*current) || *current == '_') {
            Token keyword_or_id = identifier_literal();

            if (keyword_or_id.get_type() == TokenType::Bool) {
                v.type = TokenType::Bool;
                v.data.b = (strcmp(keyword_or_id.get_value(), "true") == 0);
            }
            else {
                // If it's any other keyword (like 'assign' or 'if') or a regular identifier, skip
                continue;
            }
        }
        else {
            advance();
            continue;
        }

        // --- Type Check and Push ---
        if (!arr) {
            listType = v.type;
            arr = new UniversalArray(listType);
        }

        if (v.type == listType || v.type == TokenType::List) {
            arr->push_back(v);
        }
        else {
            // Handle type mismatch error
        }
    }

    advance(); // Skip closing ']'

    TokenType type = TokenType::List;
    Token tok(type, arr, start_line, start_column);
    return tok;
}


// --- DELETED: operator_literal() is no longer needed ---


// --- Finalized: get_Token (main lexer loop) ---
Token lexer::get_Token() {

    // 1. Skip all leading whitespace
    skip_whitespace();

    int start_line = current_line;
    int start_column = current_column;

    // 2. Check for End-of-File
    if (*current == '\0') {
        return Token(TokenType::Eof, "", start_line, start_column);
    }

    // 3. Dispatch based on the current character
    char current_char = *current;

    // --- Literals ---
    if (isdigit(current_char)) {
        return number_literal();
    }

    if (current_char == '[') { // List/Array literal
        return list_literal();
    }

    if (current_char == '\'') { // Char literal
        return single_char_literal();
    }

    // Identifiers, Keywords, and now ALL WORD OPERATORS
    if (isalpha(current_char) || current_char == '_') {
        return identifier_literal();
    }

    // --- Punctuation (The only remaining symbols) ---
    switch (current_char) {
    case '(': advance(); return Token(TokenType::LParen, "(", start_line, start_column);
    case ')': advance(); return Token(TokenType::RParen, ")", start_line, start_column);
    case '{': advance(); return Token(TokenType::LBrace, "{", start_line, start_column);
    case '}': advance(); return Token(TokenType::RBrace, "}", start_line, start_column);

    default:
        // Unknown symbol
        advance();
        return Token(TokenType::UNKNOWN, &current_char, start_line, start_column);
    }
}