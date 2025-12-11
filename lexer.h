#include "token.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <unordered_map>
#pragma once


class lexer {
private:
	const char* source_code;
	size_t current_index;
	int current_line;
	int current_column;
	const char* current;
	const char* start;
	// unordered_map <const char*, TokenType> KEYWORDS;
	char peek()const;
	void advance();
	void skip_whitespace();
	Token make_token(TokenType type, const char* value);
	Token identifier_literal(); // Now handles all word-based operators
	Token number_literal();
	Token list_literal();
	bool isSpace(char c);
	Token single_char_literal();

public:
	lexer(const char* user_code, size_t length);
	Token get_Token();
};