#ifndef RATIONAL_UTILS_H
#define RATIONAL_UTILS_H

#include "algstructures/Rational.h"
#include <string>

// Красивый вывод: если знаменатель == 1, выводим только числитель
inline std::string rationalStr(const Rational& r) {
    if (r.isInteger()) return r.getNumerator().toString();
    return r.toString();
}

// Проверки через числитель/знаменатель, не через строку
inline bool rationalIsZero(const Rational& r) {
    return r.getNumerator().toString() == "0";
}

inline bool rationalIsOne(const Rational& r) {
    return r.isInteger() && r.getNumerator().toString() == "1";
}

#endif
