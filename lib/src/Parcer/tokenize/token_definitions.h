#ifndef TOKEN_DEFINITIONS_H
#define TOKEN_DEFINITIONS_H

#include <string>

enum class Token {
    // Команды LaTeX
    FRAC, SQRT, GCD, LCM,
    
    // Скобки
    LBRACE, RBRACE, LPAREN, RPAREN,
    
    // Операторы
    PLUS, MINUS, MULTIPLY, DIVIDE, POWER, MODULO,
    
    // Разделители
    PIPE, COMMA,
    
    // Данные (для CAS важно, что NUMBER - это строка цифр любой длины)
    NUMBER,     
    VARIABLE,   
    
    // Служебные
    WHITESPACE, 
    UNKNOWN,
    END_OF_FILE
};

struct TokenResult {
    Token type;
    std::string lexeme;
    size_t position;

    TokenResult(Token t, std::string lex, size_t pos) 
        : type(t), lexeme(std::move(lex)), position(pos) {}
};

inline std::string tokenToString(Token token) {
    switch (token) {
        case Token::FRAC: return "FRAC";
        case Token::SQRT: return "SQRT";
        case Token::GCD: return "GCD";
        case Token::LCM: return "LCM";
        case Token::LBRACE: return "LBRACE";
        case Token::RBRACE: return "RBRACE";
        case Token::LPAREN: return "LPAREN";
        case Token::RPAREN: return "RPAREN";
        case Token::PLUS: return "PLUS";
        case Token::MINUS: return "MINUS";
        case Token::MULTIPLY: return "MULTIPLY";
        case Token::DIVIDE: return "DIVIDE";
        case Token::POWER: return "POWER";
        case Token::MODULO: return "MODULO";
        case Token::PIPE: return "PIPE";
        case Token::COMMA: return "COMMA";
        case Token::NUMBER: return "NUMBER";
        case Token::VARIABLE: return "VARIABLE";
        case Token::WHITESPACE: return "WHITESPACE";
        default: return "UNKNOWN";
    }
}

#endif // TOKEN_DEFINITIONS_H