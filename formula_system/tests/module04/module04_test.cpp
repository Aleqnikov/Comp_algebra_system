#include <gtest/gtest.h>
#include "module04_tree_to_postfix.h"
#include "module05_postfix_to_str.h"
#include "module01_to_prefix.h"

// М4
TEST(Module04, Var) {
    auto t = parseInfix("x");
    EXPECT_EQ(treeToPostfixStr(t), "x");
}

TEST(Module04, Add) {
    auto t = parseInfix("x+y");
    EXPECT_EQ(treeToPostfixStr(t), "x y +");
}

TEST(Module04, MulAdd) {
    auto t = parseInfix("3*x+5*x");
    EXPECT_EQ(treeToPostfixStr(t), "3 x * 5 x * +");
}

TEST(Module04, Sin) {
    auto t = parseInfix("sin(x)");
    EXPECT_EQ(treeToPostfixStr(t), "x sin");
}

TEST(Module04, Log) {
    auto t = parseInfix("log(2,x)");
    EXPECT_EQ(treeToPostfixStr(t), "2 x log");
}

TEST(Module04, Complex) {
    auto t = parseInfix("sin(x)+3*x^2");
    EXPECT_EQ(treeToPostfixStr(t), "x sin 3 x 2 ^ * +");
}

// М5
TEST(Module05, Add) {
    EXPECT_EQ(postfixStrToFullString("x y +"), "(x+y)");
}

TEST(Module05, MulAdd) {
    EXPECT_EQ(postfixStrToFullString("3 x * 5 x * +"), "((3*x)+(5*x))");
}

TEST(Module05, Sin) {
    EXPECT_EQ(postfixStrToFullString("x sin"), "sin(x)");
}

TEST(Module05, Log) {
    EXPECT_EQ(postfixStrToFullString("2 x log"), "log(2,x)");
}

TEST(Module05, RoundTrip) {
    // инфикс → постфикс → полная строка → сравниваем с М6
    auto t = parseInfix("sin(x)+3*x^2");
    auto pf = treeToPostfix(t);
    EXPECT_EQ(postfixToFullString(pf), "(sin(x)+(3*(x^2)))");
}
