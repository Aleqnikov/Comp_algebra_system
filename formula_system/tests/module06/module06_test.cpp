#include <gtest/gtest.h>
#include "module06_tree_to_str.h"
#include "module07_assoc.h"
#include "module08_priority.h"
#include "module01_to_prefix.h"

// М6
TEST(Module06, Number) {
    EXPECT_EQ(treeToFullString(parseInfix("3")), "3");
}

TEST(Module06, Add) {
    EXPECT_EQ(treeToFullString(parseInfix("x+y")), "(x+y)");
}

TEST(Module06, MulAdd) {
    EXPECT_EQ(treeToFullString(parseInfix("x+3*y")), "(x+(3*y))");
}

TEST(Module06, Sin) {
    EXPECT_EQ(treeToFullString(parseInfix("sin(x)")), "sin(x)");
}

TEST(Module06, Log) {
    EXPECT_EQ(treeToFullString(parseInfix("log(2,x)")), "log(2,x)");
}

TEST(Module06, Complex) {
    EXPECT_EQ(treeToFullString(parseInfix("sin(x)+3*x^2")),
              "(sin(x)+(3*(x^2)))");
}

// М7 — ассоциативность умножения
TEST(Module07, MulChain) {
    // a*b*c не требует скобок
    EXPECT_EQ(treeToStringAssoc(parseInfix("a*b*c")), "a*b*c");
}

TEST(Module07, AddChain) {
    // a+b+c не требует скобок
    EXPECT_EQ(treeToStringAssoc(parseInfix("a+b+c")), "a+b+c");
}

TEST(Module07, MulWithAdd) {
    // (a+b)*c — нужны скобки вокруг сложения
    EXPECT_EQ(treeToStringAssoc(parseInfix("(a+b)*c")), "(a+b)*c");
}

TEST(Module07, Sin) {
    EXPECT_EQ(treeToStringAssoc(parseInfix("sin(x)")), "sin(x)");
}

// М8 — приоритет операций
TEST(Module08, NoExtraParens) {
    // 3*x+5 не нужны скобки вокруг 3*x
    EXPECT_EQ(treeToStringPriority(parseInfix("3*x+5")), "3*x+5");
}

TEST(Module08, NeedParensAdd) {
    // (x+y)*z — скобки нужны
    EXPECT_EQ(treeToStringPriority(parseInfix("(x+y)*z")), "(x+y)*z");
}

TEST(Module08, PowerNoParens) {
    // x^2 не нужны скобки
    EXPECT_EQ(treeToStringPriority(parseInfix("x^2")), "x^2");
}

TEST(Module08, SubRight) {
    // a-(b+c) — нужны скобки
    EXPECT_EQ(treeToStringPriority(parseInfix("a-(b+c)")), "a-(b+c)");
}

TEST(Module08, MulDiv) {
    // a*b/c — без скобок
    EXPECT_EQ(treeToStringPriority(parseInfix("a*b/c")), "a*b/c");
}

TEST(Module08, Complex) {
    EXPECT_EQ(treeToStringPriority(parseInfix("sin(x)+3*x^2")),
              "sin(x)+3*x^2");
}
