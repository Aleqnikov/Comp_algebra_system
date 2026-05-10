#include "module10_simplify_postfix.h"
#include "module02_from_postfix.h" // только для splitPostfix
#include "rational_utils.h"
#include <stack>
#include <vector>
#include <string>

// Проверки прямо на строке токена
static bool isZeroTok(const std::string& s) {
    try { Rational r(s); r.reduce(); return rationalIsZero(r); }
    catch(...) { return false; }
}
static bool isOneTok(const std::string& s) {
    try { Rational r(s); r.reduce(); return rationalIsOne(r); }
    catch(...) { return false; }
}
static bool isNumberTok(const std::string& s) {
    if (s.empty()) return false;
    size_t i = (s[0]=='-') ? 1 : 0;
    if (i == s.size()) return false;
    while (i < s.size() && std::isdigit((unsigned char)s[i])) ++i;
    if (i < s.size() && s[i]=='/') {
        ++i;
        while (i < s.size() && std::isdigit((unsigned char)s[i])) ++i;
    }
    return i == s.size();
}

// Постфиксный токен — это или число, или переменная, или оператор
// Стек хранит постфиксные подвыражения (векторы токенов)
using Expr = std::vector<std::string>;

static bool exprIsZero(const Expr& e) {
    return e.size() == 1 && isZeroTok(e[0]);
}
static bool exprIsOne(const Expr& e) {
    return e.size() == 1 && isOneTok(e[0]);
}
static Expr makeNum(const std::string& s) { return {s}; }

// Применить оператор с упрощением, вернуть результирующее постфиксное подвыражение
static Expr applyOp(const std::string& op, const Expr& l, const Expr& r) {
    // Свёртка двух числовых констант
    if (l.size()==1 && r.size()==1 && isNumberTok(l[0]) && isNumberTok(r[0])) {
        try {
            Rational a(l[0]), b(r[0]);
            Rational res("0");
            bool ok = true;
            if      (op=="+") res = a+b;
            else if (op=="-") res = a-b;
            else if (op=="*") res = a*b;
            else if (op=="/") { if(!rationalIsZero(b)) res=a/b; else ok=false; }
            else ok = false;
            if (ok) { res.reduce(); return {rationalStr(res)}; }
        } catch(...) {}
    }

    if (op == "+") {
        if (exprIsZero(l)) return r;
        if (exprIsZero(r)) return l;
    }
    if (op == "-") {
        if (exprIsZero(r)) return l;
        if (l == r) return makeNum("0");
    }
    if (op == "*") {
        if (exprIsZero(l) || exprIsZero(r)) return makeNum("0");
        if (exprIsOne(l)) return r;
        if (exprIsOne(r)) return l;
    }
    if (op == "/") {
        if (exprIsZero(l)) return makeNum("0");
        if (exprIsOne(r)) return l;
    }
    if (op == "^") {
        if (exprIsZero(r)) return makeNum("1");
        if (exprIsOne(r))  return l;
        if (exprIsZero(l)) return makeNum("0");
    }

    // Не упрощается — собираем постфикс: l r op
    Expr res;
    res.insert(res.end(), l.begin(), l.end());
    res.insert(res.end(), r.begin(), r.end());
    res.push_back(op);
    return res;
}

static Expr applyUnary(const std::string& op, const Expr& arg) {
    if (op == "neg" && arg.size()==1 && isNumberTok(arg[0])) {
        try {
            Rational r(arg[0]);
            Integer neg = -r.getNumerator();
            Rational res(neg, r.getDenominator());
            res.reduce();
            return {rationalStr(res)};
        } catch(...) {}
    }
    Expr res;
    res.insert(res.end(), arg.begin(), arg.end());
    res.push_back(op);
    return res;
}

// М10: прямая обработка постфиксного вектора
std::vector<std::string> simplifyPostfix(const std::vector<std::string>& postfix) {
    std::stack<Expr> st;

    static const std::vector<std::string> BINARY = {"+","-","*","/","^"};
    static const std::vector<std::string> UNARY1 = {"neg","sin","cos","tan","cot","sqrt","exp"};
    static const std::vector<std::string> UNARY2 = {"log"}; // бинарная функция

    auto isBin = [&](const std::string& s) {
        for (auto& b: BINARY) if (b==s) return true;
        return false;
    };
    auto isUnary = [&](const std::string& s) {
        for (auto& u: UNARY1) if (u==s) return true;
        return false;
    };

    for (const std::string& tok : postfix) {
        if (isBin(tok)) {
            if (st.size() < 2) { st.push({tok}); continue; }
            Expr r = st.top(); st.pop();
            Expr l = st.top(); st.pop();
            st.push(applyOp(tok, l, r));
        } else if (isUnary(tok)) {
            if (st.empty()) { st.push({tok}); continue; }
            Expr arg = st.top(); st.pop();
            st.push(applyUnary(tok, arg));
        } else if (tok == "log") {
            if (st.size() < 2) { st.push({tok}); continue; }
            Expr arg  = st.top(); st.pop();
            Expr base = st.top(); st.pop();
            Expr res;
            res.insert(res.end(), base.begin(), base.end());
            res.insert(res.end(), arg.begin(),  arg.end());
            res.push_back("log");
            st.push(res);
        } else {
            // число или переменная
            st.push({tok});
        }
    }

    if (st.empty()) return {};
    return st.top();
}

std::string simplifyPostfixStr(const std::string& postfixStr) {
    auto v = simplifyPostfix(splitPostfix(postfixStr));
    std::string s;
    for (size_t i = 0; i < v.size(); ++i) { if(i) s+=' '; s+=v[i]; }
    return s;
}