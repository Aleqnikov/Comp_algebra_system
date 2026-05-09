#ifndef MODULE05_POSTFIX_TO_STR_H
#define MODULE05_POSTFIX_TO_STR_H

#include <string>
#include <vector>

// М5: постфиксная запись → полная скобочная строка
// Реализовано через М2 + М6
std::string postfixToFullString(const std::vector<std::string>& postfix);
std::string postfixStrToFullString(const std::string& postfixStr);

#endif
