#include <gtest/gtest.h>
#include "module09_simplify_tree.h"
#include "module10_simplify_postfix.h"
#include "module08_priority.h"
#include "module01_to_prefix.h"
#include "module02_from_postfix.h"

static std::string s9(const std::string& expr) {
    return treeToStringPriority(simplifyTree(parseInfix(expr)));
}

// М9 — по дереву
TEST(Module09, AddZeroRight) {
    EXPECT_EQ(s9("x+0"), "x");
}

TEST(Module09, AddZeroLeft) {
    EXPECT_EQ(s9("0+x"), "x");
}

TEST(Module09, SubZero) {
    EXPECT_EQ(s9("x-0"), "x");
}

TEST(Module09, MulOne) {
    EXPECT_EQ(s9("x*1"), "x");
}

TEST(Module09, OneMul) {
    EXPECT_EQ(s9("1*x"), "x");
}

TEST(Module09, MulZero) {
    EXPECT_EQ(s9("x*0"), "0");
}

TEST(Module09, ZeroMul) {
    EXPECT_EQ(s9("0*x"), "0");
}

TEST(Module09, DivOne) {
    EXPECT_EQ(s9("x/1"), "x");
}

TEST(Module09, PowZero) {
    EXPECT_EQ(s9("x^0"), "1");
}

TEST(Module09, PowOne) {
    EXPECT_EQ(s9("x^1"), "x");
}

TEST(Module09, ZeroPow) {
    EXPECT_EQ(s9("0^x"), "0");
}

TEST(Module09, ConstFold) {
    EXPECT_EQ(s9("3+5"), "8");
}

TEST(Module09, ConstFoldMul) {
    EXPECT_EQ(s9("3*5"), "15");
}

TEST(Module09, SubSelf) {
    EXPECT_EQ(s9("x-x"), "0");
}

TEST(Module09, Complex) {
    // (x+0)*(1*y) = x*y
    EXPECT_EQ(s9("(x+0)*(1*y)"), "x*y");
}

TEST(Module09, ComplexFull) {
    // (0+1)*(1*y) = y
    EXPECT_EQ(s9("(0+1)*(1*y)"), "y");
}

// М10 — по постфиксу (без дерева)
TEST(Module10, AddZero) {
    EXPECT_EQ(simplifyPostfixStr("x 0 +"), "x");
}

TEST(Module10, MulOne) {
    EXPECT_EQ(simplifyPostfixStr("x 1 *"), "x");
}

TEST(Module10, MulZero) {
    EXPECT_EQ(simplifyPostfixStr("x 0 *"), "0");
}

TEST(Module10, PowZero) {
    EXPECT_EQ(simplifyPostfixStr("x 0 ^"), "1");
}

TEST(Module10, PowOne) {
    EXPECT_EQ(simplifyPostfixStr("x 1 ^"), "x");
}

TEST(Module10, DivOne) {
    EXPECT_EQ(simplifyPostfixStr("x 1 /"), "x");
}

TEST(Module10, ConstFold) {
    EXPECT_EQ(simplifyPostfixStr("3 5 +"), "8");
}

TEST(Module10, SubSelf) {
    EXPECT_EQ(simplifyPostfixStr("x 2 ^ x 2 ^ -"), "0");
}

TEST(Module10, ZeroMul) {
    EXPECT_EQ(simplifyPostfixStr("0 x sin *"), "0");
}