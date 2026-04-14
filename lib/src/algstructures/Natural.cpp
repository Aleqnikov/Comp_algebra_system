#include "Natural.h"
#include "../Exceptions/UniversalStringException.h"
#include <cmath>
#include <algorithm>

std::string Natural::toString() const {
    if (this->nums_.empty())
        throw UniversalStringException("Natural: atypical behavior, the vector of numbers should not be empty");
    std::string result(this->nums_.size(), '0');
    for (int i = this->nums_.size() - 1; i >= 0; --i) {
        result[this->nums_.size() - i - 1] = '0' + this->nums_.at(i);
    }
    return result;
}

Natural::Natural(const std::string& str) {
    if (str.empty())
        throw UniversalStringException("Natural:  wrong argument, the string should not be empty");
    nums_.resize(str.size());
    for (std::size_t i = 0; i < str.size(); ++i) {
        char c = str[str.size() - 1 - i];
        if (c < '0' || c > '9')
            throw UniversalStringException("Natural:  wrong argument, string contains non-digit character");
        nums_[i] = c - '0';
    }
    while (nums_.size() > 1 && nums_.back() == 0)
        nums_.pop_back();
}

const std::vector<uint8_t>& Natural::getNums() const noexcept {
    return this->nums_;
}

Natural::Natural(const std::vector<uint8_t>& CpNumbers) {
    if (CpNumbers.empty())
        throw UniversalStringException("Natural:  wrong argument, the vector of numbers should not be empty");
    this->nums_ = CpNumbers;
    while (this->nums_.size() > 1 && this->nums_.back() == 0) {
        this->nums_.pop_back();
    }
}

uint8_t Natural::cmp(const Natural* other) const {
    if (this->nums_.size() > other->nums_.size()) return 2;
    if (this->nums_.size() < other->nums_.size()) return 1;
    for (size_t i = this->nums_.size(); i-- > 0;) {
        uint8_t a = this->nums_[i];
        uint8_t b = other->nums_[i];
        if (a > b) return 2;
        if (a < b) return 1;
    }
    return 0;
}

bool Natural::operator!=(std::size_t val) const {
    if (val == 0)
        return !(nums_.size() == 1 && nums_[0] == 0);
    return true;
}

void Natural::operator++() {
    uint8_t carry = 1;
    for (size_t i = 0; i < this->nums_.size() && carry; ++i) {
        uint8_t sum = this->nums_[i] + carry;
        this->nums_[i] = sum % 10;
        carry = sum / 10;
    }
    if (carry) this->nums_.push_back(carry);
}

Natural Natural::operator+(const Natural& other) const {
    size_t n = this->nums_.size();
    size_t m = other.nums_.size();
    size_t maxlen = std::max(n, m);
    std::vector<uint8_t> res;
    res.reserve(maxlen + 1);
    uint8_t carry = 0;
    for (size_t i = 0; i < maxlen; ++i) {
        uint8_t a = (i < n) ? this->nums_[i] : 0;
        uint8_t b = (i < m) ? other.nums_[i] : 0;
        uint8_t s = a + b + carry;
        res.push_back(s % 10);
        carry = s / 10;
    }
    if (carry) res.push_back(carry);
    return Natural(res);
}


Natural Natural::operator-(const Natural& other) const {
    uint8_t comparison = this->cmp(&other);
    if (comparison == 1) {
        throw UniversalStringException("Natural:  subtrahend larger than minuend");
    }
    if (comparison == 0) return Natural(std::vector<uint8_t>{0});
    std::vector<uint8_t> res(this->nums_.size(), 0);
    int borrow = 0;
    for (size_t i = 0; i < this->nums_.size(); ++i) {
        int a = this->nums_[i];
        int b = (i < other.nums_.size()) ? other.nums_[i] : 0;
        int diff = a - b - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
        res[i] = static_cast<uint8_t>(diff);
    }
    return Natural(res);
}


Natural Natural::operator*(std::size_t b) const {
    if (b > 9) {
        throw UniversalStringException("Natural:  digit out of range (" + std::to_string(b) + ")");
    }
    if (b == 0) return Natural(std::vector<uint8_t>{0});
    std::vector<uint8_t> res;
    res.reserve(this->nums_.size() + 1);
    unsigned int carry = 0;
    for (size_t i = 0; i < this->nums_.size(); ++i) {
        unsigned int prod = static_cast<unsigned int>(this->nums_[i]) * b + carry;
        res.push_back(static_cast<uint8_t>(prod % 10));
        carry = prod / 10;
    }
    while (carry > 0) {
        res.push_back(static_cast<uint8_t>(carry % 10));
        carry /= 10;
    }
    return Natural(res);
}

