#ifndef FORMULA_TREE_H
#define FORMULA_TREE_H

#include <memory>
#include <string>
#include <vector>
// Подключи свой путь к Rational:
#include "../lib/src/algstructures/Rational.h"

enum class NodeType {
    NUMBER, VARIABLE,
    ADD, SUB, MUL, DIV, POW,
    NEG,
    SIN, COS, TAN, COT, SQRT, EXP, LOG
};

struct FormulaNode {
    NodeType type;
    Rational value;          // только NUMBER
    std::string varName;     // только VARIABLE
    std::vector<std::shared_ptr<FormulaNode>> children; // 0/1/2

    // Статические фабрики
    static std::shared_ptr<FormulaNode> makeNumber(const Rational& r);
    static std::shared_ptr<FormulaNode> makeVariable(const std::string& name);
    static std::shared_ptr<FormulaNode> makeUnary(NodeType t,
        std::shared_ptr<FormulaNode> child);
    static std::shared_ptr<FormulaNode> makeBinary(NodeType t,
        std::shared_ptr<FormulaNode> left,
        std::shared_ptr<FormulaNode> right);

    // Вспомогательные
    bool isLeaf()   const { return children.empty(); }
    bool isUnary()  const { return children.size() == 1; }
    bool isBinary() const { return children.size() == 2; }

    FormulaNode() : value(Rational("0")) {}
};

using NodePtr = std::shared_ptr<FormulaNode>;

// Утилиты
bool   nodeTypeIsFunc(NodeType t);   // SIN/COS/TAN/COT/SQRT/EXP/LOG
bool   nodeTypeIsUnaryOp(NodeType t);// NEG
bool   nodeTypeIsBinaryOp(NodeType t);
std::string nodeTypeToSymbol(NodeType t); // "+", "sin", etc.

// Глубокое копирование дерева
NodePtr cloneTree(const NodePtr& node);

#endif