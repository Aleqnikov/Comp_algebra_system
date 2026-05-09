#include "module07_assoc.h"
#include "rational_utils.h"

// Возвращает true если op1 и op2 одного типа И оба ассоциативны
static bool sameAssoc(NodeType parent, NodeType child) {
    return (parent == NodeType::ADD && child == NodeType::ADD) ||
           (parent == NodeType::MUL && child == NodeType::MUL);
}

std::string treeToStringAssoc(const NodePtr& node) {
    if (!node) return "";

    switch (node->type) {
        case NodeType::NUMBER:   return rationalStr(node->value);
        case NodeType::VARIABLE: return node->varName;
        case NodeType::NEG:
            return "(-" + treeToStringAssoc(node->children[0]) + ")";

        case NodeType::ADD:
        case NodeType::MUL: {
            std::string sym = nodeTypeToSymbol(node->type);
            // Левый операнд: скобки нужны только если child — другой тип бинарного
            auto wrapIfNeeded = [&](const NodePtr& child) -> std::string {
                std::string s = treeToStringAssoc(child);
                if (child->isBinary() && !sameAssoc(node->type, child->type))
                    return "(" + s + ")";
                return s;
            };
            return wrapIfNeeded(node->children[0]) + sym + wrapIfNeeded(node->children[1]);
        }

        case NodeType::SUB: {
            std::string l = treeToStringAssoc(node->children[0]);
            NodePtr rc = node->children[1];
            std::string r = treeToStringAssoc(rc);
            // Правый операнд SUB со сложением/вычитанием — в скобки
            if (rc->type == NodeType::ADD || rc->type == NodeType::SUB)
                r = "(" + r + ")";
            return l + "-" + r;
        }

        case NodeType::DIV: {
            NodePtr lc = node->children[0], rc = node->children[1];
            std::string l = treeToStringAssoc(lc);
            std::string r = treeToStringAssoc(rc);
            if (lc->isBinary()) l = "(" + l + ")";
            if (rc->isBinary()) r = "(" + r + ")";
            return l + "/" + r;
        }

        case NodeType::POW: {
            NodePtr lc = node->children[0], rc = node->children[1];
            std::string l = treeToStringAssoc(lc);
            std::string r = treeToStringAssoc(rc);
            if (lc->isBinary() || lc->type == NodeType::NEG) l = "(" + l + ")";
            return l + "^" + r;
        }

        case NodeType::SIN: case NodeType::COS: case NodeType::TAN:
        case NodeType::COT: case NodeType::SQRT: case NodeType::EXP:
            return nodeTypeToSymbol(node->type)
                   + "(" + treeToStringAssoc(node->children[0]) + ")";

        case NodeType::LOG:
            return "log(" + treeToStringAssoc(node->children[0])
                     + "," + treeToStringAssoc(node->children[1]) + ")";
        default: return "?";
    }
}
