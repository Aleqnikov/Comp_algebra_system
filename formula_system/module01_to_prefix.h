#ifndef MODULE01_TO_PREFIX_H
#define MODULE01_TO_PREFIX_H

#include "formula_tree.h"
#include <string>

// Разбор инфиксной строки → дерево (рекурсивный спуск).
// Бросает std::runtime_error при синтаксической ошибке.
NodePtr parseInfix(const std::string& expr);

// М1: инфиксная строка → префиксная строка
// Пример: "sin(x)+3*x^2" → "+ sin x * 3 ^ x 2"
std::string infixToPrefix(const std::string& expr);

#endif
