#ifndef NATURAL_H
#define NATURAL_H

/**
* @breif Класс натуральных чисел, реализация натуральных числе в рамках системы компютерной алгебры, обладает
* некоторым набором свойств, необходимым для работы все системы.
* Является базовым классом от которой зависят все последующие.
* Стуркутура хранения - это массив, который как бы характеризует представление числа в 10 сс.
*/


#include <vector>
#include <iostream>
#include <cstdint>



class Natural {
public:
    Natural (const std::vector<uint8_t> &numbers);
    Natural(const std::string& str);

    // Метод которые необходим для визуализации числа.
    std::string toString() const;

    const std::vector<uint8_t>& getNums() const noexcept;
    uint8_t cmp(const Natural* other) const;


    bool operator!=(std::size_t val) const;  // isNotEqualZero → != 0
    void operator++();                        // increment
    Natural operator+(const Natural& other) const;   // add
    Natural operator-(const Natural& other) const;   // subtract
    Natural operator*(std::size_t b) const;          // multiplyByDigit
    Natural operator*(const Natural& other) const;   // multiply
    Natural operator/(const Natural& other) const;   // quotient
    Natural operator%(const Natural& other) const;   // remainder

    Natural multiplyByPowerOfTen(std::size_t k) const;
    Natural subtractMultiplied(const Natural& other, std::size_t c) const;
    Natural getFirstDivisionDigit(const Natural& other) const;
    static Natural gcd(const Natural& a, const Natural& b);
    static Natural lcm(const Natural& a, const Natural& b);
    
    bool operator==(const Natural& other) const { return this->cmp(&other) == 0;}
    bool operator<(const Natural& other) const { return this->cmp(&other) == 1; }
    bool operator>(const Natural& other) const  { return other < *this; }
    bool operator<=(const Natural& other) const { return !(*this > other); }
    bool operator>=(const Natural& other) const { return !(*this < other); }
    bool operator!=(const Natural& other) const { return !(*this == other); }


private:
    std::vector<uint8_t> nums_;




};



#endif //NATURAL_H
