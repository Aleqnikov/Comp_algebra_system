#include <gtest/gtest.h>
#include "module13_commutative.h"
#include "module08_priority.h"
#include "module01_to_prefix.h"

static std::string s13(const std::string& expr) {
    return treeToStringPriority(sortFactors(parseInfix(expr)));
}

TEST(Module13, NumberFirst) {
    // x*3 → 3*x
    EXPECT_EQ(s13("x*3"), "3*x");
}

TEST(Module13, AlreadySorted) {
    // 3*x уже в порядке
    EXPECT_EQ(s13("3*x"), "3*x");
}

TEST(Module13, MergeNumbers) {
    // 2*3*x → 6*x
    EXPECT_EQ(s13("2*3*x"), "6*x");
}

TEST(Module13, VarsLexOrder) {
    // y*x → x*y
    EXPECT_EQ(s13("y*x"), "x*y");
}

TEST(Module13, NumberVarFunc) {
    // sin(x)*x*2 → 2*x*sin(x)
    EXPECT_EQ(s13("sin(x)*x*2"), "2*x*sin(x)");
}

TEST(Module13, ManyFactors) {
    // x*sin(x)*2*3 → 6*x*sin(x)
    EXPECT_EQ(s13("x*sin(x)*2*3"), "6*x*sin(x)");
}

TEST(Module13, TrigOrder) {
    // cos(x)*sin(x) — оба тригонометрические, порядок стабильный
    auto s = s13("cos(x)*sin(x)");
    EXPECT_NE(s.find("sin"), std::string::npos);
    EXPECT_NE(s.find("cos"), std::string::npos);
}

TEST(Module13, LogBeforeTrig) {
    // log(2,x)*sin(x) → log(2,x) перед sin(x)
    auto s = s13("sin(x)*log(2,x)");
    EXPECT_LT(s.find("log"), s.find("sin"));
}

TEST(Module13, PowBeforeTrig) {
    // x^2*sin(x) → x^2 перед sin(x)
    auto s = s13("sin(x)*x^2");
    EXPECT_LT(s.find('^'), s.find("sin"));
}

TEST(Module13, NoMulUnchanged) {
    // выражение без умножения не меняется
    EXPECT_EQ(s13("x+y"), "x+y");
}
