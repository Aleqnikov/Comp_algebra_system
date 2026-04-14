#ifndef AHO_CORASICK_H
#define AHO_CORASICK_H

#include <map>
#include <memory>
#include <queue>
#include <string>
#include <optional>
#include "token_definitions.h"
#include "../../Exceptions/UniversalStringException.h"


class AhoCorasickNode {
public:
    std::map<char, std::shared_ptr<AhoCorasickNode>> children;
    std::shared_ptr<AhoCorasickNode> fail;
    std::optional<Token> output;

    AhoCorasickNode() : fail(nullptr), output(std::nullopt) {}
};


class AhoCorasick {
private:
    std::shared_ptr<AhoCorasickNode> root;
    bool built;

public:
    AhoCorasick() : root(std::make_shared<AhoCorasickNode>()), built(false) {}

    void addPattern(const std::string& pattern, Token token);

    void buildFailLinks();

    /**
     * @brief Находит самый длинный паттерн, который совпадает с началом текста
     * @param text Полный текст
     * @param startPos Позиция, с которой начинать проверку
     * @return Пара {Длина совпадения, Токен}. Если не найдено - длина 0.
     */
    std::pair<size_t, std::optional<Token>> findLongestPrefix(const std::string& text, size_t startPos) const;
};

#endif // AHO_CORASICK_H