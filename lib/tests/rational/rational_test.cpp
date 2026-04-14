#include <gtest/gtest.h>
#include "algstructures/Rational.h"

// --- 1. Конструкторы и строковое представление ---
TEST(RationalTest, ConstructionAndToString) {
    // Обычная дробь
    EXPECT_EQ(Rational("1/2").toString(), "1/2");
    // Целое число (должно превращаться в n/1)
    EXPECT_EQ(Rational("5").toString(), "5/1");
    // Отрицательное число
    EXPECT_EQ(Rational("-3/4").toString(), "-3/4");
    // Ноль
    EXPECT_EQ(Rational("0").toString(), "0/1");
}

// --- 2. Сокращение дробей (Метод reduce) ---
TEST(RationalTest, Reduction) {
    // 10/20 -> 1/2
    Rational r1("10/20");
    r1.reduce();
    EXPECT_EQ(r1.toString(), "1/2");

    // -5/5 -> -1/1
    Rational r2("-5/5");
    r2.reduce();
    EXPECT_EQ(r2.toString(), "-1/1");

    // Уже сокращенная
    Rational r3("7/3");
    r3.reduce();
    EXPECT_EQ(r3.toString(), "7/3");
}

// --- 3. Сложение и вычитание ---
TEST(RationalTest, AdditionSubtraction) {
    // 1/2 + 1/6 = 4/6 -> 2/3
    Rational a("1/2");
    Rational b("1/6");
    Rational resAdd = a + b;
    resAdd.reduce();
    EXPECT_EQ(resAdd.toString(), "2/3");

    // 1/4 - 1/2 = -1/4
    Rational c("1/4");
    Rational d("1/2");
    Rational resSub = c - d;
    resSub.reduce();
    EXPECT_EQ(resSub.toString(), "-1/4");

    // Сложение с отрицательным: 1/2 + (-1/2) = 0/1
    Rational resZero = Rational("1/2") + Rational("-1/2");
    resZero.reduce();
    EXPECT_EQ(resZero.toString(), "0/1");
}

// --- 4. Умножение и деление ---
TEST(RationalTest, MultiplicationDivision) {
    // 2/3 * 3/4 = 6/12 -> 1/2
    Rational a("2/3");
    Rational b("3/4");
    Rational resMul = a * b;
    resMul.reduce();
    EXPECT_EQ(resMul.toString(), "1/2");

    // 1/2 / 2/1 = 1/4
    Rational c("1/2");
    Rational d("2");
    Rational resDiv = c / d;
    resDiv.reduce();
    EXPECT_EQ(resDiv.toString(), "1/4");

    // Умножение на ноль
    EXPECT_EQ((Rational("5/8") * Rational("0")).toString(), "0/1");
}

// --- 5. Проверка на целое и конвертация ---
TEST(RationalTest, IntegerConversion) {
    // 10/2 — это целое (5)
    Rational r1("10/2");
    r1.reduce();
    EXPECT_TRUE(r1.isInteger());
    EXPECT_EQ(r1.toInteger().toString(), "5");

    // 1/2 — не целое
    EXPECT_FALSE(Rational("1/2").isInteger());
}

// --- 6. Пыточные кейсы (Ошибки) ---
TEST(RationalTest, EdgeCases) {
    // Деление на ноль
    EXPECT_ANY_THROW(Rational("1/2") / Rational("0"));
    
    // Очень большие числа (проверка, что не упадет)
    Rational big("1000000000000000/2000000000000000");
    big.reduce();
    EXPECT_EQ(big.toString(), "1/2");
}