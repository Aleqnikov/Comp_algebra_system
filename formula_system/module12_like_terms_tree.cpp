#include "module12_like_terms_tree.h"
#include "module04_tree_to_postfix.h"
#include "rational_utils.h"
#include <map>
#include <vector>

// Плоский список слагаемых дерева ADD/SUB
static void flattenAdd(const NodePtr& node, std::vector<NodePtr>& terms) {
    if (!node) return;
    if (node->type == NodeType::ADD) {
        flattenAdd(node->children[0], terms);
        flattenAdd(node->children[1], terms);
    } else if (node->type == NodeType::SUB) {
        flattenAdd(node->children[0], terms);
        // b вычитается → добавляем как (-1)*b
        NodePtr neg_b = FormulaNode::makeBinary(NodeType::MUL,
            FormulaNode::makeNumber(Rational("-1")), node->children[1]);
        flattenAdd(neg_b, terms);
    } else {
        terms.push_back(node);
    }
}

// Разбить слагаемое на (коэффициент Rational, «тело» NodePtr).
// Поддерживаемые формы: c, c*u, u (считаем коэф=1), neg(u) (коэф=-1), neg(c*u)
static std::pair<Rational, NodePtr> splitCoeff(const NodePtr& term) {
    if (term->type == NodeType::NUMBER)
        return {term->value, FormulaNode::makeNumber(Rational("1"))};

    if (term->type == NodeType::MUL) {
        NodePtr l = term->children[0], r = term->children[1];
        if (l->type == NodeType::NUMBER) {
            // l — число, рекурсивно разбираем r
            auto [rc, rbody] = splitCoeff(r);
            Rational combined = l->value * rc;
            combined.reduce();
            return {combined, rbody};
        }
        if (r->type == NodeType::NUMBER) {
            auto [lc, lbody] = splitCoeff(l);
            Rational combined = lc * r->value;
            combined.reduce();
            return {combined, lbody};
        }
    }

    if (term->type == NodeType::NEG) {
        auto [c, body] = splitCoeff(term->children[0]);
        Integer neg_c = -c.getNumerator();
        Rational negR(neg_c, c.getDenominator());
        negR.reduce();
        return {negR, body};
    }

    return {Rational("1"), term};
}

static NodePtr buildFromTerms(const std::vector<NodePtr>& terms) {
    if (terms.empty()) return FormulaNode::makeNumber(Rational("0"));
    NodePtr acc = terms[0];
    for (size_t i = 1; i < terms.size(); ++i)
        acc = FormulaNode::makeBinary(NodeType::ADD, acc, terms[i]);
    return acc;
}

NodePtr collectLikeTerms(const NodePtr& node) {
    if (!node) return node;

    // Рекурсивно обрабатываем не-ADD/SUB поддеревья
    if (node->type != NodeType::ADD && node->type != NodeType::SUB) {
        NodePtr r = cloneTree(node);
        for (auto& c : r->children) c = collectLikeTerms(c);
        return r;
    }

    // Собираем плоский список слагаемых
    std::vector<NodePtr> terms;
    flattenAdd(node, terms);

    // Рекурсивно обрабатываем каждое слагаемое
    for (auto& t : terms) t = collectLikeTerms(t);

    // Группируем по постфиксному представлению тела
    // Используем map<string, Rational> для суммирования коэффициентов
    // Сохраняем порядок первого появления
    std::vector<std::string> order;
    std::map<std::string, Rational> coeffMap;
    std::map<std::string, NodePtr>  bodyMap;

    for (auto& t : terms) {
        auto [coeff, body] = splitCoeff(t);
        std::string key = treeToPostfixStr(body);
        if (coeffMap.find(key) == coeffMap.end()) {
            order.push_back(key);
            coeffMap.emplace(key, Rational("0"));
            bodyMap[key] = body;
        }
        coeffMap.at(key) = coeffMap.at(key) + coeff;
    }

    // Собираем результат
    std::vector<NodePtr> result;
    for (auto& key : order) {
        Rational c = coeffMap.at(key);
        c.reduce();
        if (rationalIsZero(c)) continue;

        NodePtr body = bodyMap.at(key);
        NodePtr term;

        // body == "1" означает, что слагаемое — просто число
        if (body->type == NodeType::NUMBER && rationalIsOne(body->value)) {
            term = FormulaNode::makeNumber(c);
        } else if (rationalIsOne(c)) {
            term = body;
        } else if (c.getNumerator().isNegative()) {
            // отрицательный коэф — neg(|c|*body)
            Integer pos = -c.getNumerator();
            Rational posR(pos, c.getDenominator());
            posR.reduce();
            if (rationalIsOne(posR))
                term = FormulaNode::makeUnary(NodeType::NEG, body);
            else
                term = FormulaNode::makeUnary(NodeType::NEG,
                    FormulaNode::makeBinary(NodeType::MUL,
                        FormulaNode::makeNumber(posR), body));
        } else {
            term = FormulaNode::makeBinary(NodeType::MUL,
                FormulaNode::makeNumber(c), body);
        }
        result.push_back(term);
    }

    if (result.empty()) return FormulaNode::makeNumber(Rational("0"));
    return buildFromTerms(result);
}