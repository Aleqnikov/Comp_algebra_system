#include "module01_to_prefix.h"
#include "tokenizer.h"
#include "rational_utils.h"
#include <stdexcept>
#include <sstream>

// ─── Рекурсивный спуск ───────────────────────────────────────────────────────
// Грамматика (с приоритетами):
//   expr   = term   { ('+' | '-') term }
//   term   = factor { ('*' | '/') factor }
//   factor = base   [ '^' factor ]          (правоассоциативно)
//   base   = NUMBER | VARIABLE | FUNC '(' args ')' | '(' expr ')' | '-' base

struct Parser {
    const std::vector<Token>& toks;
    size_t pos;

    Token cur() const { return toks[pos]; }
    Token consume() { return toks[pos++]; }
    bool at(TokenType t) const { return toks[pos].type == t; }

    Token expect(TokenType t) {
        if (!at(t)) throw std::runtime_error("Parser: unexpected token '" + toks[pos].text + "'");
        return consume();
    }

    // expr = term { ('+' | '-') term }
    NodePtr parseExpr() {
        NodePtr left = parseTerm();
        while (at(TokenType::PLUS) || at(TokenType::MINUS)) {
            NodeType op = at(TokenType::PLUS) ? NodeType::ADD : NodeType::SUB;
            consume();
            NodePtr right = parseTerm();
            left = FormulaNode::makeBinary(op, left, right);
        }
        return left;
    }

    // term = factor { ('*' | '/') factor }
    NodePtr parseTerm() {
        NodePtr left = parseFactor();
        while (at(TokenType::STAR) || at(TokenType::SLASH)) {
            NodeType op = at(TokenType::STAR) ? NodeType::MUL : NodeType::DIV;
            consume();
            NodePtr right = parseFactor();
            left = FormulaNode::makeBinary(op, left, right);
        }
        return left;
    }

    // factor = base [ '^' factor ]
    NodePtr parseFactor() {
        NodePtr b = parseBase();
        if (at(TokenType::CARET)) {
            consume();
            NodePtr exp = parseFactor(); // правоассоциативно
            return FormulaNode::makeBinary(NodeType::POW, b, exp);
        }
        return b;
    }

    // base = NUMBER | VARIABLE | FUNC '(' args ')' | '(' expr ')' | '-' base
    NodePtr parseBase() {
        // Унарный минус
        if (at(TokenType::MINUS)) {
            consume();
            NodePtr child = parseBase();
            // Если число — просто меняем знак
            if (child->type == NodeType::NUMBER) {
                std::string s = "-" + child->value.toString();
                return FormulaNode::makeNumber(Rational(s));
            }
            return FormulaNode::makeUnary(NodeType::NEG, child);
        }

        // Скобки
        if (at(TokenType::LPAREN)) {
            consume();
            NodePtr e = parseExpr();
            expect(TokenType::RPAREN);
            return e;
        }

        // Функция
        if (at(TokenType::FUNC)) {
            std::string fname = consume().text;
            expect(TokenType::LPAREN);

            if (fname == "log") {
                // log(base, arg)
                NodePtr base = parseExpr();
                expect(TokenType::COMMA);
                NodePtr arg = parseExpr();
                expect(TokenType::RPAREN);
                return FormulaNode::makeBinary(NodeType::LOG, base, arg);
            } else {
                // унарная функция
                NodePtr arg = parseExpr();
                expect(TokenType::RPAREN);
                NodeType nt;
                if      (fname == "sin")  nt = NodeType::SIN;
                else if (fname == "cos")  nt = NodeType::COS;
                else if (fname == "tan")  nt = NodeType::TAN;
                else if (fname == "cot")  nt = NodeType::COT;
                else if (fname == "sqrt") nt = NodeType::SQRT;
                else if (fname == "exp")  nt = NodeType::EXP;
                else throw std::runtime_error("Unknown function: " + fname);
                return FormulaNode::makeUnary(nt, arg);
            }
        }

        // Число
        if (at(TokenType::NUMBER)) {
            std::string s = consume().text;
            return FormulaNode::makeNumber(Rational(s));
        }

        // Переменная
        if (at(TokenType::VARIABLE)) {
            std::string name = consume().text;
            return FormulaNode::makeVariable(name);
        }

        throw std::runtime_error("Parser: unexpected token '" + cur().text + "'");
    }
};

NodePtr parseInfix(const std::string& expr) {
    auto toks = tokenize(expr);
    Parser p{toks, 0};
    NodePtr root = p.parseExpr();
    if (!p.at(TokenType::END))
        throw std::runtime_error("Parser: trailing tokens");
    return root;
}

// ─── Построение префиксной строки по дереву ──────────────────────────────────
static std::string treeToPrefixStr(const NodePtr& node) {
    if (!node) return "";

    switch (node->type) {
        case NodeType::NUMBER:
            return rationalStr(node->value);
        case NodeType::VARIABLE:
            return node->varName;
        default:
            break;
    }

    std::string sym = nodeTypeToSymbol(node->type);

    if (node->isUnary()) {
        return sym + " " + treeToPrefixStr(node->children[0]);
    }
    if (node->isBinary()) {
        return sym + " " + treeToPrefixStr(node->children[0])
                   + " " + treeToPrefixStr(node->children[1]);
    }
    return sym;
}

// ─── М1 ──────────────────────────────────────────────────────────────────────
std::string infixToPrefix(const std::string& expr) {
    NodePtr tree = parseInfix(expr);
    return treeToPrefixStr(tree);
}
