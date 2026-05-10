#include <gtest/gtest.h>
#include "module11_like_terms_postfix.h"
#include "module12_like_terms_tree.h"
#include "module08_priority.h"
#include "module01_to_prefix.h"

// М11 — подобные по постфиксу
TEST(Module11, NoLikeTerms) {
    // x + y — не подобные
    auto v = collectLikeTermsPostfix({"x","y","+"});
    EXPECT_EQ(v.size(), 3u); // x y +
}

TEST(Module11, TwoLikeTerms) {
    // 3*x + 5*x = 8*x
    auto s = collectLikeTermsPostfixStr("3 x * 5 x * +");
    // должен содержать 8 и x
    EXPECT_NE(s.find('8'), std::string::npos);
    EXPECT_NE(s.find('x'), std::string::npos);
}

TEST(Module11, ThreeTermsMixed) {
    // 3*x + 5*x + 2*y → 8*x + 2*y
    auto s = collectLikeTermsPostfixStr("3 x * 5 x * + 2 y * +");
    EXPECT_NE(s.find('8'), std::string::npos);
    EXPECT_NE(s.find('y'), std::string::npos);
}

TEST(Module11, CancelTerms) {
    // x^2 - x^2 = 0
    auto s = collectLikeTermsPostfixStr("x 2 ^ x 2 ^ -");
    EXPECT_EQ(s, "0");
}

TEST(Module11, ZeroCoeff) {
    // 0*sin(x) + y = y
    auto s = collectLikeTermsPostfixStr("0 x sin * y +");
    // 0*sin(x) должен исчезнуть
    EXPECT_NE(s.find('y'), std::string::npos);
}

TEST(Module11, SingleTerm) {
    // одно слагаемое — без изменений
    auto s = collectLikeTermsPostfixStr("x sin");
    EXPECT_NE(s.find("sin"), std::string::npos);
}

// М12 — подобные по дереву
static std::string s12(const std::string& expr) {
    return treeToStringPriority(collectLikeTerms(parseInfix(expr)));
}

TEST(Module12, Simple) {
    // 3*x + 5*x = 8*x
    EXPECT_EQ(s12("3*x+5*x"), "8*x");
}

TEST(Module12, Mixed) {
    // 3*x + 5*x + 2*y = 8*x + 2*y
    EXPECT_EQ(s12("3*x+5*x+2*y"), "8*x+2*y");
}

TEST(Module12, Cancel) {
    // x^2 - x^2 = 0
    EXPECT_EQ(s12("x^2-x^2"), "0");
}

TEST(Module12, NoLike) {
    // sin(x) + x^2 — не подобные
    auto s = s12("sin(x)+x^2");
    EXPECT_NE(s.find("sin"), std::string::npos);
    EXPECT_NE(s.find("x^2"), std::string::npos);
}

TEST(Module12, NegCoeff) {
    // 5*x - 3*x = 2*x
    EXPECT_EQ(s12("5*x-3*x"), "2*x");
}

TEST(Module12, LogCancel) {
    // log(2,x) + x^2 - x^2 = log(2,x)
    EXPECT_EQ(s12("log(2,x)+x^2-x^2"), "log(2,x)");
}