Natural Natural::multiplyByPowerOfTen(std::size_t k) const {

	if (!(*this != 0))
		return *this;

	if (this->nums_.size() > SIZE_MAX - k) {
		throw UniversalStringException("Natural:  The size of number is greater than " + std::to_string(SIZE_MAX));
	}

	std::vector<uint8_t> res;
	res.reserve(this->nums_.size() + k);

	res.assign(k, 0);
	res.insert(res.end(), this->nums_.begin(), this->nums_.end());

	return Natural(res);
}

Natural Natural::operator*(const Natural& other) const {
    if ((this->nums_.size() == 1 && this->nums_[0] == 0) ||
        (other.nums_.size() == 1 && other.nums_[0] == 0)) {
        return Natural(std::vector<uint8_t>{0});
    }
    std::vector<uint8_t> res(this->nums_.size() + other.nums_.size(), 0);
    for (size_t i = 0; i < nums_.size(); ++i) {
        uint8_t carry = 0;
        for (size_t j = 0; j < other.nums_.size() || carry; ++j) {
            unsigned long long cur = res[i + j] +
                                     static_cast<unsigned long long>(nums_[i]) *
                                     (j < other.nums_.size() ? other.nums_[j] : 0) +
                                     carry;
            res[i + j] = static_cast<uint8_t>(cur % 10);
            carry = static_cast<uint8_t>(cur / 10);
        }
    }
    while (res.size() > 1 && res.back() == 0)
        res.pop_back();
    return Natural(res);
}

Natural Natural::subtractMultiplied(const Natural& other, std::size_t c) const {
    if (c > 9)
        throw UniversalStringException("Natural:  The multiplier is not a digit from 0 to 9!");
    Natural multiplied = other * c;
    if (cmp(&multiplied) == 1)
        throw UniversalStringException("Natural:  the subtracted is greater than the reduced");
    return *this - multiplied;
}

Natural Natural::getFirstDivisionDigit(const Natural& other) const {
	if (other == Natural("0")) {
		throw UniversalStringException("Natural:  Division by zero!");
	}
	if (*this < other) {
		throw UniversalStringException("Natural:  First less than second");
	}

	std::size_t k = nums_.size() - other.getNums().size();
	Natural temp = other.multiplyByPowerOfTen(k);

	if (*this < temp) {
		temp = other.multiplyByPowerOfTen(--k);
	}

	for (uint8_t d = 9; d >= 1; --d) {

		if (!(temp * static_cast<std::size_t>(d) > *this)) {
			return Natural(std::vector<uint8_t>{d});
		}
	}

	return Natural("0");
}

Natural Natural::operator/(const Natural& other) const {
    if (other == Natural("0")) {
        throw UniversalStringException("Natural: can not divide by zero");
    }
    if (this->cmp(&other) == 1) {
        return Natural(std::vector<uint8_t>{0});
    }
    Natural current(std::vector<uint8_t>{0});
    std::vector<uint8_t> result;
    result.reserve(nums_.size());
    for (int i = static_cast<int>(nums_.size()) - 1; i >= 0; --i) {
        current = current.multiplyByPowerOfTen(1);
        current = current + Natural(std::vector<uint8_t>{nums_[i]});
        uint8_t q = 0;
        if (current.cmp(&other) != 1) {
            for (int digit = 9; digit >= 1; --digit) {
                Natural candidate = other * static_cast<std::size_t>(digit);
                try {
                    Natural newCurrent = current - candidate;
                    q = static_cast<uint8_t>(digit);
                    current = std::move(newCurrent);
                    break;
                } catch (const UniversalStringException&) {
                    continue;
                }
            }
        }
        result.push_back(q);
    }
    std::reverse(result.begin(), result.end());
    while (result.size() > 1 && result.back() == 0) {
        result.pop_back();
    }
    return Natural(result);
}

Natural Natural::operator%(const Natural& other) const {
    if (other == Natural("0")) {
        throw UniversalStringException("Natural:  can not divide by zero");
    }
    if (cmp(&other) == 1) return *this;
    Natural q = *this / other;
    Natural product = other * q;
    return *this - product;
}

Natural Natural::gcd(const Natural& a, const Natural& b) {
    Natural first = a;
    Natural second = b;
    if (first == Natural("0") && second == Natural("0")) {
        throw UniversalStringException("Natural: the gcd for two zeros is not uniquely defined");
    }
    if (!(second != 0)) {
        return first;
    }
    while (second != 0) {
        Natural tmp = first % second;
        first = second;
        second = tmp;
    }
    return first;
}


Natural Natural::lcm(const Natural& a, const Natural& b) {
    if (a == Natural("0") || b == Natural("0")) {
        throw UniversalStringException("Natural:  the lcm for zeros is not uniquely defined");
    }
    return (a * b) / gcd(a, b);
}