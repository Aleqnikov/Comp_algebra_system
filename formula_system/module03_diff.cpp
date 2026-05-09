#include "module03_diff.h"
#include <stdexcept>

// Вспомогательные строители узлов
static NodePtr N(const std::string& s) { return FormulaNode::makeNumber(Rational(s)); }
static NodePtr add(NodePtr a, NodePtr b){ return FormulaNode::makeBinary(NodeType::ADD,a,b); }
static NodePtr sub(NodePtr a, NodePtr b){ return FormulaNode::makeBinary(NodeType::SUB,a,b); }
static NodePtr mul(NodePtr a, NodePtr b){ return FormulaNode::makeBinary(NodeType::MUL,a,b); }
static NodePtr div_(NodePtr a,NodePtr b){ return FormulaNode::makeBinary(NodeType::DIV,a,b); }
static NodePtr pow_(NodePtr a,NodePtr b){ return FormulaNode::makeBinary(NodeType::POW,a,b); }
static NodePtr neg(NodePtr a)           { return FormulaNode::makeUnary(NodeType::NEG,a); }
static NodePtr sin_(NodePtr a)          { return FormulaNode::makeUnary(NodeType::SIN,a); }
static NodePtr cos_(NodePtr a)          { return FormulaNode::makeUnary(NodeType::COS,a); }
static NodePtr sqrt_(NodePtr a)         { return FormulaNode::makeUnary(NodeType::SQRT,a); }
static NodePtr log_(NodePtr base,NodePtr arg){ return FormulaNode::makeBinary(NodeType::LOG,base,arg); }
static NodePtr exp_(NodePtr a)          { return FormulaNode::makeUnary(NodeType::EXP,a); }

NodePtr differentiate(const NodePtr& node, const std::string& var) {
    if (!node) throw std::runtime_error("differentiate: null node");

    switch (node->type) {

    // d(c)/dx = 0
    case NodeType::NUMBER:
        return N("0");

    // d(x)/dx = 1, d(y)/dx = 0
    case NodeType::VARIABLE:
        return (node->varName == var) ? N("1") : N("0");

    // d(-u)/dx = -u'
    case NodeType::NEG: {
        NodePtr up = differentiate(node->children[0], var);
        return neg(up);
    }

    // d(u+v)/dx = u' + v'
    case NodeType::ADD: {
        return add(differentiate(node->children[0], var),
                   differentiate(node->children[1], var));
    }

    // d(u-v)/dx = u' - v'
    case NodeType::SUB: {
        return sub(differentiate(node->children[0], var),
                   differentiate(node->children[1], var));
    }

    // d(u*v)/dx = u'v + uv'
    case NodeType::MUL: {
        NodePtr u = node->children[0], v = node->children[1];
        NodePtr up = differentiate(u, var), vp = differentiate(v, var);
        return add(mul(up, cloneTree(v)), mul(cloneTree(u), vp));
    }

    // d(u/v)/dx = (u'v - uv') / v^2
    case NodeType::DIV: {
        NodePtr u = node->children[0], v = node->children[1];
        NodePtr up = differentiate(u, var), vp = differentiate(v, var);
        return div_(sub(mul(up, cloneTree(v)), mul(cloneTree(u), vp)),
                    pow_(cloneTree(v), N("2")));
    }

    // d(u^n)/dx = n * u^(n-1) * u'   (n — константа Rational)
    // d(u^v)/dx = (u^v)*(v'*ln(u) + v*u'/u)  (общий случай)
    case NodeType::POW: {
        NodePtr u = node->children[0], v = node->children[1];
        bool vIsConst = (v->type == NodeType::NUMBER);
        NodePtr up = differentiate(u, var);
        NodePtr vp = differentiate(v, var);

        if (vIsConst) {
            // n * u^(n-1) * u'
            Rational n_minus_1 = v->value - Rational("1");
            NodePtr newExp = FormulaNode::makeNumber(n_minus_1);
            return mul(mul(cloneTree(v),
                           pow_(cloneTree(u), newExp)),
                       up);
        } else {
            // (u^v) * (v'*ln(u) + v * u'/u)
            // ln(u) = log(e, u) — представим как log("e", u)
            // Но у нас нет NodeType::E, используем log с базой e как LOG(e_var, u)
            // Для общности: ln(u) = log(e, u) где base — переменная "e"
            NodePtr ln_u = log_(FormulaNode::makeVariable("e"), cloneTree(u));
            NodePtr term1 = mul(vp, ln_u);
            NodePtr term2 = mul(cloneTree(v), div_(up, cloneTree(u)));
            return mul(cloneTree(node), add(term1, term2));
        }
    }

    // d(sin u)/dx = cos(u) * u'
    case NodeType::SIN: {
        NodePtr u = node->children[0];
        return mul(cos_(cloneTree(u)), differentiate(u, var));
    }

    // d(cos u)/dx = -sin(u) * u'
    case NodeType::COS: {
        NodePtr u = node->children[0];
        return mul(neg(sin_(cloneTree(u))), differentiate(u, var));
    }

    // d(tan u)/dx = u' / cos^2(u)
    case NodeType::TAN: {
        NodePtr u = node->children[0];
        return div_(differentiate(u, var),
                    pow_(cos_(cloneTree(u)), N("2")));
    }

    // d(cot u)/dx = -u' / sin^2(u)
    case NodeType::COT: {
        NodePtr u = node->children[0];
        return div_(neg(differentiate(u, var)),
                    pow_(sin_(cloneTree(u)), N("2")));
    }

    // d(sqrt u)/dx = u' / (2 * sqrt(u))
    case NodeType::SQRT: {
        NodePtr u = node->children[0];
        return div_(differentiate(u, var),
                    mul(N("2"), sqrt_(cloneTree(u))));
    }

    // d(exp(u))/dx = exp(u) * u'   (основание e)
    case NodeType::EXP: {
        NodePtr u = node->children[0];
        return mul(cloneTree(node), differentiate(u, var));
    }

    // d(log(a, u))/dx = u' / (u * ln(a))
    // ln(a) = log(e, a)
    case NodeType::LOG: {
        NodePtr a = node->children[0], u = node->children[1];
        NodePtr up = differentiate(u, var);
        NodePtr ln_a = log_(FormulaNode::makeVariable("e"), cloneTree(a));
        return div_(up, mul(cloneTree(u), ln_a));
    }

    default:
        throw std::runtime_error("differentiate: unknown NodeType");
    }
}
