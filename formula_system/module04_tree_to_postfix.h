#ifndef MODULE04_TREE_TO_POSTFIX_H
#define MODULE04_TREE_TO_POSTFIX_H

#include "formula_tree.h"
#include <string>
#include <vector>

// М4: дерево → постфикс (вектор строк)
std::vector<std::string> treeToPostfix(const NodePtr& node);

// М4 удобная версия: → строка через пробел
std::string treeToPostfixStr(const NodePtr& node);

#endif
