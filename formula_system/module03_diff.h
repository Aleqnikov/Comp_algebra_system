#ifndef MODULE03_DIFF_H
#define MODULE03_DIFF_H

#include "formula_tree.h"
#include <string>

// М3: символьное дифференцирование дерева по переменной var.
// Возвращает новое дерево — производную.
NodePtr differentiate(const NodePtr& node, const std::string& var);

#endif
