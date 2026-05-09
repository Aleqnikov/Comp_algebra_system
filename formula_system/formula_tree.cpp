#include "formula_tree.h"

NodePtr FormulaNode::makeNumber(const Rational& r) {
    auto n = std::shared_ptr<FormulaNode>(new FormulaNode());
    n->type  = NodeType::NUMBER;
    n->value = r;
    return n;
}

NodePtr FormulaNode::makeVariable(const std::string& name) {
    auto n = std::shared_ptr<FormulaNode>(new FormulaNode());
    n->type    = NodeType::VARIABLE;
    n->varName = name;
    return n;
}

NodePtr FormulaNode::makeUnary(NodeType t, NodePtr child) {
    auto n = std::shared_ptr<FormulaNode>(new FormulaNode());
    n->type = t;
    n->children.push_back(child);
    return n;
}

NodePtr FormulaNode::makeBinary(NodeType t, NodePtr left, NodePtr right) {
    auto n = std::shared_ptr<FormulaNode>(new FormulaNode());
    n->type = t;
    n->children.push_back(left);
    n->children.push_back(right);
    return n;
}

bool nodeTypeIsFunc(NodeType t) {
    return t == NodeType::SIN  || t == NodeType::COS  ||
           t == NodeType::TAN  || t == NodeType::COT  ||
           t == NodeType::SQRT || t == NodeType::EXP  ||
           t == NodeType::LOG;
}

bool nodeTypeIsUnaryOp(NodeType t) {
    return t == NodeType::NEG;
}

bool nodeTypeIsBinaryOp(NodeType t) {
    return t == NodeType::ADD || t == NodeType::SUB ||
           t == NodeType::MUL || t == NodeType::DIV ||
           t == NodeType::POW;
}

std::string nodeTypeToSymbol(NodeType t) {
    switch (t) {
        case NodeType::ADD:      return "+";
        case NodeType::SUB:      return "-";
        case NodeType::MUL:      return "*";
        case NodeType::DIV:      return "/";
        case NodeType::POW:      return "^";
        case NodeType::NEG:      return "neg";
        case NodeType::SIN:      return "sin";
        case NodeType::COS:      return "cos";
        case NodeType::TAN:      return "tan";
        case NodeType::COT:      return "cot";
        case NodeType::SQRT:     return "sqrt";
        case NodeType::EXP:      return "exp";
        case NodeType::LOG:      return "log";
        case NodeType::NUMBER:   return "#";
        case NodeType::VARIABLE: return "@";
        default:                 return "?";
    }
}

NodePtr cloneTree(const NodePtr& node) {
    if (!node) return nullptr;
    auto n = std::shared_ptr<FormulaNode>(new FormulaNode());
    n->type    = node->type;
    n->value   = node->value;
    n->varName = node->varName;
    for (auto& c : node->children)
        n->children.push_back(cloneTree(c));
    return n;
}
