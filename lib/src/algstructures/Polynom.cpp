#include "Polynom.h"
#include "../Exceptions/UniversalStringException.h"
#include <algorithm>

Polynom::Polynom(const std::vector<Rational>& coefficients)
    : coefficients_(coefficients) {

    while (coefficients_.size() > 1 && 
           coefficients_.back().getNumerator().getSign() == 0) {
        coefficients_.pop_back();
    }
}

const std::vector<Rational>& Polynom::getCoefficients() const noexcept {
    return coefficients_;
}

std::string Polynom::toString() const {
    if (coefficients_.empty())
        throw UniversalStringException("Polynom: polynomial cannot have empty coefficients");
    
    std::string result;
    
    for (int i = coefficients_.size() - 1; i >= 0; --i) {
        const Rational& coeff = coefficients_[i];
        
        if (coeff.getNumerator().getSign() == 0 && 
            !(i == 0 && coefficients_.size() == 1)) {
            continue;
        }
        
        if (i != static_cast<int>(coefficients_.size()) - 1) {
            result += " ";
            result += (coeff.getNumerator().getSign() == 1) ? "- " : "+ ";
        } else if (coeff.getNumerator().getSign() == 1) {
            result += "-";
        }
        
        std::string coeff_str;
        if (coeff.getDenominator().toString() == "1") {
            coeff_str = coeff.getNumerator().abs().toString();
        } else {
            Rational abs_coeff(Integer::fromNatural(coeff.getNumerator().abs()), coeff.getDenominator());
            coeff_str = abs_coeff.toString();
        }
        
        result += coeff_str;
        
        if (i > 0) {
            result += "x";
            if (i > 1) {
                result += "^" + std::to_string(i);
            }
        }
    }
    
    return result;
}

std::size_t Polynom::getDegree() const {
    return coefficients_.size() - 1;
}

Rational Polynom::getLeadingCoefficient() const {
    return coefficients_.back();
}

// P1: Сложение
Polynom Polynom::operator+(const Polynom& other) const {
    const std::vector<Rational>& longer = (coefficients_.size() >= other.coefficients_.size())
                                          ? coefficients_
                                          : other.coefficients_;
    const std::vector<Rational>& shorter = (coefficients_.size() >= other.coefficients_.size())
                                           ? other.coefficients_
                                           : coefficients_;
    
    std::vector<Rational> result;
    result.reserve(longer.size());
    
    for (size_t i = 0; i < shorter.size(); ++i) {
        result.push_back(longer[i] + shorter[i]);
    }
    
    for (size_t i = shorter.size(); i < longer.size(); ++i) {
        result.push_back(longer[i]);
    }
    
    while (result.size() > 1 && result.back().getNumerator().getSign() == 0) {
        result.pop_back();
    }
    
    for (auto& r : result) {
        r.reduce();
    }
    
    return Polynom(result);
}

Polynom Polynom::operator-(const Polynom& other) const {
    std::vector<Rational> negated;
    negated.reserve(other.coefficients_.size());
    
    for (const Rational& coeff : other.coefficients_) {
        negated.push_back(Rational(-coeff.getNumerator(), coeff.getDenominator()));
    }
    
    return *this + Polynom(negated);
}

Polynom Polynom::operator*(const Rational& rational) const {
    if (rational.getNumerator().getSign() == 0) {
        return Polynom({Rational(Integer("0"), Natural("1"))});
    }
    
    std::vector<Rational> result;
    result.reserve(coefficients_.size());
    
    for (const Rational& coeff : coefficients_) {
        Rational product = coeff * rational;
        product.reduce();
        result.push_back(product);
    }
    
    return Polynom(result);
}

Polynom Polynom::multiplyByXPower(std::size_t k) const {
    std::vector<Rational> result;
    
    try {
        result.reserve(k + coefficients_.size());
    } catch (const std::bad_alloc&) {
        throw UniversalStringException("Polynom:  not enough memory for polynomial multiplication");
    }
    
    Rational zero(Integer("0"), Natural("1"));
    for (size_t i = 0; i < k; ++i) {
        result.push_back(zero);
    }
    
    for (const Rational& coeff : coefficients_) {
        result.push_back(coeff);
    }
    
    return Polynom(result);
}

Polynom Polynom::factorOut() const {
    Natural gcd_num = coefficients_[0].getNumerator().abs();
    Natural lcm_denom = coefficients_[0].getDenominator();
    
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        gcd_num = Natural::gcd(gcd_num, coefficients_[i].getNumerator().abs());
        lcm_denom = Natural::lcm(lcm_denom, coefficients_[i].getDenominator());
    }
    
    Rational factor(Integer::fromNatural(lcm_denom), gcd_num);
    return *this * factor;
}

