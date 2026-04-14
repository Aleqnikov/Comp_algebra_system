#include "AhoCorasick.h"


void AhoCorasick::addPattern(const std::string& pattern, Token token) {
    if (pattern.empty()) {
        throw AhoCorasickException("Cannot add empty pattern");
    }
    auto current = root;
    for (char ch : pattern) {
        if (current->children.find(ch) == current->children.end()) {
            current->children[ch] = std::make_shared<AhoCorasickNode>();
        }
        current = current->children[ch];
    }
    current->output = token;
    built = false; // Требуется перестройка fail-ссылок, если будем использовать search
}

void AhoCorasick::buildFailLinks() {
    std::queue<std::shared_ptr<AhoCorasickNode>> q;
    for (auto& [ch, child] : root->children) {
        child->fail = root;
        q.push(child);
    }
    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        for (auto& [ch, child] : current->children) {
            q.push(child);
            auto fail = current->fail;
            while (fail != nullptr && fail->children.find(ch) == fail->children.end()) {
                fail = fail->fail;
            }
            child->fail = (fail == nullptr) ? root : fail->children[ch];
        }
    }
    built = true;
}

/**
 * @brief Находит самый длинный паттерн, который совпадает с началом текста
 * @param text Полный текст
 * @param startPos Позиция, с которой начинать проверку
 * @return Пара {Длина совпадения, Токен}. Если не найдено - длина 0.
 */
std::pair<size_t, std::optional<Token>> AhoCorasick::findLongestPrefix(const std::string& text, size_t startPos) const {
    auto current = root;
    size_t maxLen = 0;
    std::optional<Token> bestToken = std::nullopt;
    size_t currentLen = 0;

    for (size_t i = startPos; i < text.size(); ++i) {
        char ch = text[i];
        auto it = current->children.find(ch);
        if (it == current->children.end()) {
            break; // Совпадение прервалось
        }
        current = it->second;
        currentLen++;

        if (current->output.has_value()) {
            maxLen = currentLen;
            bestToken = current->output;
        }
    }
    return {maxLen, bestToken};
}
