#include <gtest/gtest.h>
#include "Parcer/tokenize/Tokenizer.h"
#include "Parcer/tokenize/token_definitions.h"
#include "Exceptions/UniversalStringException.h"

// Тест базовых арифметических операций
TEST(TokenizerTest, BasicArithmetic) {
    Tokenizer tokenizer;
    std::string input = "x + 123 * y";
    auto tokens = tokenizer.tokenize(input);

    ASSERT_EQ(tokens.size(), 5);
    
    EXPECT_EQ(tokens[0].type, Token::VARIABLE);
    EXPECT_EQ(tokens[0].lexeme, "x");
    EXPECT_EQ(tokens[0].position, 0);

    EXPECT_EQ(tokens[1].type, Token::PLUS);
    EXPECT_EQ(tokens[2].type, Token::NUMBER);
    EXPECT_EQ(tokens[2].lexeme, "123");
    
    EXPECT_EQ(tokens[4].type, Token::VARIABLE);
    EXPECT_EQ(tokens[4].lexeme, "y");
}

// Тест команд LaTeX
TEST(TokenizerTest, LaTeXCommands) {
    Tokenizer tokenizer;
    std::string input = "\\frac{x}{y}";
    auto tokens = tokenizer.tokenize(input);

    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[0].type, Token::FRAC);
    EXPECT_EQ(tokens[0].lexeme, "\\frac");
    
    EXPECT_EQ(tokens[1].type, Token::LBRACE);
    EXPECT_EQ(tokens[2].type, Token::VARIABLE);
    EXPECT_EQ(tokens[3].type, Token::RBRACE);
}

// Тест жадного поиска (Aho-Corasick должен брать самый длинный паттерн)
TEST(TokenizerTest, GreedyMatching) {
    Tokenizer tokenizer;
    // Предположим, у нас есть паттерны "^" и какой-то гипотетический "^^"
    // Но проверим на существующих: \frac vs \f (если бы был)
    std::string input = "\\gcd"; 
    auto tokens = tokenizer.tokenize(input);

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, Token::GCD);
    EXPECT_EQ(tokens[0].lexeme, "\\gcd");
}

// Тест длинных чисел (BigInt)
TEST(TokenizerTest, LongNumbers) {
    Tokenizer tokenizer;
    std::string bigNum = "123456789012345678901234567890";
    auto tokens = tokenizer.tokenize(bigNum);

    ASSERT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, Token::NUMBER);
    EXPECT_EQ(tokens[0].lexeme, bigNum);
}

// Тест обработки неизвестных символов (Исключения)
TEST(TokenizerTest, UnknownSymbols) {
    Tokenizer tokenizer;
    std::string input = "x @ y";
    
    // Проверяем, что выбрасывается наше кастомное исключение
    EXPECT_THROW(tokenizer.tokenize(input), TokenizerException);
}

// Тест на отсутствие пробелов
TEST(TokenizerTest, NoWhitespaces) {
    Tokenizer tokenizer;
    std::string input = "a+b*c^2";
    auto tokens = tokenizer.tokenize(input);

    ASSERT_EQ(tokens.size(), 7);
    EXPECT_EQ(tokens[1].type, Token::PLUS);
    EXPECT_EQ(tokens[3].type, Token::MULTIPLY);
    EXPECT_EQ(tokens[5].type, Token::POWER);
}

// Тест позиционирования
TEST(TokenizerTest, TokenPositions) {
    Tokenizer tokenizer;
    std::string input = "  x  + y";
    auto tokens = tokenizer.tokenize(input);

    // "x" находится на позиции 2 (после двух пробелов)
    EXPECT_EQ(tokens[0].lexeme, "x");
    EXPECT_EQ(tokens[0].position, 2);

    // "+" находится на позиции 5
    EXPECT_EQ(tokens[1].lexeme, "+");
    EXPECT_EQ(tokens[1].position, 5);
}