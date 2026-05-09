#include "module04_tree_to_postfix.h"
#include "rational_utils.h"
#include <sstream>

static void traverse(const NodePtr& node, std::vector<std::string>& out) {
    if (!node) return;
    for (auto& c : node->children) traverse(c, out);
    switch (node->type) {
        case NodeType::NUMBER:   out.push_back(rationalStr(node->value)); break;
        case NodeType::VARIABLE: out.push_back(node->varName); break;
        default:                 out.push_back(nodeTypeToSymbol(node->type)); break;
    }
}

std::vector<std::string> treeToPostfix(const NodePtr& node) {
    std::vector<std::string> res;
    traverse(node, res);
    return res;
}

std::string treeToPostfixStr(const NodePtr& node) {
    auto v = treeToPostfix(node);
    std::string s;
    for (size_t i = 0; i < v.size(); ++i) {
        if (i) s += ' ';
        s += v[i];
    }
    return s;
}
