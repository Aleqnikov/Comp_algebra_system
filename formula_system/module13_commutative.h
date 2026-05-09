#ifndef MODULE13_COMMUTATIVE_H
#define MODULE13_COMMUTATIVE_H

#include "formula_tree.h"

// М13: перестановка множителей в каждом узле MUL в порядке:
// 1. Числовой коэффициент (NUMBER)
// 2. Переменные в лексикографическом порядке
// 3. sqrt от переменных (рядом со своей переменной)
// 4. Степенные функции (POW)
// 5. Показательные (EXP)
// 6. Логарифмы (LOG)
// 7. Тригонометрические: SIN, COS, TAN, COT
NodePtr sortFactors(const NodePtr& node);

#endif
