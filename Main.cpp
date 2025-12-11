#include "lexer.h"
#include "token.h"
#include <iostream>

using namespace std;

int main() {

}
/*
1️⃣ Lexer Functions (tokenizer)

Your current ones:

peek() → check next char

advance() → move current pointer

skip_whitespace() → skip spaces/tabs/newlines

isSpace(char) → helper

make_token() → create Token object

identifier_literal() → parse identifiers and keywords

number_literal() → parse integers/decimals

single_char_literal() → parse char literals

list_literal() → parse list/array literals

Additional lexer functions you will need:

string_literal() → parse "text"

comment_skip() → skip single-line // or multi-line /* */ /*

operator_token() → parse operators like + - * / %

comparison_token() → parse < > <= >= == !=

eof_token() → generate EOF token at end of file

get_next_token() → main function that decides which literal/keyword/operator to call

Essentially, the lexer exposes a stream of tokens to the parser.

2️⃣ Parser Functions

The parser builds an AST (Abstract Syntax Tree). For a 10–30k line compiler, you will want functions like:

parse_program() → entry point for parsing

parse_statement() → if/while/for/return/expr statements

parse_expression() → arithmetic/logical expressions

parse_term() → multiplication/division

parse_factor() → numbers, identifiers, parentheses

parse_function() → def and parameter parsing

parse_class() → class parsing

parse_list() → parse list literals

parse_variable_declaration() → parse var x = ...

Each parser function corresponds to grammar rules.

3️⃣ AST Node Classes

You need structures for representing code:

ASTNode (base class)

ExpressionNode → literals, identifiers, arithmetic

StatementNode → assignment, loops, if/else

FunctionNode → function definitions

ClassNode → classes

ListNode → array/list literals

4️⃣ Semantic Analysis / Type Checking

check_types(ASTNode* node) → ensure assignments match type

check_scope() → variable/function/class scope

check_undeclared_identifiers() → errors for missing vars

check_function_calls() → correct arguments

check_return_types() → functions return correct type

5️⃣ Intermediate Representation / Optimization

Even a simple compiler benefits from:

generate_IR(ASTNode* node) → produce intermediate code

optimize_dead_code() → remove unused variables or statements

constant_folding() → evaluate constants at compile-time

simplify_expressions() → optimize math/logical expressions

6️⃣ Code Generation

This is where you turn AST or IR into machine code or C++ code:

generate_cpp(ASTNode* node) → write C++ equivalent

generate_binary(ASTNode* node) → optional, actual machine code

emit_operator() → emit code for + - * / /*

emit_function_call() → handle calling functions

emit_class() → convert Luciro class to C++ class

emit_list() → convert Luciro array to C++ array

7️⃣ Error Handling / Diagnostics

error(const char* message, int line, int column) → lexer/parser errors

warning(const char* message, int line, int column) → non-fatal messages

8️⃣ Utilities / Helpers

is_digit(char c)

is_alpha(char c)

is_alnum(char c)

string_to_int() / string_to_double()

token_to_string() → debug printing

advance_line() → handle \n counting
*/