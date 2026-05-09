#include "module10_simplify_postfix.h"
#include "module02_from_postfix.h"
#include "module09_simplify_tree.h"
#include "module04_tree_to_postfix.h"

std::vector<std::string> simplifyPostfix(const std::vector<std::string>& postfix) {
    auto tree = postfixToTree(postfix);
    auto simplified = simplifyTree(tree);
    return treeToPostfix(simplified);
}

std::string simplifyPostfixStr(const std::string& postfixStr) {
    auto v = simplifyPostfix(splitPostfix(postfixStr));
    std::string s;
    for (size_t i = 0; i < v.size(); ++i) { if(i) s+=' '; s+=v[i]; }
    return s;
}
