#include <gtest/gtest.h>
#include "algstructures/Natural.h"


// N1 — cmp
TEST(NaturalCmp, Basic) {
    Natural a("123");
    Natural b("124");
    EXPECT_EQ(a.cmp(&b), 1);
    EXPECT_EQ(b.cmp(&a), 2);
    EXPECT_EQ(a.cmp(&a), 0);
}

// N2 — != 0
TEST(NaturalNotEqualZero, Basic) {
    Natural zero("0");
    Natural one("1");
    EXPECT_FALSE(zero != 0);
    EXPECT_TRUE(one != 0);
}

// N3 — ++
TEST(NaturalIncrement, Basic) {
    Natural a("99");
    ++a;
    EXPECT_EQ(a.toString(), "100");
}

// N4 — +
TEST(NaturalAdd, Basic) {
    Natural a("97");
    Natural b("25");
    EXPECT_EQ((a + b).toString(), "122");
}

// N5 — -
TEST(NaturalSubtract, Basic) {
    Natural a("122");
    Natural b("25");
    EXPECT_EQ((a - b).toString(), "97");
}

// N6 — * digit
TEST(NaturalMultiplyByDigit, Basic) {
    Natural a("123");
    EXPECT_EQ((a * 3).toString(), "369");
}

// N7 — multiplyByPowerOfTen
TEST(NaturalMultiplyByPowerOfTen, Basic) {
    Natural a("123");
    EXPECT_EQ(a.multiplyByPowerOfTen(2).toString(), "12300");
}

// N8 — * Natural
TEST(NaturalMultiply, Basic) {
    Natural a("12");
    Natural b("13");
    EXPECT_EQ((a * b).toString(), "156");
}

// N9 — subtractMultiplied
TEST(NaturalSubtractMultiplied, Basic) {
    Natural a("100");
    Natural b("23");
    EXPECT_EQ(a.subtractMultiplied(b, 4).toString(), "8");
}

// N10 — getFirstDivisionDigit
TEST(NaturalGetFirstDivisionDigit, Basic) {
    Natural a("100");
    Natural b("23");
    EXPECT_EQ(a.getFirstDivisionDigit(b).toString(), "4");
}

// N11 — /
TEST(NaturalQuotient, Basic) {
	Natural a("100");
	Natural b("23");
	EXPECT_EQ((a / b).toString(), "4");
}

// N12 — %
TEST(NaturalRemainder, Basic) {
    Natural a("100");
    Natural b("23");
    EXPECT_EQ((a % b).toString(), "8");
}

// N13 — gcd
TEST(NaturalGCD, Basic) {
    Natural a("48");
    Natural b("18");
    EXPECT_EQ(Natural::gcd(a, b).toString(), "6");
}

// N14 — lcm
TEST(NaturalLCM, Basic) {
    Natural a("48");
    Natural b("18");
    EXPECT_EQ(Natural::lcm(a, b).toString(), "144");
}

TEST(NaturalExtra, ComparisonAndZero) {
    // Разная длина
    EXPECT_TRUE(Natural("100") > Natural("99"));
    EXPECT_TRUE(Natural("99") < Natural("100"));
    // Одинаковая длина, разница в середине
    EXPECT_TRUE(Natural("1024") > Natural("1023"));
    // Проверка оператора != 0
    EXPECT_TRUE(Natural("1") != 0);
    EXPECT_FALSE(Natural("0") != 0);
}

// --- ТЕСТЫ НА СЛОЖЕНИЕ И ИНКРЕМЕНТ (Переносы) ---

TEST(NaturalExtra, IncrementAndAdd) {
    // Множественный перенос: 999 + 1 = 1000
    Natural n999("999");
    ++n999;
    EXPECT_EQ(n999.toString(), "1000");

    // Сложение очень разных по длине чисел
    Natural a("1000000");
    Natural b("1");
    EXPECT_EQ((a + b).toString(), "1000001");
}

// --- ТЕСТЫ НА ВЫЧИТАНИЕ (Заемы) ---

TEST(NaturalExtra, Subtract) {
    // Заем через несколько разрядов: 1000 - 1 = 999
    EXPECT_EQ((Natural("1000") - Natural("1")).toString(), "999");
    // Вычитание в ноль
    EXPECT_EQ((Natural("123") - Natural("123")).toString(), "0");
    // Проверка исключения (нельзя вычитать большее из меньшего)
    EXPECT_ANY_THROW(Natural("50") - Natural("100"));
}

// --- ТЕСТЫ НА УМНОЖЕНИЕ ---

TEST(NaturalExtra, Multiplication) {
    // Умножение на 0
    EXPECT_EQ((Natural("12345") * 0).toString(), "0");
    EXPECT_EQ((Natural("12345") * Natural("0")).toString(), "0");
    // Умножение на 1
    EXPECT_EQ((Natural("12345") * 1).toString(), "12345");
    // Умножение больших чисел (111 * 111 = 12321)
    EXPECT_EQ((Natural("111") * Natural("111")).toString(), "12321");
    // Степень десятки
    EXPECT_EQ(Natural("10").multiplyByPowerOfTen(3).toString(), "10000");
}

// --- ТЕСТЫ НА ДЕЛЕНИЕ (Самое сложное) ---

TEST(NaturalExtra, Division) {
    // Деление на большее число (результат 0, остаток - само число)
    EXPECT_EQ((Natural("50") / Natural("100")).toString(), "0");
    EXPECT_EQ((Natural("50") % Natural("100")).toString(), "50");

    // Деление нацело
    EXPECT_EQ((Natural("100") / Natural("10")).toString(), "10");
    EXPECT_EQ((Natural("100") % Natural("10")).toString(), "0");

    // Деление с остатком
    Natural a("12345");
    Natural b("67"); // 12345 / 67 = 184, остаток 17
    EXPECT_EQ((a / b).toString(), "184");
    EXPECT_EQ((a % b).toString(), "17");
}

// --- ТЕСТЫ НА НОД И НОК ---

TEST(NaturalExtra, GCD_LCM) {
    // НОД(48, 18) = 6
    EXPECT_EQ(Natural::gcd(Natural("48"), Natural("18")).toString(), "6");
    // НОД(17, 13) = 1 (взаимно простые)
    EXPECT_EQ(Natural::gcd(Natural("17"), Natural("13")).toString(), "1");
    // НОК(12, 15) = 60
    EXPECT_EQ(Natural::lcm(Natural("12"), Natural("15")).toString(), "60");
}

// --- ТЕСТ НА ОСОБЫЕ СЛУЧАИ (getFirstDivisionDigit) ---

TEST(NaturalExtra, FirstDivisionDigit) {
    // 200 / 21 -> первая цифра 9 (так как 21*9 = 189 <= 200)
    EXPECT_EQ(Natural("200").getFirstDivisionDigit(Natural("21")).toString(), "9");
    // 100 / 11 -> первая цифра 9
    EXPECT_EQ(Natural("100").getFirstDivisionDigit(Natural("11")).toString(), "9");
}