#include <gtest/gtest.h>
#include "algstructures/Integer.h"

#include "Exceptions/UniversalStringException.h"


// Z1 - abs
TEST(IntegerAbs, Basic) {
    Integer pos("123");
    Integer neg("-123");
    EXPECT_EQ(pos.abs().toString(), "123");
    EXPECT_EQ(neg.abs().toString(), "123");
}

// Z2 - getSign
TEST(IntegerGetSign, Basic) {
    Integer pos("123");
    Integer neg("-123");
    Integer zero("0");
    EXPECT_EQ(pos.getSign(), 2);  // положительное
    EXPECT_EQ(neg.getSign(), 1);  // отрицательное
    EXPECT_EQ(zero.getSign(), 0); // ноль
}

// Z3 - унарный минус
TEST(IntegerNegate, Basic) {
    Integer pos("123");
    Integer neg("-123");
    Integer zero("0");
    EXPECT_EQ((-pos).toString(), "-123");
    EXPECT_EQ((-neg).toString(), "123");
    EXPECT_EQ((-zero).toString(), "0");
}

// Z4 - fromNatural
TEST(IntegerFromNatural, Basic) {
    Natural nat("456");
    Integer result = Integer::fromNatural(nat);
    EXPECT_EQ(result.toString(), "456");
    EXPECT_FALSE(result.isNegative());
}

// Z5 - toNatural
TEST(IntegerToNatural, Basic) {
    Integer pos("789");
    EXPECT_EQ(pos.toNatural().toString(), "789");
}

TEST(IntegerToNatural, ThrowsOnNegative) {
    Integer neg("-123");
    EXPECT_THROW(neg.toNatural(), UniversalStringException);
}

// Z6 - сложение
TEST(IntegerAdd, PositivePositive) {
    Integer a("100");
    Integer b("50");
    EXPECT_EQ((a + b).toString(), "150");
}

TEST(IntegerAdd, NegativeNegative) {
    Integer a("-100");
    Integer b("-50");
    EXPECT_EQ((a + b).toString(), "-150");
}

TEST(IntegerAdd, PositiveNegative) {
    Integer a("100");
    Integer b("-30");
    EXPECT_EQ((a + b).toString(), "70");
}

TEST(IntegerAdd, NegativePositive) {
    Integer a("-100");
    Integer b("30");
    EXPECT_EQ((a + b).toString(), "-70");
}

TEST(IntegerAdd, ResultZero) {
    Integer a("50");
    Integer b("-50");
    EXPECT_EQ((a + b).toString(), "0");
}

// Z7 - вычитание
TEST(IntegerSubtract, Basic) {
    Integer a("100");
    Integer b("30");
    EXPECT_EQ((a - b).toString(), "70");
}

TEST(IntegerSubtract, ResultNegative) {
    Integer a("30");
    Integer b("100");
    EXPECT_EQ((a - b).toString(), "-70");
}

TEST(IntegerSubtract, NegativeNumber) {
    Integer a("-50");
    Integer b("30");
    EXPECT_EQ((a - b).toString(), "-80");
}

// Z8 - умножение
TEST(IntegerMultiply, PositivePositive) {
    Integer a("12");
    Integer b("5");
    EXPECT_EQ((a * b).toString(), "60");
}

TEST(IntegerMultiply, PositiveNegative) {
    Integer a("12");
    Integer b("-5");
    EXPECT_EQ((a * b).toString(), "-60");
}

TEST(IntegerMultiply, NegativeNegative) {
    Integer a("-12");
    Integer b("-5");
    EXPECT_EQ((a * b).toString(), "60");
}

TEST(IntegerMultiply, WithZero) {
    Integer a("123");
    Integer zero("0");
    EXPECT_EQ((a * zero).toString(), "0");
}

// Z9 - деление
TEST(IntegerQuotient, PositivePositive) {
    Integer a("100");
    Integer b("23");
    EXPECT_EQ((a / b).toString(), "4");
}

TEST(IntegerQuotient, PositiveNegative) {
    Integer a("100");
    Integer b("-23");
    EXPECT_EQ((a / b).toString(), "-4");
}

TEST(IntegerQuotient, NegativePositive) {
    Integer a("-100");
    Integer b("23");
    EXPECT_EQ((a / b).toString(), "-4");
}

TEST(IntegerQuotient, ThrowsOnZero) {
    Integer a("100");
    Integer zero("0");
    EXPECT_THROW(a / zero, UniversalStringException);
}

// Z10 - остаток
TEST(IntegerRemainder, Basic) {
    Integer a("100");
    Integer b("23");
    EXPECT_EQ((a % b).toString(), "8");
}

TEST(IntegerRemainder, ThrowsOnZero) {
    Integer a("100");
    Integer zero("0");
    EXPECT_THROW(a % zero, UniversalStringException);
}