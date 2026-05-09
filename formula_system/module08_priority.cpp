#include "module08_priority.h"
#include "rational_utils.h"

// Приоритеты: чем больше — тем сильнее связывает
static int priority(NodeType t) {
    switch (t) {
        case NodeType::ADD: case NodeType::SUB: return 1;
        case NodeType::MUL: case NodeType::DIV: return 2;
        case NodeType::POW:                     return 3;
        case NodeType::NEG:                     return 4;
        default:                                return 10; // функции
    }
}

static bool isRightAssoc(NodeType t) { return t == NodeType::POW; }

// needParens: нужно ли оборачивать child в скобки, когда он стоит в позиции
// (isRight=true — правый операнд) у родителя parent
static bool needParens(NodeType parent, NodeType child, bool isRight) {
    if (!nodeTypeIsBinaryOp(child) && !nodeTypeIsUnaryOp(child)) return false;
    int pp = priority(parent), cp = priority(child);
    if (cp < pp) return true;
    if (cp == pp) {
        // левоассоциативный: правый операнд требует скобок при равном приоритете
        if (isRight && !isRightAssoc(parent)) return true;
        // для SUB правый ADD/SUB тоже нужны скобки
        if (isRight && parent == NodeType::SUB &&
            (child == NodeType::ADD || child == NodeType::SUB)) return true;
    }
    return false;
}

std::string treeToStringPriority(const NodePtr& node) {
    if (!node) return "";

    switch (node->type) {
        case NodeType::NUMBER:   return rationalStr(node->value);
        case NodeType::VARIABLE: return node->varName;

        case NodeType::NEG: {
            NodePtr c = node->children[0];
            std::string s = treeToStringPriority(c);
            if (nodeTypeIsBinaryOp(c->type)) s = "(" + s + ")";
            return "-" + s;
        }

        case NodeType::ADD: case NodeType::SUB:
        case NodeType::MUL: case NodeType::DIV:
        case NodeType::POW: {
            NodePtr lc = node->children[0], rc = node->children[1];
            std::string l = treeToStringPriority(lc);
            std::string r = treeToStringPriority(rc);
            if (needParens(node->type, lc->type, false)) l = "(" + l + ")";
            if (needParens(node->type, rc->type, true))  r = "(" + r + ")";
            return l + nodeTypeToSymbol(node->type) + r;
        }

        case NodeType::SIN: case NodeType::COS: case NodeType::TAN:
        case NodeType::COT: case NodeType::SQRT: case NodeType::EXP:
            return nodeTypeToSymbol(node->type)
                   + "(" + treeToStringPriority(node->children[0]) + ")";

        case NodeType::LOG:
            return "log(" + treeToStringPriority(node->children[0])
                     + "," + treeToStringPriority(node->children[1]) + ")";

        default: return "?";
    }
}
