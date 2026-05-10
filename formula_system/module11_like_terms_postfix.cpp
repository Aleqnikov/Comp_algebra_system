#include "module11_like_terms_postfix.h"
#include "module02_from_postfix.h"
#include "rational_utils.h"
#include <stack>
#include <vector>
#include <string>
#include <map>

using Expr = std::vector<std::string>;

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

static std::string exprToStr(const Expr& e) {
    std::string s;
    for (size_t i=0; i<e.size(); ++i) { if(i) s+=' '; s+=e[i]; }
    return s;
}

// Разобрать постфиксное выражение на (коэффициент, тело)
// Тело — постфикс без числового множителя
static std::pair<Rational, Expr> splitTerm(const Expr& e) {
    // Просто число
    if (e.size()==1 && isNumberTok(e[0]))
        return {Rational(e[0]), {"1"}};

    // число body * — число первый токен
    if (e.size()>=3 && e.back()=="*" && isNumberTok(e[0])) {
        Rational c(e[0]);
        Expr body(e.begin()+1, e.end()-1);
        return {c, body};
    }

    // body число * — число предпоследний
    if (e.size()>=3 && e.back()=="*" && isNumberTok(e[e.size()-2])) {
        Rational c(e[e.size()-2]);
        Expr body(e.begin(), e.end()-2);
        return {c, body};
    }

    return {Rational("1"), e};
}

static Expr buildTerm(Rational c, const Expr& body) {
    c.reduce();
    bool bodyIsOne = (body.size()==1 && body[0]=="1");
    if (bodyIsOne) return {rationalStr(c)};
    if (rationalIsOne(c)) return body;
    Expr res = {rationalStr(c)};
    res.insert(res.end(), body.begin(), body.end());
    res.push_back("*");
    return res;
}

// Структура: каждый элемент стека — это набор слагаемых (уже разобранных)
// либо "атомарное" выражение (не сумма)
struct StackItem {
    bool isSumList; // true — список слагаемых; false — атомарное выражение
    Expr atom;      // если !isSumList
    // если isSumList — данные в глобальных order/coeffMap/bodyMap
    // Но проще: храним плоский постфикс и разбираем его
};

