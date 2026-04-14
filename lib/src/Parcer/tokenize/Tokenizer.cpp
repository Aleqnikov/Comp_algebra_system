#include "Tokenizer.h"

std::vector<TokenResult> Tokenizer::tokenize(const std::string& text) {
    std::vector<TokenResult> tokens;
    size_t i = 0;

    while (i < text.size()) {
        // 1. Пропуск пробелов
        if (isWhitespace(text[i])) {
            i++;
            continue;
        }

        // 2. Попытка найти оператор или команду через автомат (O(len))
        // Это эффективнее, чем перебирать мапу вручную
        auto [len, tokenOpt] = automaton.findLongestPrefix(text, i);
        
        if (tokenOpt.has_value()) {
            tokens.emplace_back(tokenOpt.value(), text.substr(i, len), i);
            i += len;
            continue;
        }

        // 3. Распознавание чисел (длинная арифметика - просто собираем строку цифр)
        if (isDigit(text[i])) {
            size_t start = i;
            while (i < text.size() && isDigit(text[i])) {
                i++;
            }
            tokens.emplace_back(Token::NUMBER, text.substr(start, i - start), start);
            continue;
        }

        // 4. Распознавание переменных
        if (isAlpha(text[i])) {
            size_t start = i;
            while (i < text.size() && isAlpha(text[i])) { 
                i++;
            }
            tokens.emplace_back(Token::VARIABLE, text.substr(start, i - start), start);
            continue;
        }

        // 5. Если ничего не подошло
        throw TokenizerException("Unknown symbol '" + std::string(1, text[i]) + "'", i);
    }

    return tokens;
}