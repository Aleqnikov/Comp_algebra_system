#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <vector>

enum class TokenType {
    NUMBER,    // рациональное: "3", "-2", "3/4"
    VARIABLE,  // имя переменной: "x", "y", "alpha"
    FUNC,      // sin, cos, tan, cot, sqrt, exp, log
    PLUS,      // +
    MINUS,     // -
    STAR,      // *
    SLASH,     // /
    CARET,     // ^
    LPAREN,    // (
    RPAREN,    // )
    COMMA,     // ,
    END        // конец строки
};

struct Token {
    TokenType   type;
    std::string text;
};

// Лексический анализ строки инфиксного выражения.
// Бросает std::runtime_error при неожиданном символе.
std::vector<Token> tokenize(const std::string& expr);

// Имена функций (в нижнем регистре)
bool isFuncName(const std::string& s);

#endif
