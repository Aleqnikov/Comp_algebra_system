#include "module13_commutative.h"
#include "rational_utils.h"
#include <algorithm>
#include <vector>

static int factorCategory(const NodePtr& n) {
    switch (n->type) {
        case NodeType::NUMBER:   return 0;
        case NodeType::VARIABLE: return 1;
        case NodeType::SQRT:     return 2;
        case NodeType::POW:      return 3;
        case NodeType::EXP:      return 4;
        case NodeType::LOG:      return 5;
        case NodeType::SIN: case NodeType::COS:
        case NodeType::TAN: case NodeType::COT: return 6;
        default:                 return 7;
    }
}

static std::string factorKey(const NodePtr& n) {
    if (n->type == NodeType::VARIABLE) return n->varName;
    if (n->type == NodeType::SQRT && !n->children.empty() &&
        n->children[0]->type == NodeType::VARIABLE)
        return n->children[0]->varName;
    return "";
}

static bool factorLess(const NodePtr& a, const NodePtr& b) {
    int ca = factorCategory(a), cb = factorCategory(b);
    if (ca != cb) return ca < cb;
    return factorKey(a) < factorKey(b);
}

static void flattenMul(const NodePtr& node, std::vector<NodePtr>& factors) {
    if (node->type == NodeType::MUL) {
        flattenMul(node->children[0], factors);
        flattenMul(node->children[1], factors);
    } else {
        factors.push_back(node);
    }
}

static NodePtr buildMul(const std::vector<NodePtr>& v) {
    if (v.empty()) return FormulaNode::makeNumber(Rational("1"));
    NodePtr acc = v[0];
    for (size_t i = 1; i < v.size(); ++i)
        acc = FormulaNode::makeBinary(NodeType::MUL, acc, v[i]);
    return acc;
}

NodePtr sortFactors(const NodePtr& node) {
    if (!node) return node;

    NodePtr r = cloneTree(node);
    for (auto& c : r->children) c = sortFactors(c);

    if (r->type != NodeType::MUL) return r;

    std::vector<NodePtr> factors;
    flattenMul(r, factors);

    // Свернуть все числовые константы в одну
    Rational combined("1");
    std::vector<NodePtr> nonNumbers;
    for (auto& f : factors) {
        if (f->type == NodeType::NUMBER) {
            combined = combined * f->value;
        } else {
            nonNumbers.push_back(f);
        }
    }
    combined.reduce();

    // Сортируем нечисловые множители
    std::stable_sort(nonNumbers.begin(), nonNumbers.end(), factorLess);

    // Собираем: число впереди (если не 1)
    std::vector<NodePtr> result;
    if (!rationalIsOne(combined))
        result.push_back(FormulaNode::makeNumber(combined));
    result.insert(result.end(), nonNumbers.begin(), nonNumbers.end());

    return buildMul(result);
}