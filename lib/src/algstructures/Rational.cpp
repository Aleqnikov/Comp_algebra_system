#include "Rational.h"
#include "../Exceptions/UniversalStringException.h"

Rational::Rational(const Integer& numerator, const Natural& denominator)
    : numerator_(numerator), denominator_(denominator) {
    if (!(denominator_ != 0))
        throw UniversalStringException("Rational: denominator cannot be zero");
}


Rational::Rational(const std::string& str) 
    : numerator_("0"), denominator_("1") {
    size_t delimiter_pos = str.find('/');
    
    if (delimiter_pos == std::string::npos) {
        numerator_ = Integer(str);
        denominator_ = Natural("1");
    } else {

        std::string num_str = str.substr(0, delimiter_pos);
        std::string denom_str = str.substr(delimiter_pos + 1);
        numerator_ = Integer(num_str);
        denominator_ = Natural(denom_str);
        
        if (!(denominator_ != 0))
            throw UniversalStringException("Rational:  denominator cannot be zero");
    }
}

std::string Rational::toString() const {
    return numerator_.toString() + "/" + denominator_.toString();
}

const Integer& Rational::getNumerator() const noexcept {
    return numerator_;
}

const Natural& Rational::getDenominator() const noexcept {
    return denominator_;
}

void Rational::reduce() {
    Natural numerator_abs = numerator_.abs();
    Natural gcd = Natural::gcd(numerator_abs, denominator_);

    if (gcd.toString() == "1")
        return;

    Integer gcd_as_int = Integer::fromNatural(gcd);
    numerator_ = numerator_ / gcd_as_int;
    denominator_ = denominator_ / gcd;
}

bool Rational::isInteger() const {
    return denominator_.toString() == "1";
}

Rational Rational::fromInteger(const Integer& integer) {
    return Rational(integer, Natural("1"));
}

Integer Rational::toInteger() const {
    if (!isInteger())
        throw UniversalStringException("Rational:  cannot convert fraction to integer, denominator must be 1");
    return numerator_;
}

Rational Rational::operator+(const Rational& other) const {
    Natural common_denom = Natural::lcm(denominator_, other.denominator_);
    
    Natural factor_this = common_denom / denominator_;
    Natural factor_other = common_denom / other.denominator_;

    Integer new_num_this = numerator_ * Integer::fromNatural(factor_this);
    Integer new_num_other = other.numerator_ * Integer::fromNatural(factor_other);
    
    Integer sum_numerator = new_num_this + new_num_other;
    
    Rational result(sum_numerator, common_denom);
    result.reduce();
    return result;
}


Rational Rational::operator-(const Rational& other) const {
    Natural common_denom = Natural::lcm(denominator_, other.denominator_);
    
    Natural factor_this = common_denom / denominator_;
    Natural factor_other = common_denom / other.denominator_;
    
    Integer new_num_this = numerator_ * Integer::fromNatural(factor_this);
    Integer new_num_other = other.numerator_ * Integer::fromNatural(factor_other);
    
    Integer diff_numerator = new_num_this - new_num_other;
    
    Rational result(diff_numerator, common_denom);
    result.reduce();
    return result;
}

Rational Rational::operator*(const Rational& other) const {
    Integer new_numerator = numerator_ * other.numerator_;
    Natural new_denominator = denominator_ * other.denominator_;
    
    Rational result(new_numerator, new_denominator);
    result.reduce();
    return result;
}

Rational Rational::operator/(const Rational& other) const {
    if (other.numerator_.getSign() == 0)
        throw UniversalStringException("Rational:  cannot divide by zero");
    
    Integer new_numerator = numerator_ * Integer::fromNatural(other.denominator_);
    
    Natural other_num_abs = other.numerator_.abs();
    Natural new_denominator = denominator_ * other_num_abs;
    
    if (other.numerator_.isNegative())
        new_numerator = -new_numerator;
    
    Rational result(new_numerator, new_denominator);
    result.reduce();
    return result;
}