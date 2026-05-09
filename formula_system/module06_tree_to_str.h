#ifndef MODULE06_TREE_TO_STR_H
#define MODULE06_TREE_TO_STR_H

#include "formula_tree.h"
#include <string>

// М6: дерево → полная скобочная инфиксная запись
// Пример: (sin(x) + (3 * (x ^ 2)))
std::string treeToFullString(const NodePtr& node);

#endif
