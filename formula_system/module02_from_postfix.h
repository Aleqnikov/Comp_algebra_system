#ifndef MODULE02_FROM_POSTFIX_H
#define MODULE02_FROM_POSTFIX_H

#include "formula_tree.h"
#include <string>
#include <vector>

// Постфиксная запись — вектор токенов-строк.
// Числа: "3", "3/4", "-2"; переменные: "x";
// Операторы: "+","-","*","/","^","neg";
// Функции: "sin","cos","tan","cot","sqrt","exp","log"

// М2: постфикс (вектор строк) → дерево
NodePtr postfixToTree(const std::vector<std::string>& postfix);

// Вспомогательно: разбить строку постфикса по пробелам
std::vector<std::string> splitPostfix(const std::string& s);

#endif
