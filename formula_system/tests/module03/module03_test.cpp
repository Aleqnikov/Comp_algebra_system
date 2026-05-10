#include <gtest/gtest.h>
#include "module01_to_prefix.h"
#include "module03_diff.h"
#include "module08_priority.h"
#include "module09_simplify_tree.h"

static std::string diff(const std::string& expr, const std::string& var = "x") {
    return treeToStringPriority(
        simplifyTree(differentiate(parseInfix(expr), var)));
}

TEST(Module03, Constant) {
    // d(5)/dx = 0
    EXPECT_EQ(diff("5"), "0");
}

TEST(Module03, Variable) {
    // d(x)/dx = 1
    EXPECT_EQ(diff("x"), "1");
}

TEST(Module03, OtherVariable) {
    // d(y)/dx = 0
    EXPECT_EQ(diff("y"), "0");
}

TEST(Module03, Sum) {
    // d(x+y)/dx = 1
    EXPECT_EQ(diff("x+y"), "1");
}

TEST(Module03, Product) {
    // d(x*x)/dx = x+x = 2*x (после упрощения)
    auto t = simplifyTree(differentiate(parseInfix("x*x"), "x"));
    auto s = treeToStringPriority(t);
    // результат должен содержать x
    EXPECT_NE(s.find('x'), std::string::npos);
}

TEST(Module03, Power) {
    // d(x^3)/dx = 3*x^2
    EXPECT_EQ(diff("x^3"), "3*x^2");
}

TEST(Module03, Sin) {
    // d(sin(x))/dx = cos(x)
    EXPECT_EQ(diff("sin(x)"), "cos(x)");
}

TEST(Module03, Cos) {
    // d(cos(x))/dx = -sin(x)
    EXPECT_EQ(diff("cos(x)"), "-sin(x)");
}

TEST(Module03, Sqrt) {
    // d(sqrt(x))/dx = 1/(2*sqrt(x))
    auto t = simplifyTree(differentiate(parseInfix("sqrt(x)"), "x"));
    auto s = treeToStringPriority(t);
    EXPECT_NE(s.find("sqrt"), std::string::npos);
}

TEST(Module03, Exp) {
    // d(exp(x))/dx = exp(x)
    EXPECT_EQ(diff("exp(x)"), "exp(x)");
}

TEST(Module03, Log) {
    // d(log(e,x))/dx = 1/(x*log(e,e))
    auto t = simplifyTree(differentiate(parseInfix("log(e,x)"), "x"));
    auto s = treeToStringPriority(t);
    EXPECT_NE(s.find('x'), std::string::npos);
}

TEST(Module03, Chain) {
    // d(sin(x^2))/dx — содержит cos и x
    auto t = simplifyTree(differentiate(parseInfix("sin(x^2)"), "x"));
    auto s = treeToStringPriority(t);
    EXPECT_NE(s.find("cos"), std::string::npos);
    EXPECT_NE(s.find('x'), std::string::npos);
}