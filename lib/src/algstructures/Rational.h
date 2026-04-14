#ifndef RATIONAL_H
#define RATIONAL_H

#include "Integer.h"
#include "Natural.h"

/**
 * @brief Класс реализующий поле рациональных чисел, является основным классом с которым мы манипулируем в программе.
 * В будущем планируется построить архитектуру, позволяющую задавать и другие поля.
 */

class Rational {
public:
    Rational(const Integer& numerator, const Natural& denominator);
    Rational(const std::string& str);  // Формат: "123/456" или "123"
    

    std::string toString() const;
    const Integer& getNumerator() const noexcept;
    const Natural& getDenominator() const noexcept;

    void reduce();
    bool isInteger() const;
    static Rational fromInteger(const Integer& integer);
    Integer toInteger() const;
    
    Rational operator+(const Rational& other) const;
    Rational operator-(const Rational& other) const;
    Rational operator*(const Rational& other) const;
    Rational operator/(const Rational& other) const;

private:
    Integer numerator_;
    Natural denominator_;
};

#endif //RATIONAL_H