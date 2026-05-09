#ifndef MODULE08_PRIORITY_H
#define MODULE08_PRIORITY_H

#include "formula_tree.h"
#include <string>

// М8: построить строку формулы, удалив лишние скобки
// на основе приоритета операций и ассоциативности.
std::string treeToStringPriority(const NodePtr& node);

#endif
