#include "module06_tree_to_str.h"
#include "rational_utils.h"

std::string treeToFullString(const NodePtr& node) {
    if (!node) return "";

    switch (node->type) {
        case NodeType::NUMBER:
            return rationalStr(node->value);
        case NodeType::VARIABLE:
            return node->varName;
        case NodeType::NEG:
            return "(-" + treeToFullString(node->children[0]) + ")";
        case NodeType::ADD:
        case NodeType::SUB:
        case NodeType::MUL:
        case NodeType::DIV:
        case NodeType::POW:
            return "(" + treeToFullString(node->children[0])
                       + nodeTypeToSymbol(node->type)
                       + treeToFullString(node->children[1]) + ")";
        case NodeType::SIN:
        case NodeType::COS:
        case NodeType::TAN:
        case NodeType::COT:
        case NodeType::SQRT:
        case NodeType::EXP:
            return nodeTypeToSymbol(node->type)
                   + "(" + treeToFullString(node->children[0]) + ")";
        case NodeType::LOG:
            return "log(" + treeToFullString(node->children[0])
                     + "," + treeToFullString(node->children[1]) + ")";
        default:
            return "?";
    }
}
