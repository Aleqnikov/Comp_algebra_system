#include "tokenizer.h"
#include <cctype>
#include <stdexcept>
#include <algorithm>

static const std::vector<std::string> FUNCS = {
    "sin","cos","tan","cot","sqrt","exp","log"
};

bool isFuncName(const std::string& s) {
    std::string lo = s;
    std::transform(lo.begin(), lo.end(), lo.begin(), ::tolower);
    for (auto& f : FUNCS) if (f == lo) return true;
    return false;
}

std::vector<Token> tokenize(const std::string& expr) {
    std::vector<Token> tokens;
    size_t i = 0, n = expr.size();

    auto skipSpaces = [&]() {
        while (i < n && std::isspace((unsigned char)expr[i])) ++i;
    };

    while (true) {
        skipSpaces();
        if (i >= n) { tokens.push_back({TokenType::END, ""}); break; }

        char c = expr[i];

        // Буква — переменная или функция
        if (std::isalpha((unsigned char)c)) {
            std::string word;
            while (i < n && std::isalpha((unsigned char)expr[i]))
                word += expr[i++];
            std::string lo = word;
            std::transform(lo.begin(), lo.end(), lo.begin(), ::tolower);
            if (isFuncName(lo))
                tokens.push_back({TokenType::FUNC, lo});
            else
                tokens.push_back({TokenType::VARIABLE, word});
            continue;
        }

        // Цифра — число, возможно дробь a/b
        if (std::isdigit((unsigned char)c)) {
            std::string num;
            while (i < n && std::isdigit((unsigned char)expr[i]))
                num += expr[i++];
            // Проверяем дробь
            if (i < n && expr[i] == '/') {
                size_t j = i + 1;
                if (j < n && std::isdigit((unsigned char)expr[j])) {
                    num += '/';
                    ++i;
                    while (i < n && std::isdigit((unsigned char)expr[i]))
                        num += expr[i++];
                }
            }
            tokens.push_back({TokenType::NUMBER, num});
            continue;
        }

        // Унарный минус / бинарный минус — разбираем как MINUS, парсер решит
        switch (c) {
            case '+': tokens.push_back({TokenType::PLUS,   "+"}); ++i; break;
            case '-': tokens.push_back({TokenType::MINUS,  "-"}); ++i; break;
            case '*': tokens.push_back({TokenType::STAR,   "*"}); ++i; break;
            case '/': tokens.push_back({TokenType::SLASH,  "/"}); ++i; break;
            case '^': tokens.push_back({TokenType::CARET,  "^"}); ++i; break;
            case '(': tokens.push_back({TokenType::LPAREN, "("}); ++i; break;
            case ')': tokens.push_back({TokenType::RPAREN, ")"}); ++i; break;
            case ',': tokens.push_back({TokenType::COMMA,  ","}); ++i; break;
            default:
                throw std::runtime_error(
                    std::string("Tokenizer: unexpected char '") + c + "'");
        }
    }
    return tokens;
}
