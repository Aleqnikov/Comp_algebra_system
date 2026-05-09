#include "module13_commutative.h"
#include <algorithm>
#include <vector>

// Категория множителя для сортировки
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

// Лексикографическое имя для сравнения внутри категории
static std::string factorKey(const NodePtr& n) {
    if (n->type == NodeType::VARIABLE) return n->varName;
    if (n->type == NodeType::SQRT && !n->children.empty() &&
        n->children[0]->type == NodeType::VARIABLE)
        return n->children[0]->varName; // рядом со своей переменной
    return "";
}

static bool factorLess(const NodePtr& a, const NodePtr& b) {
    int ca = factorCategory(a), cb = factorCategory(b);
    if (ca != cb) return ca < cb;
    // Внутри категории: переменные и sqrt лексикографически
    return factorKey(a) < factorKey(b);
}

// Собрать плоский список множителей из цепочки MUL
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

    // Рекурсивно
    NodePtr r = cloneTree(node);
    for (auto& c : r->children) c = sortFactors(c);

    if (r->type != NodeType::MUL) return r;

    // Собираем плоский список
    std::vector<NodePtr> factors;
    flattenMul(r, factors);

    // Сортируем устойчиво
    std::stable_sort(factors.begin(), factors.end(), factorLess);

    return buildMul(factors);
}
