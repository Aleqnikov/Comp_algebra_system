#ifndef MODULE12_LIKE_TERMS_TREE_H
#define MODULE12_LIKE_TERMS_TREE_H

#include "formula_tree.h"

// М12: приведение подобных слагаемых по дереву.
// «Подобные» = одинаковая переменная-часть (сравнение postfix подстрок).
// Например: 3*x + 5*x → 8*x
// Стратегия: собрать все слагаемые с вершины ADD в плоский список,
// сгруппировать по постфиксному представлению «переменной части»,
// сложить коэффициенты.
NodePtr collectLikeTerms(const NodePtr& node);

#endif