Polynom Polynom::operator*(const Polynom& other) const {
    Rational zero(Integer("0"), Natural("1"));
    
    bool this_zero = (coefficients_.size() == 1 && coefficients_[0].getNumerator().getSign() == 0);
    bool other_zero = (other.coefficients_.size() == 1 && other.coefficients_[0].getNumerator().getSign() == 0);
    
    if (this_zero || other_zero)
        return Polynom({zero});
    
    size_t n = coefficients_.size();
    size_t m = other.coefficients_.size();
    
    std::vector<Rational> result;
    try {
        result.assign(n + m - 1, zero);
    } catch (const std::bad_alloc&) {
        throw UniversalStringException("Polynom:  not enough memory for polynomial multiplication");
    }
    
    for (size_t i = 0; i < n; ++i) {
        if (coefficients_[i].getNumerator().getSign() == 0) continue;
        
        for (size_t j = 0; j < m; ++j) {
            if (other.coefficients_[j].getNumerator().getSign() == 0) continue;
            
            Rational product = coefficients_[i] * other.coefficients_[j];
            result[i + j] = result[i + j] + product;
        }
    }
    
    return Polynom(result);
}


Polynom Polynom::operator/(const Polynom& other) const {
    Rational zero(Integer("0"), Natural("1"));
    
    if (other.coefficients_.back().getNumerator().getSign() == 0)
        throw UniversalStringException("Polynom:  cannot divide by zero polynomial");
    
    size_t divisor_size = other.coefficients_.size();
    size_t dividend_size = coefficients_.size();
    
    if (dividend_size < divisor_size)
        return Polynom({zero});
    
    std::vector<Rational> remainder = coefficients_;
    std::vector<Rational> quotient(dividend_size - divisor_size + 1, zero);
    
    const Rational& divisor_leading = other.coefficients_.back();
    
    for (size_t pos = dividend_size; pos >= divisor_size; --pos) {
        size_t quotient_idx = pos - divisor_size;
        
        if (remainder[pos - 1].getNumerator().getSign() == 0)
            continue;
        
        Rational coeff = remainder[pos - 1] / divisor_leading;
        quotient[quotient_idx] = coeff;
        
        for (size_t j = 0; j < divisor_size; ++j) {
            Rational product = other.coefficients_[j] * coeff;
            remainder[quotient_idx + j] = remainder[quotient_idx + j] - product;
        }
    }
    
    while (quotient.size() > 1 && quotient.back().getNumerator().getSign() == 0) {
        quotient.pop_back();
    }
    
    return Polynom(quotient);
}

Polynom Polynom::operator%(const Polynom& other) const {
    Polynom quotient = *this / other;
    Polynom product = quotient * other;
    return *this - product;
}

Polynom Polynom::gcd(const Polynom& a, const Polynom& b) {
    if ((a.getDegree() == 0 && a.coefficients_[0].getNumerator().getSign() == 0) ||
        (b.getDegree() == 0 && b.coefficients_[0].getNumerator().getSign() == 0)) {
        throw UniversalStringException("Polynom:   cannot compute GCD with zero polynomial");
    }
    
    Polynom first = a;
    Polynom second = b;
    
    while (!(second.getDegree() == 0 && second.coefficients_[0].getNumerator().getSign() == 0)) {
        Polynom temp = first % second;
        first = second;
        second = temp;
    }
    
    Rational leading = first.getLeadingCoefficient();
    Rational one(Integer("1"), Natural("1"));
    Rational inverse = one / leading;
    
    return first * inverse;
}

Polynom Polynom::derivative() const {
    if (getDegree() == 0) {
        return Polynom({Rational(Integer("0"), Natural("1"))});
    }
    
    std::vector<Rational> result;
    result.reserve(coefficients_.size() - 1);
    
    for (size_t i = 1; i < coefficients_.size(); ++i) {
        Rational power(Integer(std::to_string(i)), Natural("1"));
        result.push_back(coefficients_[i] * power);
    }
    
    return Polynom(result);
}

Polynom Polynom::makeSquareFree() const {
    if (getDegree() == 0)
        return *this;
    
    Polynom deriv = derivative();
    Polynom gcd_poly = gcd(*this, deriv);
    return *this / gcd_poly;
}