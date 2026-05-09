#ifndef MODULE07_ASSOC_H
#define MODULE07_ASSOC_H

#include "formula_tree.h"
#include <string>

// М7: построить строку формулы, удалив лишние скобки на основе
// ассоциативности умножения (a*b)*c = a*(b*c).
// Цепочки MUL/ADD пишутся без скобок.
std::string treeToStringAssoc(const NodePtr& node);

#endif
