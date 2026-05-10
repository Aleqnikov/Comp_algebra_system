#include <gtest/gtest.h>
#include "module01_to_prefix.h"
#include "module02_from_postfix.h"
#include "module06_tree_to_str.h"

TEST(Module02, SingleVar) {
    auto t = postfixToTree({"x"});
    EXPECT_EQ(t->type, NodeType::VARIABLE);
    EXPECT_EQ(t->varName, "x");
}

TEST(Module02, SingleNum) {
    auto t = postfixToTree({"3"});
    EXPECT_EQ(t->type, NodeType::NUMBER);
}

TEST(Module02, Add) {
    auto t = postfixToTree({"x", "y", "+"});
    EXPECT_EQ(t->type, NodeType::ADD);
    EXPECT_EQ(t->children[0]->varName, "x");
    EXPECT_EQ(t->children[1]->varName, "y");
}

TEST(Module02, MulThenAdd) {
    // x y * z + → (x*y)+z
    auto t = postfixToTree({"x","y","*","z","+"});
    EXPECT_EQ(t->type, NodeType::ADD);
    EXPECT_EQ(t->children[0]->type, NodeType::MUL);
}

TEST(Module02, Sin) {
    auto t = postfixToTree({"x","sin"});
    EXPECT_EQ(t->type, NodeType::SIN);
    EXPECT_EQ(t->children[0]->varName, "x");
}

TEST(Module02, Log) {
    auto t = postfixToTree({"2","x","log"});
    EXPECT_EQ(t->type, NodeType::LOG);
}

TEST(Module02, RoundTrip) {
    // Парсим инфикс → постфикс → дерево → полная строка
    auto t1 = parseInfix("sin(x)+3*x^2");
    auto t2 = postfixToTree({"x","sin","3","x","2","^","*","+"});
    EXPECT_EQ(treeToFullString(t1), treeToFullString(t2));
}

TEST(Module02, SplitPostfix) {
    auto v = splitPostfix("x y +");
    ASSERT_EQ(v.size(), 3u);
    EXPECT_EQ(v[0], "x");
    EXPECT_EQ(v[1], "y");
    EXPECT_EQ(v[2], "+");
}