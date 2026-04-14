#ifndef LATEX_LEXEMES_H
#define LATEX_LEXEMES_H

#include <map>
#include <string>
#include "token_definitions.h"

// Легко редактируемый список паттернов
const std::map<std::string, Token> LATEX_LEXEMES_MAP = {
    // LaTeX команды
    {"\\frac", Token::FRAC},
    {"\\sqrt", Token::SQRT},
    {"\\gcd",  Token::GCD},
    {"\\lcm",  Token::LCM},

    // Скобки
    {"{", Token::LBRACE},
    {"}", Token::RBRACE},
    {"(", Token::LPAREN},
    {")", Token::RPAREN},

    // Операторы
    // Примечание: "x^" убрал, так как это комбинация переменной и оператора.
    // Парсер должен сам разбираться с последовательностью Variable -> Power.
    {"+",  Token::PLUS},
    {"-",  Token::MINUS},
    {"*",  Token::MULTIPLY},
    {"/",  Token::DIVIDE},
    {"^",  Token::POWER},
    {"%",  Token::MODULO},

    // Разделители
    {"|", Token::PIPE},
    {",", Token::COMMA},
    
    // Можно добавить сравнения, если нужно для уравнений
    {"=", Token::UNKNOWN} // Пример
};

#endif // LATEX_LEXEMES_H