#include <gtest/gtest.h>
#include "module01_to_prefix.h"

TEST(Module01, SimpleAdd) {
    EXPECT_EQ(infixToPrefix("x+y"), "+ x y");
}

TEST(Module01, SimpleMul) {
    EXPECT_EQ(infixToPrefix("x*y"), "* x y");
}

TEST(Module01, AddAndMul) {
    EXPECT_EQ(infixToPrefix("x+3*y"), "+ x * 3 y");
}

TEST(Module01, Power) {
    EXPECT_EQ(infixToPrefix("x^2"), "^ x 2");
}

TEST(Module01, Sin) {
    EXPECT_EQ(infixToPrefix("sin(x)"), "sin x");
}

TEST(Module01, Log) {
    EXPECT_EQ(infixToPrefix("log(2,x)"), "log 2 x");
}

TEST(Module01, Nested) {
    EXPECT_EQ(infixToPrefix("sin(x)+3*x^2"), "+ sin x * 3 ^ x 2");
}

TEST(Module01, UnaryMinus) {
    // -x парсится как число или neg
    NodePtr t = parseInfix("-x");
    EXPECT_EQ(t->type, NodeType::NEG);
}

TEST(Module01, Parentheses) {
    // (x+y)*z
    EXPECT_EQ(infixToPrefix("(x+y)*z"), "* + x y z");
}

TEST(Module01, RightAssocPow) {
    // x^y^z → x^(y^z)
    EXPECT_EQ(infixToPrefix("x^y^z"), "^ x ^ y z");
}
