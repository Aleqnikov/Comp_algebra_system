#ifndef MODULE11_LIKE_TERMS_POSTFIX_H
#define MODULE11_LIKE_TERMS_POSTFIX_H

#include <string>
#include <vector>

// М11: приведение подобных слагаемых по постфиксной записи
// Реализовано через М2 + М12 + М4
std::vector<std::string> collectLikeTermsPostfix(const std::vector<std::string>& postfix);
std::string collectLikeTermsPostfixStr(const std::string& postfixStr);

#endif
