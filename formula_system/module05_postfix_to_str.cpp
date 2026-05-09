#include "module05_postfix_to_str.h"
#include "module02_from_postfix.h"
#include "module06_tree_to_str.h"

std::string postfixToFullString(const std::vector<std::string>& postfix) {
    return treeToFullString(postfixToTree(postfix));
}

std::string postfixStrToFullString(const std::string& postfixStr) {
    return postfixToFullString(splitPostfix(postfixStr));
}
