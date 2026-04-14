#include <gtest/gtest.h>
#include "algstructures/Polynom.h"

#include "Exceptions/UniversalStringException.h"

// Конструктор и геттеры
TEST(PolynomBasic, Constructor) {
    // 2 + 3x + x^2
    std::vector<Rational> coeffs = {
        Rational("2"),
        Rational("3"),
        Rational("1")
    };
    Polynom p(coeffs);
    EXPECT_EQ(p.getDegree(), 2);
    EXPECT_EQ(p.getLeadingCoefficient().toString(), "1/1");
}

TEST(PolynomBasic, ToString) {
    // 2 + 3x
    std::vector<Rational> coeffs = {Rational("2"), Rational("3")};
    Polynom p(coeffs);
    std::string s = p.toString();
    EXPECT_FALSE(s.empty());
}

// P1 - сложение
TEST(PolynomAdd, Basic) {
    // (1 + 2x) + (3 + 4x) = 4 + 6x
    Polynom p1({Rational("1"), Rational("2")});
    Polynom p2({Rational("3"), Rational("4")});
    Polynom result = p1 + p2;
    EXPECT_EQ(result.getCoefficients()[0].toString(), "4/1");
    EXPECT_EQ(result.getCoefficients()[1].toString(), "6/1");
}

TEST(PolynomAdd, DifferentDegrees) {
    // (1 + 2x) + (3) = 4 + 2x
    Polynom p1({Rational("1"), Rational("2")});
    Polynom p2({Rational("3")});
    Polynom result = p1 + p2;
    EXPECT_EQ(result.getDegree(), 1);
    EXPECT_EQ(result.getCoefficients()[0].toString(), "4/1");
}

// P2 - вычитание
TEST(PolynomSubtract, Basic) {
    // (5 + 3x) - (2 + x) = 3 + 2x
    Polynom p1({Rational("5"), Rational("3")});
    Polynom p2({Rational("2"), Rational("1")});
    Polynom result = p1 - p2;
    EXPECT_EQ(result.getCoefficients()[0].toString(), "3/1");
    EXPECT_EQ(result.getCoefficients()[1].toString(), "2/1");
}

// P3 - умножение на рациональное
TEST(PolynomMultiplyRational, Basic) {
    // (1 + 2x) * 3 = 3 + 6x
    Polynom p({Rational("1"), Rational("2")});
    Rational r("3");
    Polynom result = p * r;
    EXPECT_EQ(result.getCoefficients()[0].toString(), "3/1");
    EXPECT_EQ(result.getCoefficients()[1].toString(), "6/1");
}

TEST(PolynomMultiplyRational, Zero) {
    Polynom p({Rational("1"), Rational("2")});
    Rational zero("0");
    Polynom result = p * zero;
    EXPECT_EQ(result.getDegree(), 0);
    EXPECT_EQ(result.getCoefficients()[0].getNumerator().getSign(), 0);
}

// P4 - умножение на x^k
TEST(PolynomMultiplyXPower, Basic) {
    // (1 + 2x) * x^2 = x^2 + 2x^3
    Polynom p({Rational("1"), Rational("2")});
    Polynom result = p.multiplyByXPower(2);
    EXPECT_EQ(result.getDegree(), 3);
    EXPECT_EQ(result.getCoefficients()[0].getNumerator().getSign(), 0); // 0
    EXPECT_EQ(result.getCoefficients()[1].getNumerator().getSign(), 0); // 0
    EXPECT_EQ(result.getCoefficients()[2].toString(), "1/1");
    EXPECT_EQ(result.getCoefficients()[3].toString(), "2/1");
}

// P7 - вынесение НОД/НОК
TEST(PolynomFactorOut, Basic) {
    // 2/3 + 4/6x
    Polynom p({Rational("2/3"), Rational("4/6")});
    Polynom result = p.factorOut();
    EXPECT_EQ(result.getCoefficients()[0].getDenominator().toString(), "1");
    EXPECT_EQ(result.getCoefficients()[1].getDenominator().toString(), "1");
}

// P8 - умножение многочленов
TEST(PolynomMultiply, Basic) {
    // (1 + x) * (1 + x) = 1 + 2x + x^2
    Polynom p1({Rational("1"), Rational("1")});
    Polynom p2({Rational("1"), Rational("1")});
    Polynom result = p1 * p2;


    EXPECT_EQ(result.getDegree(), 2);
    EXPECT_EQ(result.getCoefficients()[0].toString(), "1/1");
    EXPECT_EQ(result.getCoefficients()[1].toString(), "2/1");
    EXPECT_EQ(result.getCoefficients()[2].toString(), "1/1");
}

// P9 - деление
TEST(PolynomDivide, Basic) {
    // (x^2 + 3x + 2) / (x + 1) = x + 2
    Polynom dividend({Rational("2"), Rational("3"), Rational("1")});
    Polynom divisor({Rational("1"), Rational("1")});
    Polynom result = dividend / divisor;
    EXPECT_EQ(result.getDegree(), 1);
}

TEST(PolynomDivide, ThrowsOnZero) {
    Polynom p({Rational("1"), Rational("1")});
    Polynom zero({Rational("0")});
    EXPECT_THROW(p / zero, UniversalStringException);
}

// P10 - остаток
TEST(PolynomRemainder, Basic) {
    // (x^2 + 3x + 3) / (x + 1) → остаток
    Polynom dividend({Rational("3"), Rational("3"), Rational("1")});
    Polynom divisor({Rational("1"), Rational("1")});
    Polynom remainder = dividend % divisor;
    EXPECT_EQ(remainder.getDegree(), 0);
}

// P11 - НОД
TEST(PolynomGCD, Basic) {
    // НОД(x^2 - 1, x - 1)
    Polynom p1({Rational("-1"), Rational("0"), Rational("1")});
    Polynom p2({Rational("-1"), Rational("1")});
    Polynom result = Polynom::gcd(p1, p2);
    EXPECT_EQ(result.getDegree(), 1);
}

// P12 - производная
TEST(PolynomDerivative, Basic) {
    // (x^3 + 2x^2 + 3x + 4)' = 3x^2 + 4x + 3
    Polynom p({Rational("4"), Rational("3"), Rational("2"), Rational("1")});
    Polynom deriv = p.derivative();
    EXPECT_EQ(deriv.getDegree(), 2);
    EXPECT_EQ(deriv.getCoefficients()[0].toString(), "3/1");
    EXPECT_EQ(deriv.getCoefficients()[1].toString(), "4/1");
    EXPECT_EQ(deriv.getCoefficients()[2].toString(), "3/1");
}

TEST(PolynomDerivative, Constant) {
    Polynom p({Rational("5")});
    Polynom deriv = p.derivative();
    EXPECT_EQ(deriv.getDegree(), 0);
    EXPECT_EQ(deriv.getCoefficients()[0].getNumerator().getSign(), 0);
}

// P13 - без кратных корней
TEST(PolynomMakeSquareFree, Basic) {
    // (x-1)^2 = x^2 - 2x + 1
    Polynom p({Rational("1"), Rational("-2"), Rational("1")});
    Polynom result = p.makeSquareFree();
    EXPECT_EQ(result.getDegree(), 1);
}