#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <vector>
#include <string>
#include "token_definitions.h"
#include "latex_lexemes.h"
#include "AhoCorasick.h"
#include "../../Exceptions/UniversalStringException.h"

class Tokenizer {
private:
    AhoCorasick automaton;

    static bool isDigit(char ch) {
        return ch >= '0' && ch <= '9';
    }

    static bool isAlpha(char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    static bool isWhitespace(char ch) {
        return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
    }

public:
    Tokenizer() {
        try {
            for (const auto& [pattern, token] : LATEX_LEXEMES_MAP) {
                automaton.addPattern(pattern, token);
            }
            automaton.buildFailLinks();
        } catch (const std::exception& e) {
            throw TokenizerException(std::string("Init failed: ") + e.what());
        }
    }

    std::vector<TokenResult> tokenize(const std::string& text);
};

// Глобальный интерфейс 
inline std::vector<TokenResult> tokenize(const std::string& text) {
    static Tokenizer tokenizer;
    return tokenizer.tokenize(text);
}

#endif // TOKENIZER_H