#include "Integer.h"
#include "../Exceptions/UniversalStringException.h"
#include <algorithm>

Integer::Integer(const std::string& str) 
    : natural_(str[0] == '-' ? str.substr(1) : str),
      is_negative_(str[0] == '-') {

    
    if (!(natural_ != 0))
        is_negative_ = false;
}

Integer::Integer(const Natural& nat, bool is_negative) 
    : natural_(nat), is_negative_(is_negative) {
    if (!(natural_ != 0))
        is_negative_ = false;
}


bool Integer::isNegative() const noexcept {
    return is_negative_;
}

std::string Integer::toString() const {
    return (is_negative_ ? "-" : "") + natural_.toString();
}

const Natural& Integer::getNatural() const noexcept {
    return natural_;
}

Natural Integer::abs() const {
    return natural_;
}

uint8_t Integer::getSign() const {
    if (!(natural_ != 0))
        return 0;
    return is_negative_ ? 1 : 2;
}

Integer Integer::operator-() const {
    if (!(natural_ != 0))
        return *this;
    return Integer(natural_, !is_negative_);
}

Integer Integer::fromNatural(const Natural& nat) {
    return Integer(nat, false);
}

Natural Integer::toNatural() const {
    if (is_negative_)
        throw UniversalStringException("Integer:  cannot convert negative integer to natural");
    return natural_;
}

Integer Integer::operator+(const Integer& other) const {
    Natural abs_this = this->abs();
    Natural abs_other = other.abs();
    
    if (this->getSign() == other.getSign()) {
        Natural sum = abs_this + abs_other;
        return Integer(sum, is_negative_);
    } else {
        uint8_t cmp = abs_this.cmp(&abs_other);
        if (cmp == 0) {
            return Integer(Natural("0"), false);
        } else if (cmp == 2) {
            Natural diff = abs_this - abs_other;
            return Integer(diff, is_negative_);
        } else {
            Natural diff = abs_other - abs_this;
            return Integer(diff, other.is_negative_);
        }
    }
}

Integer Integer::operator-(const Integer& other) const {
    Integer neg_other = -other;
    return *this + neg_other;
}

Integer Integer::operator*(const Integer& other) const {
    uint8_t sign_this = this->getSign();
    uint8_t sign_other = other.getSign();
    
    if (sign_this == 0 || sign_other == 0)
        return Integer(Natural("0"), false);
    
    Natural abs_this = this->abs();
    Natural abs_other = other.abs();
    Natural product = abs_this * abs_other;
    
    bool result_negative = (sign_this != sign_other);
    return Integer(product, result_negative);
}

Integer Integer::operator/(const Integer& other) const {
    if (other.getSign() == 0)
        throw UniversalStringException("Integer:  cannot divide by zero");
    
    Natural dividend = this->abs();
    Natural divisor = other.abs();
    
    if (dividend.cmp(&divisor) == 1)
        return Integer(Natural("0"), false);
    
    Natural quotient = dividend / divisor;
    Integer result(quotient, false);
    
    return (this->getSign() != other.getSign()) ? -result : result;
}

Integer Integer::operator%(const Integer& other) const {
    if (other.getSign() == 0)
        throw UniversalStringException("Integer: cannot divide by zero");
    
    Natural dividend_abs = this->abs();
    Natural divisor_abs = other.abs();
    
    if (dividend_abs.cmp(&divisor_abs) == 1) {
        if (this->isNegative()) {
            return Integer(divisor_abs - dividend_abs, false);
        }
        return *this;
    }
    
    Integer quotient_result = *this / other;
    Integer product = other * quotient_result;
    Integer remainder = *this - product;
    
    if (remainder.isNegative()) {
        remainder = remainder + Integer(divisor_abs, false);
    }
    
    return remainder;
}