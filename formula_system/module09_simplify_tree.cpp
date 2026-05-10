#include "module09_simplify_tree.h"
#include "rational_utils.h"
#include "module04_tree_to_postfix.h"

static bool isZero(const NodePtr& n) {
    return n->type == NodeType::NUMBER && rationalIsZero(n->value);
}
static bool isOne(const NodePtr& n) {
    return n->type == NodeType::NUMBER && rationalIsOne(n->value);
}
static NodePtr N(const std::string& s){ return FormulaNode::makeNumber(Rational(s)); }

static bool sameTree(const NodePtr& a, const NodePtr& b) {
    return treeToPostfixStr(a) == treeToPostfixStr(b);
}

// Один проход упрощения
static NodePtr simplifyOnce(const NodePtr& node) {
    if (!node) return node;

    NodePtr result = cloneTree(node);
    for (auto& c : result->children)
        c = simplifyOnce(c);

    // Свёртка двух констант
    if (result->isBinary() &&
        result->children[0]->type == NodeType::NUMBER &&
        result->children[1]->type == NodeType::NUMBER) {
        Rational a = result->children[0]->value;
        Rational b = result->children[1]->value;
        Rational r("0");
        bool folded = true;
        switch (result->type) {
            case NodeType::ADD: r = a + b; break;
            case NodeType::SUB: r = a - b; break;
            case NodeType::MUL: r = a * b; break;
            case NodeType::DIV:
                if (!rationalIsZero(b)) r = a / b; else folded = false;
                break;
            default: folded = false;
        }
        if (folded) { r.reduce(); return FormulaNode::makeNumber(r); }
    }

    // x - x = 0
    if (result->type == NodeType::SUB &&
        sameTree(result->children[0], result->children[1]))
        return N("0");

    // Протаскивание констант: (expr * C1) * C2 → expr * (C1*C2)
    // и C1 * (C2 * expr) → (C1*C2) * expr
    if (result->type == NodeType::MUL) {
        NodePtr l = result->children[0], r = result->children[1];
        // (A * C1) * C2 → A * (C1*C2)
        if (r->type == NodeType::NUMBER && l->type == NodeType::MUL &&
            l->children[1]->type == NodeType::NUMBER) {
            Rational merged = l->children[1]->value * r->value;
            merged.reduce();
            return simplifyOnce(FormulaNode::makeBinary(NodeType::MUL,
                l->children[0], FormulaNode::makeNumber(merged)));
        }
        // C1 * (C2 * A) → (C1*C2) * A
        if (l->type == NodeType::NUMBER && r->type == NodeType::MUL &&
            r->children[0]->type == NodeType::NUMBER) {
            Rational merged = l->value * r->children[0]->value;
            merged.reduce();
            return simplifyOnce(FormulaNode::makeBinary(NodeType::MUL,
                FormulaNode::makeNumber(merged), r->children[1]));
        }
    }

    if (result->type == NodeType::ADD) {
        if (isZero(result->children[0])) return result->children[1];
        if (isZero(result->children[1])) return result->children[0];
    }
    if (result->type == NodeType::SUB) {
        if (isZero(result->children[1])) return result->children[0];
    }
    if (result->type == NodeType::MUL) {
        if (isZero(result->children[0]) || isZero(result->children[1])) return N("0");
        if (isOne(result->children[0])) return result->children[1];
        if (isOne(result->children[1])) return result->children[0];
    }
    if (result->type == NodeType::DIV) {
        if (isOne(result->children[1]))  return result->children[0];
        if (isZero(result->children[0])) return N("0");
    }
    if (result->type == NodeType::POW) {
        if (isZero(result->children[1])) return N("1");
        if (isOne(result->children[1]))  return result->children[0];
        if (isZero(result->children[0])) return N("0");
    }
    if (result->type == NodeType::NEG) {
        if (isZero(result->children[0])) return N("0");
    }

    return result;
}

// Повторяем до стабилизации
NodePtr simplifyTree(const NodePtr& node) {
    NodePtr cur = node;
    for (int i = 0; i < 20; ++i) {
        NodePtr next = simplifyOnce(cur);
        if (treeToPostfixStr(next) == treeToPostfixStr(cur)) break;
        cur = next;
    }
    return cur;
}
// ПРИМЕЧАНИЕ: функция уже определена выше через simplifyOnce + итерацию