std::vector<std::string> collectLikeTermsPostfix(const std::vector<std::string>& postfix) {
    // Стек хранит постфиксные подвыражения
    std::stack<Expr> st;

    auto isBinArith = [](const std::string& s){
        return s=="*"||s=="/"||s=="^";
    };
    auto isFunc = [](const std::string& s){
        return s=="sin"||s=="cos"||s=="tan"||s=="cot"||
               s=="sqrt"||s=="exp"||s=="log"||s=="neg";
    };

    for (const std::string& tok : postfix) {
        if (tok == "+" || tok == "-") {
            Expr r = st.top(); st.pop();
            Expr l = st.top(); st.pop();
            // Сразу раскладываем оба на слагаемые и объединяем
            // Плоский список: l_terms + (знак)*r_terms
            // Но у нас нет рекурсии — просто соединяем постфиксно
            // Анализируем l и r на предмет суммы (последний токен + или -)
            // Здесь важно: мы храним в стеке УЖЕ упрощённые постфиксы
            // Для приведения подобных нам нужен плоский список
            // Создадим его сбором через рекурсивный разбор накопленного постфикса

            // Итог кладём как объединённый постфикс
            Expr res;
            res.insert(res.end(), l.begin(), l.end());
            res.insert(res.end(), r.begin(), r.end());
            res.push_back(tok);
            st.push(res);
        } else if (isBinArith(tok)) {
            Expr r = st.top(); st.pop();
            Expr l = st.top(); st.pop();
            Expr res;
            res.insert(res.end(), l.begin(), l.end());
            res.insert(res.end(), r.begin(), r.end());
            res.push_back(tok);
            st.push(res);
        } else if (isFunc(tok)) {
            Expr a = st.top(); st.pop();
            Expr res;
            res.insert(res.end(), a.begin(), a.end());
            res.push_back(tok);
            st.push(res);
        } else {
            st.push({tok});
        }
    }

    Expr full = st.top();

    // Теперь разбираем full постфикс на плоский список слагаемых
    // с помощью рекурсивного стекового обхода
    std::vector<std::string> order;
    std::map<std::string, Rational> coeffMap;
    std::map<std::string, Expr>     bodyMap;

    // Внутренний стек для разбора суммы
    std::stack<Expr> st2;
    auto isBinArith2 = [](const std::string& s){
        return s=="*"||s=="/"||s=="^";
    };
    auto isFunc2 = [](const std::string& s){
        return s=="sin"||s=="cos"||s=="tan"||s=="cot"||
               s=="sqrt"||s=="exp"||s=="log"||s=="neg";
    };

    // Рекурсивная функция через лямбду невозможна — используем явный стек
    // Вместо этого: обходим full, при + и - раскладываем, иначе собираем

    struct Frame { Expr expr; bool negated; };
    std::stack<Frame> work;
    work.push({full, false});

    while (!work.empty()) {
        Frame fr = work.top(); work.pop();
        const Expr& e = fr.expr;
        bool neg = fr.negated;

        if (e.empty()) continue;

        // Если последний токен + или - — раскладываем
        if (e.back() == "+" || e.back() == "-") {
            // Нужно найти границу правого операнда
            // Для этого считаем баланс токенов (каждый операнд потребляет/производит 1)
            // Идём справа налево: ищем где правый операнд заканчивается
            // Стек-счётчик: +/- и бин.арифм. потребляют 2 дают 1 → delta=-1
            //               унарные: потребляют 1 дают 1 → delta=0
            //               листья: дают 1 → delta=+1
            // Идём справа, останавливаемся когда баланс=1 после обработки правого
            std::string op = e.back();
            int balance = 0;
            int split = (int)e.size()-2; // начинаем с предпоследнего
            while (split >= 0) {
                const std::string& t = e[split];
                if (t=="+"||t=="-"||isBinArith2(t)) balance--;
                else if (isFunc2(t)) { /* delta 0 */ }
                else balance++; // лист
                if (balance == 1) break;
                split--;
            }

            Expr right(e.begin()+split, e.end()-1);
            Expr left(e.begin(), e.begin()+split);

            // Левую часть — тоже раскладываем рекурсивно
            work.push({left, neg});
            // Правую: если op=="-" то меняем знак
            work.push({right, neg ^ (op=="-")});
        } else {
            // Атомарное выражение — добавляем как слагаемое
            auto [coeff, body] = splitTerm(e);
            if (neg) {
                Integer nc = -coeff.getNumerator();
                coeff = Rational(nc, coeff.getDenominator());
            }
            coeff.reduce();
            std::string key = exprToStr(body);
            if (coeffMap.find(key) == coeffMap.end()) {
                order.push_back(key);
                coeffMap.emplace(key, Rational("0"));
                bodyMap[key] = body;
            }
            coeffMap.at(key) = coeffMap.at(key) + coeff;
        }
    }

    // Собираем результат
    std::vector<Expr> parts;
    for (auto& key : order) {
        Rational c = coeffMap.at(key); c.reduce();
        if (rationalIsZero(c)) continue;
        parts.push_back(buildTerm(c, bodyMap.at(key)));
    }

    if (parts.empty()) return {"0"};

    Expr result = parts[0];
    for (size_t i=1; i<parts.size(); ++i) {
        result.insert(result.end(), parts[i].begin(), parts[i].end());
        result.push_back("+");
    }
    return result;
}

std::string collectLikeTermsPostfixStr(const std::string& postfixStr) {
    auto v = collectLikeTermsPostfix(splitPostfix(postfixStr));
    std::string s;
    for (size_t i=0; i<v.size(); ++i) { if(i) s+=' '; s+=v[i]; }
    return s;
}