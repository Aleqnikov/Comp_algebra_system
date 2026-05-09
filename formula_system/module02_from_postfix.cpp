#include "module02_from_postfix.h"
#include "tokenizer.h"
#include <stack>
#include <stdexcept>
#include <sstream>

std::vector<std::string> splitPostfix(const std::string& s) {
    std::vector<std::string> res;
    std::istringstream ss(s);
    std::string tok;
    while (ss >> tok) res.push_back(tok);
    return res;
}

static bool isNumberStr(const std::string& s) {
    if (s.empty()) return false;
    size_t i = 0;
    if (s[0] == '-') { ++i; if (i == s.size()) return false; }
    while (i < s.size() && std::isdigit((unsigned char)s[i])) ++i;
    if (i < s.size() && s[i] == '/') {
        ++i;
        if (i == s.size()) return false;
        while (i < s.size() && std::isdigit((unsigned char)s[i])) ++i;
    }
    return i == s.size();
}

static bool isVarStr(const std::string& s) {
    if (s.empty() || !std::isalpha((unsigned char)s[0])) return false;
    if (isFuncName(s)) return false;
    for (char c : s) if (!std::isalpha((unsigned char)c)) return false;
    return true;
}

NodePtr postfixToTree(const std::vector<std::string>& postfix) {
    std::stack<NodePtr> st;

    auto pop = [&]() -> NodePtr {
        if (st.empty()) throw std::runtime_error("postfixToTree: stack underflow");
        NodePtr t = st.top(); st.pop(); return t;
    };

    for (const std::string& tok : postfix) {
        // Число
        if (isNumberStr(tok)) {
            st.push(FormulaNode::makeNumber(Rational(tok)));
            continue;
        }
        // Переменная
        if (isVarStr(tok)) {
            st.push(FormulaNode::makeVariable(tok));
            continue;
        }
        // Бинарные операторы
        if (tok == "+" || tok == "-" || tok == "*" || tok == "/" || tok == "^") {
            NodePtr r = pop(), l = pop();
            NodeType nt = tok == "+" ? NodeType::ADD :
                          tok == "-" ? NodeType::SUB :
                          tok == "*" ? NodeType::MUL :
                          tok == "/" ? NodeType::DIV : NodeType::POW;
            st.push(FormulaNode::makeBinary(nt, l, r));
            continue;
        }
        // Унарный минус
        if (tok == "neg") {
            NodePtr c = pop();
            st.push(FormulaNode::makeUnary(NodeType::NEG, c));
            continue;
        }
        // Функции
        NodeType ft;
        bool isBin = (tok == "log");
        if      (tok == "sin")  ft = NodeType::SIN;
        else if (tok == "cos")  ft = NodeType::COS;
        else if (tok == "tan")  ft = NodeType::TAN;
        else if (tok == "cot")  ft = NodeType::COT;
        else if (tok == "sqrt") ft = NodeType::SQRT;
        else if (tok == "exp")  ft = NodeType::EXP;
        else if (tok == "log")  ft = NodeType::LOG;
        else throw std::runtime_error("postfixToTree: unknown token '" + tok + "'");

        if (isBin) {
            NodePtr arg = pop(), base = pop();
            st.push(FormulaNode::makeBinary(ft, base, arg));
        } else {
            NodePtr arg = pop();
            st.push(FormulaNode::makeUnary(ft, arg));
        }
    }

    if (st.size() != 1)
        throw std::runtime_error("postfixToTree: invalid postfix expression");
    return st.top();
}
