// Демонстрация всех модулей системы символьных вычислений
// Подключи свои пути к Rational/Integer/Natural перед компиляцией

#include <iostream>
#include "module01_to_prefix.h"
#include "module02_from_postfix.h"
#include "module03_diff.h"
#include "module04_tree_to_postfix.h"
#include "module05_postfix_to_str.h"
#include "module06_tree_to_str.h"
#include "module07_assoc.h"
#include "module08_priority.h"
#include "module09_simplify_tree.h"
#include "module10_simplify_postfix.h"
#include "module11_like_terms_postfix.h"
#include "module12_like_terms_tree.h"
#include "module13_commutative.h"

void demo(const std::string& expr) {
    std::cout << "\n============================\n";
    std::cout << "Выражение: " << expr << "\n";

    // М1
    std::string prefix = infixToPrefix(expr);
    std::cout << "[М1] Префикс:   " << prefix << "\n";

    // Дерево из инфикса
    NodePtr tree = parseInfix(expr);

    // М4
    std::cout << "[М4] Постфикс:  " << treeToPostfixStr(tree) << "\n";

    // М6
    std::cout << "[М6] Полная:    " << treeToFullString(tree) << "\n";

    // М8 — без лишних скобок
    std::cout << "[М8] Приоритет: " << treeToStringPriority(tree) << "\n";

    // М7
    std::cout << "[М7] Ассоц:     " << treeToStringAssoc(tree) << "\n";

    // М5 (постфикс → строка)
    auto postfix = treeToPostfix(tree);
    std::cout << "[М5] Из постф:  " << postfixToFullString(postfix) << "\n";

    // М2 (постфикс → дерево → М6)
    NodePtr tree2 = postfixToTree(postfix);
    std::cout << "[М2→М6]:        " << treeToFullString(tree2) << "\n";

    // М3 — производная по x
    NodePtr diff = differentiate(tree, "x");
    std::cout << "[М3] d/dx:      " << treeToStringPriority(diff) << "\n";

    // М9 — упрощение производной
    NodePtr simplified = simplifyTree(diff);
    std::cout << "[М9] Упрощ:     " << treeToStringPriority(simplified) << "\n";

    // М9+М12 комбо
    NodePtr simplLike = simplifyTree(collectLikeTerms(simplified));
    std::cout << "[М9+М12]:       " << treeToStringPriority(simplLike) << "\n";

    // М12 — подобные слагаемые по дереву
    NodePtr like = collectLikeTerms(tree);
    std::cout << "[М12] Подобные: " << treeToStringPriority(like) << "\n";

    // М13 — сортировка множителей
    NodePtr sorted = sortFactors(tree);
    std::cout << "[М13] Сортир:   " << treeToStringPriority(sorted) << "\n";
}

int main() {
    try {
        demo("sin(x)+3*x^2");
        demo("3*x+5*x+2*y");
        demo("x*sin(x)*2*3");
        demo("(x+0)*(1*y)");
        demo("log(2,x)+x^2-x^2");

        // Прямые тесты М10 и М11 по постфиксу (без дерева)
        std::cout << "\n=== Прямые тесты М10/М11 (без дерева) ===\n";
        struct PTest { std::string name, postfix; };
        std::vector<PTest> ptests = {
            {"x+0",         "x 0 +"},
            {"1*y",         "1 y *"},
            {"3*x+5*x+2*y", "3 x * 5 x * + 2 y * +"},
            {"x^2-x^2",     "x 2 ^ x 2 ^ -"},
            {"0*sin(x)+y*1","0 x sin * y 1 * +"},
        };
        for (auto& t : ptests) {
            auto pf = splitPostfix(t.postfix);
            std::cout << "\nПостфикс: " << t.postfix << "\n";
            std::cout << "[М10]: " << simplifyPostfixStr(t.postfix) << "\n";
            std::cout << "[М11]: " << collectLikeTermsPostfixStr(t.postfix) << "\n";
        }
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
    }
    return 0;
}