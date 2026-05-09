#include "module11_like_terms_postfix.h"
#include "module02_from_postfix.h"
#include "module12_like_terms_tree.h"
#include "module04_tree_to_postfix.h"

std::vector<std::string> collectLikeTermsPostfix(const std::vector<std::string>& postfix) {
    auto tree = postfixToTree(postfix);
    auto simplified = collectLikeTerms(tree);
    return treeToPostfix(simplified);
}

std::string collectLikeTermsPostfixStr(const std::string& postfixStr) {
    auto v = collectLikeTermsPostfix(splitPostfix(postfixStr));
    std::string s;
    for (size_t i = 0; i < v.size(); ++i) { if(i) s+=' '; s+=v[i]; }
    return s;
}
