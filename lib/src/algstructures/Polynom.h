#ifndef POLYNOM_H
#define POLYNOM_H

#include "Rational.h"
#include <vector>
#include <string>

/**
 * @brief Данный класс описывает полиномы от одной переменной, над полем рациональных чисел.
 * Коэффициенты хранятся от младшей степени к старшей: {a0, a1, a2} = a0 + a1*x + a2*x^2
 */

class Polynom {
public:
    explicit Polynom(const std::vector<Rational>& coefficients);
    
    const std::vector<Rational>& getCoefficients() const noexcept;
    std::string toString() const;
    std::size_t getDegree() const;
    Rational getLeadingCoefficient() const;
    
    Polynom operator+(const Polynom& other) const;
    Polynom operator-(const Polynom& other) const;
    Polynom operator*(const Rational& rational) const;
    Polynom multiplyByXPower(std::size_t k) const;
    Polynom factorOut() const;
    Polynom operator*(const Polynom& other) const;
    Polynom operator/(const Polynom& other) const;
    Polynom operator%(const Polynom& other) const;
    static Polynom gcd(const Polynom& a, const Polynom& b);
    Polynom derivative() const;
    Polynom makeSquareFree() const;

private:
    std::vector<Rational> coefficients_;  // от младшей к старшей степени
};

#endif //POLYNOM_H