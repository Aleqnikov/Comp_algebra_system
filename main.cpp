#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QFile>
#include <QDateTime>
#include <QScrollArea>
#include <QGroupBox>
#include <QSplitter>
#include <QTextStream>

#include "lib/src/Exceptions/UniversalStringException.h"
#include "lib/src/Validator/Validator.h"
#include "lib/src/algstructures/Natural.h"
#include "lib/src/algstructures/Integer.h"
#include "lib/src/algstructures/Rational.h"
#include "lib/src/algstructures/Polynom.h"

#include "formula_system/module01_to_prefix.h"
#include "formula_system/module02_from_postfix.h"
#include "formula_system/module03_diff.h"
#include "formula_system/module04_tree_to_postfix.h"
#include "formula_system/module05_postfix_to_str.h"
#include "formula_system/module06_tree_to_str.h"
#include "formula_system/module07_assoc.h"
#include "formula_system/module08_priority.h"
#include "formula_system/module09_simplify_tree.h"
#include "formula_system/module10_simplify_postfix.h"
#include "formula_system/module11_like_terms_postfix.h"
#include "formula_system/module12_like_terms_tree.h"
#include "formula_system/module13_commutative.h"

// ── Темы ─────────────────────────────────────────────────────────────────────
namespace Theme {
const QString dark = R"(
    QWidget { background-color:#1e1e1e; color:#d4d4d4; }
    QPushButton { background:#2d2d30; color:#d4d4d4; border:1px solid #3e3e42; border-radius:4px; padding:6px 14px; font-weight:bold; }
    QPushButton:hover  { background:#38383c; border-color:#007acc; }
    QPushButton:pressed{ background:#007acc; color:#fff; }
    QLineEdit,QTextEdit { background:#252526; color:#d4d4d4; border:1px solid #3e3e42; border-radius:4px; padding:5px; }
    QLabel { background:transparent; color:#d4d4d4; }
    QTabWidget::pane { border:1px solid #3e3e42; background:#252526; }
    QTabBar::tab { background:#2d2d30; color:#9e9e9e; padding:8px 20px; border:1px solid #3e3e42; border-bottom:none; border-top-left-radius:4px; border-top-right-radius:4px; }
    QTabBar::tab:selected { background:#252526; color:#fff; }
    QGroupBox { border:1px solid #3e3e42; border-radius:4px; margin-top:10px; padding-top:6px; color:#9cdcfe; font-weight:bold; }
    QGroupBox::title { subcontrol-origin:margin; left:10px; }
    QScrollArea { border:none; background:transparent; }
    QSplitter::handle { background:#3e3e42; width:2px; }
)";
const QString light = R"(
    QWidget { background-color:#f5f5f5; color:#333; }
    QPushButton { background:#fff; color:#333; border:1px solid #ccc; border-radius:4px; padding:6px 14px; font-weight:bold; }
    QPushButton:hover  { background:#e3f0fb; border-color:#0078d4; }
    QPushButton:pressed{ background:#0078d4; color:#fff; }
    QLineEdit,QTextEdit { background:#fff; color:#333; border:1px solid #ccc; border-radius:4px; padding:5px; }
    QLabel { background:transparent; color:#333; }
    QTabWidget::pane { border:1px solid #ccc; background:#fff; }
    QTabBar::tab { background:#e6e6e6; color:#666; padding:8px 20px; border:1px solid #ccc; border-bottom:none; border-top-left-radius:4px; border-top-right-radius:4px; }
    QTabBar::tab:selected { background:#fff; color:#0078d4; font-weight:bold; }
    QGroupBox { border:1px solid #ccc; border-radius:4px; margin-top:10px; padding-top:6px; color:#0078d4; font-weight:bold; }
    QGroupBox::title { subcontrol-origin:margin; left:10px; }
    QScrollArea { border:none; background:transparent; }
    QSplitter::handle { background:#ccc; width:2px; }
)";
}

static bool isDark = true;
static void showErr(const std::string& m){ QMessageBox::critical(nullptr,"Ошибка",QString::fromStdString(m)); }
static void setResult(QTextEdit* f,const std::string& s){ f->setPlainText(QString::fromStdString(s)); }

static NodePtr fullSimplify(const NodePtr& t){
    NodePtr c=t;
    for(int i=0;i<6;++i){
        NodePtr n=sortFactors(simplifyTree(collectLikeTerms(simplifyTree(c))));
        if(treeToPostfixStr(n)==treeToPostfixStr(c)) break;
        c=n;
    }
    return c;
}

// ── Справка (диалог) ─────────────────────────────────────────────────────────
static void showHelp(){
    QDialog* d=new QDialog();
    d->setWindowTitle("Справка — синтаксис формул");
    d->resize(560,520);
    d->setStyleSheet(isDark?Theme::dark:Theme::light);
    auto* L=new QVBoxLayout(d);
    auto* sc=new QScrollArea(); sc->setWidgetResizable(true);
    auto* w=new QWidget(); auto* vl=new QVBoxLayout(w); vl->setContentsMargins(16,12,16,12); vl->setSpacing(10);

    auto T=[&](const QString& s){ auto* l=new QLabel(s); l->setStyleSheet("font-size:13px;font-weight:bold;color:#007acc;margin-top:6px;"); vl->addWidget(l); };
    auto P=[&](const QString& s){ auto* l=new QLabel(s); l->setWordWrap(true); l->setStyleSheet("font-size:12px;"); vl->addWidget(l); };
    auto C=[&](const QString& s){ auto* l=new QLabel(s); l->setFont(QFont("Monospace",11)); l->setStyleSheet("background:#252526;color:#9cdcfe;padding:8px;border-radius:4px;border:1px solid #3e3e42;"); l->setTextInteractionFlags(Qt::TextSelectableByMouse); vl->addWidget(l); };

    T("Операции");
    C("x + y      сложение\nx - y      вычитание\nx * y      умножение\nx / y      деление\nx ^ y      степень  (правоассоциативно: x^y^z = x^(y^z))");
    T("Функции");
    C("sin(x)     cos(x)     tan(x)     cot(x)\nsqrt(x)    квадратный корень\nexp(x)     показательная  e^x\nlog(a, x)  логарифм по основанию a");
    T("Числа");
    C("3          целое\n-7         отрицательное\n3/4        дробное\nx, y, t    переменные (любые буквы)");
    T("Примеры формул");
    C("sin(x) + 3*x^2\nlog(2, x) + x^2 - x^2\nexp(x) * cos(x)\nsqrt(x^2 + y^2)\n(x + 1) / (x^2 - 1)\nx^(3/2)");
    T("Что делает кнопка «Вычислить»");
    P("Вычисляет производную введённой формулы по выбранной переменной и автоматически упрощает результат:\nМ9 (0-слагаемые, 1-множители) + М12 (подобные) + М13 (сортировка множителей).");
    vl->addStretch();
    sc->setWidget(w);
    L->addWidget(sc);
    auto* ok=new QPushButton("Закрыть"); ok->setFixedWidth(100);
    QObject::connect(ok,&QPushButton::clicked,d,&QDialog::accept);
    auto* br=new QHBoxLayout(); br->addStretch(); br->addWidget(ok);
    L->addLayout(br);
    d->exec();
    delete d;
}

// ── Вкладка «Дифференцирование» ──────────────────────────────────────────────
static QWidget* buildDiffTab(QTextEdit* draft, QPushButton* copyBtn){
    auto* outer=new QTabWidget();

    // ── Подвкладка «Вычислить» ──────────────────────────────────────────────
    {
        auto* w=new QWidget();
        auto* L=new QVBoxLayout(w); L->setContentsMargins(20,20,20,20); L->setSpacing(14);

        // Строка: поле + кнопка справки
        auto* topRow=new QHBoxLayout();
        auto* formulaEdit=new QLineEdit();
        formulaEdit->setPlaceholderText("Введите формулу, например:  sin(x) + 3*x^2");
        formulaEdit->setFont(QFont("Monospace",12));
        formulaEdit->setMinimumHeight(38);
        topRow->addWidget(formulaEdit,1);
        auto* helpBtn=new QPushButton("?");
        helpBtn->setFixedSize(38,38);
        helpBtn->setStyleSheet(
            "QPushButton{background:#e6a800;color:#1e1e1e;border:none;border-radius:4px;font-size:16px;font-weight:bold;}"
            "QPushButton:hover{background:#ffbf00;}");
        helpBtn->setToolTip("Справка по синтаксису");
        QObject::connect(helpBtn,&QPushButton::clicked,[](){ showHelp(); });
        topRow->addWidget(helpBtn);
        L->addLayout(topRow);

        // Строка: переменная
        auto* varRow=new QHBoxLayout();
        varRow->addWidget(new QLabel("Переменная для d/dx:"));
        auto* varEdit=new QLineEdit("x"); varEdit->setMaximumWidth(60); varEdit->setMinimumHeight(32);
        varRow->addWidget(varEdit); varRow->addStretch();
        L->addWidget(new QLabel("")); // небольшой отступ уже есть
        L->addLayout(varRow);

        // Результат
        auto* resLbl=new QLabel("Производная (упрощённая):");
        resLbl->setStyleSheet("font-size:12px; color:#888;");
        L->addWidget(resLbl);
        auto* resultField=new QTextEdit();
        resultField->setReadOnly(true);
        resultField->setFont(QFont("Monospace",13));
        resultField->setMinimumHeight(80);
        resultField->setMaximumHeight(120);
        L->addWidget(resultField);

        // Кнопка
        auto* calcBtn=new QPushButton("▶   Вычислить производную");
        calcBtn->setMinimumHeight(44);
        calcBtn->setStyleSheet(
            "QPushButton{background:#007acc;color:#fff;border:none;border-radius:5px;font-size:14px;font-weight:bold;}"
            "QPushButton:hover{background:#005a9e;}"
            "QPushButton:pressed{background:#003f6b;}");
        L->addWidget(calcBtn);
        L->addStretch();

        auto compute=[=](){
            std::string expr=formulaEdit->text().trimmed().toStdString();
            std::string var =varEdit->text().trimmed().toStdString();
            if(expr.empty()){showErr("Введите формулу");return;}
            if(var.empty()) var="x";
            try{
                NodePtr tree=parseInfix(expr);
                NodePtr diff=differentiate(tree,var);
                NodePtr res =fullSimplify(diff);
                std::string s=treeToStringPriority(res);
                setResult(resultField,s);
                copyBtn->disconnect();
                QObject::connect(copyBtn,&QPushButton::clicked,[=](){
                    auto d=draft->toPlainText();
                    if(!d.isEmpty()) d+="\n\n";
                    draft->setPlainText(d+"d/dx["+formulaEdit->text()+"]="+resultField->toPlainText());
                });
            } catch(const std::exception& e){ showErr(e.what()); resultField->clear(); }
        };

        QObject::connect(calcBtn,&QPushButton::clicked,compute);
        QObject::connect(formulaEdit,&QLineEdit::returnPressed,compute);

        outer->addTab(w,"▶  Вычислить");
    }

    // ── Подвкладка «Инструменты» ─────────────────────────────────────────────
    {
        auto* w=new QWidget();
        auto* L=new QVBoxLayout(w); L->setContentsMargins(12,12,12,12); L->setSpacing(10);

        auto* ig=new QGroupBox("Формула");
        auto* igl=new QVBoxLayout(ig);
        auto* fEdit=new QLineEdit(); fEdit->setPlaceholderText("Введите формулу..."); fEdit->setFont(QFont("Monospace",11));
        igl->addWidget(fEdit);
        auto* vr=new QHBoxLayout(); vr->addWidget(new QLabel("Переменная d/dx:"));
        auto* vEdit=new QLineEdit("x"); vEdit->setMaximumWidth(55); vr->addWidget(vEdit); vr->addStretch();
        igl->addLayout(vr);
        L->addWidget(ig);

        auto* rg=new QGroupBox("Результат");
        auto* rgl=new QVBoxLayout(rg);
        auto* rf=new QTextEdit(); rf->setReadOnly(true); rf->setFont(QFont("Monospace",10)); rf->setMaximumHeight(65);
        rgl->addWidget(rf);
        L->addWidget(rg);

        auto run=[=](const QString& mod){
            std::string expr=fEdit->text().trimmed().toStdString();
            std::string var =vEdit->text().trimmed().toStdString();
            if(expr.empty()){showErr("Введите формулу");return;}
            if(var.empty()) var="x";
            try{
                NodePtr t=parseInfix(expr); std::string res;
                if(mod=="М1: Префикс")            res=infixToPrefix(expr);
                else if(mod=="М4: Постфикс")       res=treeToPostfixStr(t);
                else if(mod=="М6: Полные скобки")  res=treeToFullString(t);
                else if(mod=="М7: Ассоциативность")res=treeToStringAssoc(t);
                else if(mod=="М8: Приоритет")      res=treeToStringPriority(t);
                else if(mod=="М3: d/dx")           res=treeToStringPriority(differentiate(t,var));
                else if(mod=="М9: Упростить")      res=treeToStringPriority(simplifyTree(t));
                else if(mod=="М9+М12+М13")         res=treeToStringPriority(fullSimplify(t));
                else if(mod=="М12: Подобные")      res=treeToStringPriority(collectLikeTerms(t));
                else if(mod=="М13: Сортировка")    res=treeToStringPriority(sortFactors(t));
                setResult(rf,res);
                copyBtn->disconnect();
                QObject::connect(copyBtn,&QPushButton::clicked,[=](){
                    auto d=draft->toPlainText(); if(!d.isEmpty()) d+="\n\n";
                    draft->setPlainText(d+"["+mod+"|"+fEdit->text()+"]: "+rf->toPlainText());
                });
            } catch(const std::exception& e){ showErr(e.what()); }
        };

        struct G { QString name; QVector<QString> btns; };
        QVector<G> groups={
            {"Запись",      {"М1: Префикс","М4: Постфикс","М6: Полные скобки"}},
            {"Скобки",      {"М7: Ассоциативность","М8: Приоритет"}},
            {"Упрощение",   {"М9: Упростить","М12: Подобные","М13: Сортировка","М9+М12+М13"}},
            {"Дифференц.",  {"М3: d/dx"}},
        };
        for(auto& g:groups){
            auto* grp=new QGroupBox(g.name); auto* gl=new QHBoxLayout(grp); gl->setSpacing(6);
            for(auto& b:g.btns){ auto* btn=new QPushButton(b); gl->addWidget(btn); QObject::connect(btn,&QPushButton::clicked,[=](){ run(b); }); }
            L->addWidget(grp);
        }
        L->addStretch();
        outer->addTab(w,"⚙  Инструменты");
    }

    return outer;
}

// ── Вкладка CAS ──────────────────────────────────────────────────────────────
static QWidget* buildCasTab(QTextEdit* draft, QPushButton* copyBtn){
    struct CM{ QString name,cat,desc; int inputs; };
    const CM mods[]={
        {"Сравнение натуральных","Натуральные","0=равны,1=меньше,2=больше",2},
        {"Сложение натуральных","Натуральные","a+b",2},
        {"Вычитание меньшего","Натуральные","a−b (a≥b)",2},
        {"Умножение натуральных","Натуральные","a×b",2},
        {"Неполное частное","Натуральные","⌊a/b⌋",2},
        {"Остаток от деления","Натуральные","a mod b",2},
        {"НОД","Натуральные","НОД(a,b)",2},
        {"НОК","Натуральные","НОК(a,b)",2},
        {"Абсолютная величина","Целые","|a|",1},
        {"Сложение целых","Целые","a+b",2},
        {"Вычитание целых","Целые","a−b",2},
        {"Умножение целых","Целые","a×b",2},
        {"Частное целых","Целые","⌊a/b⌋",2},
        {"Остаток целых","Целые","a mod b",2},
        {"Сокращение дроби","Дроби","Формат: -14/3",1},
        {"Проверка на целое","Дроби","true/false",1},
        {"Сложение дробей","Дроби","a/b+c/d",2},
        {"Вычитание дробей","Дроби","a/b−c/d",2},
        {"Умножение дробей","Дроби","a/b×c/d",2},
        {"Деление дробей","Дроби","a/b÷c/d",2},
        {"Сложение многочленов","Многочлены","P+Q",2},
        {"Вычитание многочленов","Многочлены","P−Q",2},
        {"Умножение на дробь","Многочлены","P×r",2},
        {"Умножение многочленов","Многочлены","P×Q",2},
        {"Частное деления","Многочлены","P div Q",2},
        {"Остаток деления","Многочлены","P mod Q",2},
        {"НОД многочленов","Многочлены","НОД(P,Q)",2},
        {"Старший коэффициент","Многочлены","lc(P)",1},
        {"Степень многочлена","Многочлены","deg(P)",1},
        {"Производная","Многочлены","P'",1},
        {"Кратные в простые","Многочлены","sqfree(P)",1},
    };
    int N=sizeof(mods)/sizeof(mods[0]);

    auto* modArea=new QWidget(); auto* modL=new QVBoxLayout(modArea); modL->setAlignment(Qt::AlignTop);
    modL->addWidget(new QLabel("← Выберите операцию"));

    auto buildPanel=[=](const CM& m){
        while(modL->count()){ auto* i=modL->takeAt(0); if(i->widget()) i->widget()->deleteLater(); delete i; }
        auto* t=new QLabel(m.name); QFont f=t->font(); f.setBold(true); f.setPointSize(13); t->setFont(f); modL->addWidget(t);
        auto* dd=new QLabel(m.desc); dd->setStyleSheet("color:#888;font-style:italic;"); modL->addWidget(dd);
        modL->addSpacing(8);
        QVector<QLineEdit*> ins;
        for(int i=0;i<m.inputs;++i){ modL->addWidget(new QLabel(QString("Аргумент %1:").arg(i+1))); auto* le=new QLineEdit(); modL->addWidget(le); ins.append(le); }
        modL->addSpacing(6); modL->addWidget(new QLabel("Результат:"));
        auto* rf=new QTextEdit(); rf->setReadOnly(true); rf->setMaximumHeight(75);
        modL->addWidget(rf);
        auto* btn=new QPushButton("Вычислить"); modL->addWidget(btn); modL->addStretch();
        copyBtn->disconnect();
        QObject::connect(copyBtn,&QPushButton::clicked,[=](){ auto d=draft->toPlainText(); if(!d.isEmpty()) d+="\n\n"; draft->setPlainText(d+"["+m.name+"]: "+rf->toPlainText()); });
        QObject::connect(btn,&QPushButton::clicked,[=](){
            try{
                QVector<std::string> in; for(auto* le:ins) in.append(le->text().toStdString());
                std::string res; const QString& n=m.name;
                if(n=="Сравнение натуральных"){ Natural b(in[1]); res=std::string(1,Natural(in[0]).cmp(&b)+'0'); }
                else if(n=="Сложение натуральных")  res=(Natural(in[0])+Natural(in[1])).toString();
                else if(n=="Вычитание меньшего")    res=(Natural(in[0])-Natural(in[1])).toString();
                else if(n=="Умножение натуральных") res=(Natural(in[0])*Natural(in[1])).toString();
                else if(n=="Неполное частное")      res=(Natural(in[0])/Natural(in[1])).toString();
                else if(n=="Остаток от деления")    res=(Natural(in[0])%Natural(in[1])).toString();
                else if(n=="НОД")                   res=Natural::gcd(Natural(in[0]),Natural(in[1])).toString();
                else if(n=="НОК")                   res=Natural::lcm(Natural(in[0]),Natural(in[1])).toString();
                else if(n=="Абсолютная величина")   res=Integer(in[0]).abs().toString();
                else if(n=="Сложение целых")        res=(Integer(in[0])+Integer(in[1])).toString();
                else if(n=="Вычитание целых")       res=(Integer(in[0])-Integer(in[1])).toString();
                else if(n=="Умножение целых")       res=(Integer(in[0])*Integer(in[1])).toString();
                else if(n=="Частное целых")         res=(Integer(in[0])/Integer(in[1])).toString();
                else if(n=="Остаток целых")         res=(Integer(in[0])%Integer(in[1])).toString();
                else if(n=="Сокращение дроби")      { Rational r(in[0]); r.reduce(); res=r.toString(); }
                else if(n=="Проверка на целое")     res=Rational(in[0]).isInteger()?"true":"false";
                else if(n=="Сложение дробей")       res=(Rational(in[0])+Rational(in[1])).toString();
                else if(n=="Вычитание дробей")      res=(Rational(in[0])-Rational(in[1])).toString();
                else if(n=="Умножение дробей")      res=(Rational(in[0])*Rational(in[1])).toString();
                else if(n=="Деление дробей")        res=(Rational(in[0])/Rational(in[1])).toString();
                else if(n=="Сложение многочленов")  res=(Polynom(Validator::validatePolynomial(in[0]))+Polynom(Validator::validatePolynomial(in[1]))).toString();
                else if(n=="Вычитание многочленов") res=(Polynom(Validator::validatePolynomial(in[0]))-Polynom(Validator::validatePolynomial(in[1]))).toString();
                else if(n=="Умножение на дробь")    res=(Polynom(Validator::validatePolynomial(in[0]))*Rational(in[1])).toString();
                else if(n=="Умножение многочленов") res=(Polynom(Validator::validatePolynomial(in[0]))*Polynom(Validator::validatePolynomial(in[1]))).toString();
                else if(n=="Частное деления")       res=(Polynom(Validator::validatePolynomial(in[0]))/Polynom(Validator::validatePolynomial(in[1]))).toString();
                else if(n=="Остаток деления")       res=(Polynom(Validator::validatePolynomial(in[0]))%Polynom(Validator::validatePolynomial(in[1]))).toString();
                else if(n=="НОД многочленов")       res=Polynom::gcd(Polynom(Validator::validatePolynomial(in[0])),Polynom(Validator::validatePolynomial(in[1]))).toString();
                else if(n=="Старший коэффициент")   res=Polynom(Validator::validatePolynomial(in[0])).getLeadingCoefficient().toString();
                else if(n=="Степень многочлена")    res=std::to_string(Polynom(Validator::validatePolynomial(in[0])).getDegree());
                else if(n=="Производная")           res=Polynom(Validator::validatePolynomial(in[0])).derivative().toString();
                else if(n=="Кратные в простые")     res=Polynom(Validator::validatePolynomial(in[0])).makeSquareFree().toString();
                setResult(rf,res);
            } catch(const UniversalStringException& e){ showErr(e.what()); }
              catch(const std::exception& e){ showErr(e.what()); }
        });
    };

    auto* tabs=new QTabWidget(); tabs->setFixedWidth(230);
    QVector<QString> cats;
    for(int i=0;i<N;++i) if(!cats.contains(mods[i].cat)) cats.append(mods[i].cat);
    for(const QString& cat:cats){
        auto* sc=new QScrollArea(); sc->setWidgetResizable(true);
        auto* iw=new QWidget(); auto* vb=new QVBoxLayout(iw); vb->setAlignment(Qt::AlignTop);
        for(int i=0;i<N;++i){
            if(mods[i].cat!=cat) continue;
            auto* b=new QPushButton(mods[i].name); b->setStyleSheet("text-align:left;padding-left:8px;");
            vb->addWidget(b);
            const CM& ref=mods[i];
            QObject::connect(b,&QPushButton::clicked,[=](){ buildPanel(ref); });
        }
        sc->setWidget(iw); tabs->addTab(sc,cat);
    }
    auto* sp=new QSplitter(Qt::Horizontal); sp->addWidget(tabs); sp->addWidget(modArea);
    sp->setStretchFactor(0,0); sp->setStretchFactor(1,1);
    auto* w=new QWidget(); auto* l=new QHBoxLayout(w); l->setContentsMargins(0,0,0,0); l->addWidget(sp);
    return w;
}

// ── main ─────────────────────────────────────────────────────────────────────
int main(int argc, char* argv[]){
    QApplication app(argc,argv);
    QWidget window;
    window.setWindowTitle("CAS — Система компьютерной алгебры");
    window.resize(980,660);
    window.setStyleSheet(Theme::dark);

    auto* rootL=new QVBoxLayout(&window); rootL->setContentsMargins(0,0,0,0); rootL->setSpacing(0);

    // Топбар
    auto* top=new QWidget(); top->setFixedHeight(44); top->setStyleSheet("background:#007acc;");
    auto* topL=new QHBoxLayout(top); topL->setContentsMargins(14,0,14,0);
    auto* titLbl=new QLabel("⟨CAS⟩  Система компьютерной алгебры"); titLbl->setStyleSheet("color:#fff;font-size:14px;font-weight:bold;");
    topL->addWidget(titLbl); topL->addStretch();
    auto* themeBtn=new QPushButton("☀  Светлая тема");
    themeBtn->setStyleSheet("background:#005a9e;color:#fff;border:none;padding:5px 12px;border-radius:3px;");
    themeBtn->setFixedHeight(30); topL->addWidget(themeBtn);
    rootL->addWidget(top);

    // Сплиттер
    auto* split=new QSplitter(Qt::Horizontal);

    // Две главные вкладки
    auto* mainTabs=new QTabWidget();

    // Черновик
    auto* draftW=new QWidget(); draftW->setMinimumWidth(180); draftW->setMaximumWidth(250);
    auto* draftL=new QVBoxLayout(draftW); draftL->setContentsMargins(6,10,10,10);
    auto* dtitle=new QLabel("📋  Черновик"); dtitle->setStyleSheet("font-weight:bold;"); draftL->addWidget(dtitle);
    auto* draftField=new QTextEdit(); draftField->setPlaceholderText("Промежуточные результаты...");
    draftL->addWidget(draftField,1);
    auto* copyBtn=new QPushButton("← Копировать");
    auto* clearBtn=new QPushButton("🗑  Очистить");
    draftL->addWidget(copyBtn); draftL->addWidget(clearBtn);
    QObject::connect(clearBtn,&QPushButton::clicked,draftField,&QTextEdit::clear);

    mainTabs->addTab(buildDiffTab(draftField,copyBtn), "∫  Дифференцирование");
    mainTabs->addTab(buildCasTab(draftField,copyBtn),  "№  CAS");

    split->addWidget(mainTabs); split->addWidget(draftW);
    split->setStretchFactor(0,1); split->setStretchFactor(1,0);
    split->setSizes({760,210});
    rootL->addWidget(split,1);

    QObject::connect(themeBtn,&QPushButton::clicked,[&](){
        isDark=!isDark;
        window.setStyleSheet(isDark?Theme::dark:Theme::light);
        themeBtn->setText(isDark?"☀  Светлая тема":"🌙  Тёмная тема");
    });

    window.show();
    return app.exec();
}