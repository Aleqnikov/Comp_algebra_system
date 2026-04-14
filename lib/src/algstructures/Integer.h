#ifndef INTEGER_H
#define INTEGER_H

#include "Natural.h"

/**
 * @brief Класс целых чисел, используется в программе для реализации поля рациональных чисел
 * Обладает всеми базовыми операциями, которые нужны.
 */

class Integer {
public:
    Integer(const std::string& str);
    Integer(const Natural& nat, bool is_negative);
    
    bool isNegative() const noexcept;
    std::string toString() const;
    const Natural& getNatural() const noexcept;
    
    Natural abs() const;
    uint8_t getSign() const;
    
    // Z3: Умножение на (-1)
    Integer operator-() const;  // negate → унарный минус

    static Integer fromNatural(const Natural& nat);
    Natural toNatural() const;

    Integer operator+(const Integer& other) const;
    Integer operator-(const Integer& other) const;
    Integer operator*(const Integer& other) const;
    Integer operator/(const Integer& other) const;
    Integer operator%(const Integer& other) const;

private:
    Natural natural_;
    bool is_negative_;
};

#endif //INTEGER_H