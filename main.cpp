//
// Панфилов Григорий Иванович 4385 создал эту программу!
//
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextEdit>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QTextStream>

#include "lib/src/Exceptions/UniversalStringException.h"
#include "lib/src/Validator/Validator.h"
#include "lib/src/algstructures/Natural.h"
#include "lib/src/algstructures/Integer.h"
#include "lib/src/algstructures/Rational.h"
#include "lib/src/algstructures/Polynom.h"


namespace Theme {
    const QString light = R"(
        QMainWindow, QWidget {
            background-color: rgba(240,240,240,0.9); color: #333;
        }
        QPushButton {
            background-color: #fff; color: #333; border: 1px solid #ccc;
            border-radius: 4px; padding: 8px 16px; font-weight: bold;
        }
        QPushButton:hover  { background-color: #e6e6e6; border-color: #adadad; }
        QPushButton:pressed{ background-color: #d4d4d4; }
        QLineEdit, QTextEdit {
            background-color: #fff; color: #333; border: 1px solid #ccc;
            border-radius: 4px; padding: 6px; selection-background-color: #0078d4;
        }
        QLabel { background-color: transparent; color: #333; }
        QTabWidget::pane  { border: 1px solid #ccc; background-color: #fff; }
        QTabBar::tab {
            background-color: #e6e6e6; color: #333; padding: 8px 16px;
            border: 1px solid #ccc; border-bottom: none;
            border-top-left-radius: 4px; border-top-right-radius: 4px;
        }
        QTabBar::tab:selected         { background-color: #fff; }
        QTabBar::tab:hover:!selected  { background-color: #f0f0f0; }
        QMessageBox        { background-color: #fff; color: #333; }
        QMessageBox QLabel { color: #333; }
    )";

    const QString dark = R"(
        QMainWindow, QWidget {
            background-color: rgba(30,30,30,0.9); color: #fff;
        }
        QPushButton {
            background-color: #2d2d30; color: #fff; border: 1px solid #3e3e42;
            border-radius: 4px; padding: 8px 16px; font-weight: bold;
        }
        QPushButton:hover  { background-color: #38383c; border-color: #007acc; }
        QPushButton:pressed{ background-color: #004578; }
        QLineEdit, QTextEdit {
            background-color: #252526; color: #fff; border: 1px solid #3e3e42;
            border-radius: 4px; padding: 6px; selection-background-color: #0078d4;
        }
        QLabel { background-color: transparent; color: #fff; }
        QTabWidget::pane  { border: 1px solid #3e3e42; background-color: #252526; }
        QTabBar::tab {
            background-color: #2d2d30; color: #fff; padding: 8px 16px;
            border: 1px solid #3e3e42; border-bottom: none;
            border-top-left-radius: 4px; border-top-right-radius: 4px;
        }
        QTabBar::tab:selected        { background-color: #252526; }
        QTabBar::tab:hover:!selected { background-color: #38383c; }
        QMessageBox        { background-color: #252526; color: #fff; }
        QMessageBox QLabel { color: #fff; }
    )";
}

// ─────────────────────────────────────────────
//  Структура модуля
// ─────────────────────────────────────────────
struct Module {
    QString name;
    QString category;
    int     inputCount;
    QString description;
};

// ─────────────────────────────────────────────
//  Вспомогательные функции UI
// ─────────────────────────────────────────────
static void showError(const std::string& msg) {
    QMessageBox::critical(nullptr, "Ошибка", QString::fromStdString(msg), QMessageBox::Ok);
}

static void showHelp(bool isDark) {
    const QString helpText = R"(
<h3>Общая информация</h3>
<p>Программа предоставляет различные математические модули для работы с натуральными,
целыми числами, дробями и многочленами.</p>

<h3>Как пользоваться:</h3>
<ol>
<li>Выберите категорию модуля во вкладках слева</li>
<li>Нажмите на кнопку с названием нужного модуля</li>
<li>Введите данные в поля ввода согласно описанию модуля</li>
<li>Нажмите кнопку «Вычислить» для получения результата</li>
<li>Используйте черновик для сохранения промежуточных результатов</li>
</ol>

<h3>Особенности ввода:</h3>
<ul>
<li><b>Дроби:</b> формат «a/b» (например: «3/4», «-14/3»)</li>
<li><b>Многочлены:</b> стандартная форма (например: «x^3+2/5x^2+3x+4»)</li>
</ul>

<h3>Черновик:</h3>
<ul>
<li>Текст сохраняется при переключении между модулями</li>
<li>Кнопка «Копировать результат в черновик» — быстрое сохранение</li>
</ul>

<h3>Темы оформления:</h3>
<p>Переключайтесь между светлой и тёмной темой кнопкой в левой панели.</p>
    )";

    QMessageBox box;
    box.setWindowTitle("Справка");
    box.setText(helpText);
    box.setTextFormat(Qt::RichText);
    box.setIcon(QMessageBox::Information);
    box.setStyleSheet(isDark ? Theme::dark : Theme::light);
    box.exec();
}

// ─────────────────────────────────────────────
//  Вывод результата с обрезкой длинных строк
// ─────────────────────────────────────────────
static void displayResult(QTextEdit* field, const std::string& result, const QString& modName) {
    constexpr size_t MAX_DISPLAY = 2000;
    constexpr size_t MAX_LEN     = 100000;

    if (result.size() > MAX_LEN) {
        QString fileName = "result_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss") + ".txt";
        QString filePath = QDir::currentPath() + "/" + fileName;
        QFile f(filePath);
        if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&f);
            out << QString::fromStdString(result);
        }
        std::string shortened = result.substr(0, MAX_DISPLAY)
            + "\n...\n(Полная версия сохранена в файл: " + filePath.toStdString() + ")";
        field->setPlainText(QString::fromStdString(shortened));
    } else {
        field->setPlainText(QString::fromStdString(result));
    }
}

// ─────────────────────────────────────────────
//  Построение интерфейса модуля (правая панель)
// ─────────────────────────────────────────────
static void showModuleInterface(
        QVBoxLayout*  moduleLayout,
        QLabel*       titleLabel,
        QTextEdit*    draftField,
        QPushButton*  copyBtn,
        const Module& mod)
{
    // Очистка предыдущего содержимого
    while (moduleLayout->count() > 0) {
        QLayoutItem* item = moduleLayout->takeAt(0);
        if (item->widget()) item->widget()->deleteLater();
        delete item;
    }

    titleLabel->setText(mod.name);

    // Описание
    if (!mod.description.isEmpty()) {
        auto* desc = new QLabel(mod.description);
        desc->setWordWrap(true);
        desc->setAlignment(Qt::AlignLeft | Qt::AlignTop);
        QFont f = desc->font(); f.setItalic(true); desc->setFont(f);
        desc->setStyleSheet("color:#666; background:rgba(255,255,255,0.7); padding:8px; border-radius:4px;");
        moduleLayout->addWidget(desc);
        moduleLayout->addSpacing(10);
    }

    // Поля ввода
    QVector<QLineEdit*> inputs;
    for (int i = 0; i < mod.inputCount; ++i) {
        moduleLayout->addWidget(new QLabel(QString("Число %1:").arg(i + 1)));
        auto* field = new QLineEdit();
        moduleLayout->addWidget(field);
        inputs.append(field);
    }

    // Поле результата
    moduleLayout->addWidget(new QLabel("Результат:"));
    auto* resultField = new QTextEdit();
    resultField->setReadOnly(true);
    resultField->setMaximumHeight(100);
    resultField->setLineWrapMode(QTextEdit::WidgetWidth);
    resultField->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    moduleLayout->addWidget(resultField);

    // Кнопка вычисления
    auto* calcBtn = new QPushButton("Вычислить");
    moduleLayout->addWidget(calcBtn);

    // Переподключаем «копировать результат»
    copyBtn->disconnect();
    QObject::connect(copyBtn, &QPushButton::clicked, [=]() {
        const QString res = resultField->toPlainText();
        if (res.isEmpty()) return;
        QString draft = draftField->toPlainText();
        if (!draft.isEmpty()) draft += "\n\n";
        draft += "[" + mod.name + "]: " + res;
        draftField->setPlainText(draft);
        QTextCursor c = draftField->textCursor();
        c.movePosition(QTextCursor::End);
        draftField->setTextCursor(c);
    });

    // ─────────────────────────────────────────
    //  Обработчик кнопки «Вычислить»
    // ─────────────────────────────────────────
    QObject::connect(calcBtn, &QPushButton::clicked, [=]() {
        try {
            QVector<std::string> in;
            for (auto* f : inputs) in.append(f->text().toStdString());

            std::string result;
            const QString& name = mod.name;

            // ── Натуральные и 0 ──────────────────────────────────────
            if (name == "Сравнение натуральных") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                Natural a(in[0]), b(in[1]);
                result = std::string(1, a.cmp(&b) + '0');

            } else if (name == "Проверка на 0") {
                Validator::validateNaturalNumber(in[0]);
                Natural a(in[0]);
                result = (a != 0) ? "1" : "0";

            } else if (name == "Добавление 1") {
                Validator::validateNaturalNumber(in[0]);
                Natural a(in[0]);
                ++a;
                result = a.toString();

            } else if (name == "Сложение натуральных") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = (Natural(in[0]) + Natural(in[1])).toString();

            } else if (name == "Вычитание меньшего") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = (Natural(in[0]) - Natural(in[1])).toString();

            } else if (name == "Умножение на цифру") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                if (in[1].size() != 1)
                    throw UniversalStringException("Второй аргумент — одна цифра (0–9)");
                result = (Natural(in[0]) * (size_t)(in[1][0] - '0')).toString();

            } else if (name == "Умножение на 10^k") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                if (in[1].size() > 20)
                    throw UniversalStringException("Показатель k слишком большой");
                result = Natural(in[0]).multiplyByPowerOfTen(std::stoull(in[1])).toString();

            } else if (name == "Умножение натуральных") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = (Natural(in[0]) * Natural(in[1])).toString();

            } else if (name == "Вычитание умноженного") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                Validator::validateNaturalNumber(in[2]);
                if (in[2].size() != 1)
                    throw UniversalStringException("Третий аргумент — одна цифра (0–9)");
                result = Natural(in[0]).subtractMultiplied(
                    Natural(in[1]), (size_t)(in[2][0] - '0')).toString();

            } else if (name == "Первая цифра деления") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = Natural(in[0]).getFirstDivisionDigit(Natural(in[1])).toString();

            } else if (name == "Неполное частное") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = (Natural(in[0]) / Natural(in[1])).toString();

            } else if (name == "Остаток от деления") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = (Natural(in[0]) % Natural(in[1])).toString();

            } else if (name == "НОД") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = Natural::gcd(Natural(in[0]), Natural(in[1])).toString();

            } else if (name == "НОК") {
                Validator::validateNaturalNumber(in[0]);
                Validator::validateNaturalNumber(in[1]);
                result = Natural::lcm(Natural(in[0]), Natural(in[1])).toString();

            // ── Целые ────────────────────────────────────────────────
            } else if (name == "Абсолютная величина") {
                Validator::validateIntegerNumber(in[0]);
                result = Integer(in[0]).abs().toString();

            } else if (name == "Положительность") {
                Validator::validateIntegerNumber(in[0]);
                result = std::string(1, Integer(in[0]).getSign() + '0');

            } else if (name == "Умножение на -1") {
                Validator::validateIntegerNumber(in[0]);
                result = (-Integer(in[0])).toString();

            } else if (name == "Натуральное в целое") {
                Validator::validateNaturalNumber(in[0]);
                result = Integer::fromNatural(Natural(in[0])).toString();

            } else if (name == "Целое в натуральное") {
                Validator::validateIntegerNumber(in[0]);
                result = Integer(in[0]).toNatural().toString();

            } else if (name == "Сложение целых чисел") {
                Validator::validateIntegerNumber(in[0]);
                Validator::validateIntegerNumber(in[1]);
                result = (Integer(in[0]) + Integer(in[1])).toString();

            } else if (name == "Вычитание целых чисел") {
                Validator::validateIntegerNumber(in[0]);
                Validator::validateIntegerNumber(in[1]);
                result = (Integer(in[0]) - Integer(in[1])).toString();

            } else if (name == "Умножение целых чисел") {
                Validator::validateIntegerNumber(in[0]);
                Validator::validateIntegerNumber(in[1]);
                result = (Integer(in[0]) * Integer(in[1])).toString();

            } else if (name == "Частное деления целых") {
                Validator::validateIntegerNumber(in[0]);
                Validator::validateIntegerNumber(in[1]);
                result = (Integer(in[0]) / Integer(in[1])).toString();

            } else if (name == "Остаток деления целых") {
                Validator::validateIntegerNumber(in[0]);
                Validator::validateIntegerNumber(in[1]);
                result = (Integer(in[0]) % Integer(in[1])).toString();

            // ── Дроби ────────────────────────────────────────────────
            } else if (name == "Сокращение дроби") {
                Validator::validateRationalNumber(in[0]);
                Rational r(in[0]); r.reduce();
                result = r.toString();

            } else if (name == "Проверка на целое") {
                Validator::validateRationalNumber(in[0]);
                result = Rational(in[0]).isInteger() ? "true" : "false";

            } else if (name == "Целое в дробное") {
                Validator::validateIntegerNumber(in[0]);
                result = Rational::fromInteger(Integer(in[0])).toString();

            } else if (name == "Дробное в целое") {
                Validator::validateRationalNumber(in[0]);
                result = Rational(in[0]).toInteger().toString();

            } else if (name == "Сложение дробей") {
                Validator::validateRationalNumber(in[0]);
                Validator::validateRationalNumber(in[1]);
                result = (Rational(in[0]) + Rational(in[1])).toString();

            } else if (name == "Вычитание дробей") {
                Validator::validateRationalNumber(in[0]);
                Validator::validateRationalNumber(in[1]);
                result = (Rational(in[0]) - Rational(in[1])).toString();

            } else if (name == "Умножение дробей") {
                Validator::validateRationalNumber(in[0]);
                Validator::validateRationalNumber(in[1]);
                result = (Rational(in[0]) * Rational(in[1])).toString();

            } else if (name == "Деление дробей") {
                Validator::validateRationalNumber(in[0]);
                Validator::validateRationalNumber(in[1]);
                result = (Rational(in[0]) / Rational(in[1])).toString();

            // ── Многочлены ───────────────────────────────────────────
            } else if (name == "Сложение многочленов") {
                result = (Polynom(Validator::validatePolynomial(in[0]))
                        + Polynom(Validator::validatePolynomial(in[1]))).toString();

            } else if (name == "Вычитание многочленов") {
                result = (Polynom(Validator::validatePolynomial(in[0]))
                        - Polynom(Validator::validatePolynomial(in[1]))).toString();

            } else if (name == "Умножение на дробь") {
                Validator::validateRationalNumber(in[1]);
                result = (Polynom(Validator::validatePolynomial(in[0]))
                        * Rational(in[1])).toString();

            } else if (name == "Умножение на x^k") {
                Validator::validateNaturalNumber(in[1]);
                if (in[1].size() > 20)
                    throw UniversalStringException("Показатель k слишком большой");
                result = Polynom(Validator::validatePolynomial(in[0]))
                            .multiplyByXPower(std::stoul(in[1])).toString();

            } else if (name == "Старший коэффициент") {
                result = Polynom(Validator::validatePolynomial(in[0]))
                            .getLeadingCoefficient().toString();

            } else if (name == "Степень многочлена") {
                result = std::to_string(
                    Polynom(Validator::validatePolynomial(in[0])).getDegree());

            } else if (name == "НОК и НОД") {
                result = Polynom(Validator::validatePolynomial(in[0])).factorOut().toString();

            } else if (name == "Умножение многочленов") {
                result = (Polynom(Validator::validatePolynomial(in[0]))
                        * Polynom(Validator::validatePolynomial(in[1]))).toString();

            } else if (name == "Частное деления") {
                result = (Polynom(Validator::validatePolynomial(in[0]))
                        / Polynom(Validator::validatePolynomial(in[1]))).toString();

            } else if (name == "Остаток деления") {
                result = (Polynom(Validator::validatePolynomial(in[0]))
                        % Polynom(Validator::validatePolynomial(in[1]))).toString();

            } else if (name == "НОД многочленов") {
                result = Polynom::gcd(
                    Polynom(Validator::validatePolynomial(in[0])),
                    Polynom(Validator::validatePolynomial(in[1]))).toString();

            } else if (name == "Производная") {
                result = Polynom(Validator::validatePolynomial(in[0])).derivative().toString();

            } else if (name == "Кратные в простые") {
                result = Polynom(Validator::validatePolynomial(in[0])).makeSquareFree().toString();
            }

            displayResult(resultField, result, mod.name);

        } catch (const UniversalStringException& e) {
            showError(e.what());
            resultField->clear();
        }
    });

    moduleLayout->addStretch();
}

// ─────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Калькулятор");
    window.resize(800, 600);

    // Фоновое изображение (опционально)
    QString bgStyle;
    if (QFile::exists("background.jpg"))
        bgStyle = "QWidget#mainWindow { background-image: url(background.jpg); background-position: center; }";

    window.setObjectName("mainWindow");
    window.setStyleSheet(bgStyle + Theme::dark);
    bool isDark = true;

    // ── Кнопки управления ───────────────────────────────────────────
    auto* themeBtn = new QPushButton("Светлая тема");
    themeBtn->setFixedSize(135, 35);
    auto* helpBtn = new QPushButton("Справка");
    helpBtn->setFixedSize(100, 35);

    QObject::connect(themeBtn, &QPushButton::clicked, [&]() {
        isDark = !isDark;
        window.setStyleSheet(bgStyle + (isDark ? Theme::dark : Theme::light));
        themeBtn->setText(isDark ? "Светлая тема" : "Тёмная тема");
    });
    QObject::connect(helpBtn, &QPushButton::clicked, [&]() { showHelp(isDark); });

    // ── Список модулей ───────────────────────────────────────────────
    const QVector<Module> modules = {
        // Натуральные
        {"Сравнение натуральных", "Натуральные и 0", 2, "Сравнение: 2 — первое больше, 0 — равны, 1 — первое меньше."},
        {"Проверка на 0",         "Натуральные и 0", 1, "1 если число ≠ 0, иначе 0."},
        {"Добавление 1",          "Натуральные и 0", 1, "Прибавить 1 к натуральному числу."},
        {"Сложение натуральных",  "Натуральные и 0", 2, "Сложение натуральных чисел."},
        {"Вычитание меньшего",    "Натуральные и 0", 2, "Вычитание меньшего из большего."},
        {"Умножение на цифру",    "Натуральные и 0", 2, "Умножение числа на цифру 0–9 (второй аргумент)."},
        {"Умножение на 10^k",     "Натуральные и 0", 2, "Умножение числа на 10^k (k — второй аргумент)."},
        {"Умножение натуральных", "Натуральные и 0", 2, "Умножение двух натуральных чисел."},
        {"Вычитание умноженного", "Натуральные и 0", 3, "a − b·c, где c — цифра (третий аргумент)."},
        {"Первая цифра деления",  "Натуральные и 0", 2, "Первая цифра частного при делении a на b·10^k."},
        {"Неполное частное",      "Натуральные и 0", 2, "Неполное частное от деления a на b."},
        {"Остаток от деления",    "Натуральные и 0", 2, "Остаток от деления a на b."},
        {"НОД",                   "Натуральные и 0", 2, "Наибольший общий делитель."},
        {"НОК",                   "Натуральные и 0", 2, "Наименьшее общее кратное."},
        // Целые
        {"Абсолютная величина",   "Целые", 1, "Модуль целого числа (результат — натуральное)."},
        {"Положительность",       "Целые", 1, "2 — положительное, 0 — ноль, 1 — отрицательное."},
        {"Умножение на -1",       "Целые", 1, "Смена знака целого числа."},
        {"Натуральное в целое",   "Целые", 1, "Преобразование натурального в целое."},
        {"Целое в натуральное",   "Целые", 1, "Преобразование неотрицательного целого в натуральное."},
        {"Сложение целых чисел",  "Целые", 2, "Сложение целых чисел."},
        {"Вычитание целых чисел", "Целые", 2, "Вычитание целых чисел."},
        {"Умножение целых чисел", "Целые", 2, "Умножение целых чисел."},
        {"Частное деления целых", "Целые", 2, "Частное от деления a на b (b ≠ 0)."},
        {"Остаток деления целых", "Целые", 2, "Остаток от деления a на b (b ≠ 0)."},
        // Дроби
        {"Сокращение дроби",  "Дроби", 1, "Ввод: «-14/3». Сокращение дроби."},
        {"Проверка на целое", "Дроби", 1, "Ввод: «-14/3». true если знаменатель = 1."},
        {"Целое в дробное",   "Дроби", 1, "Ввод: целое число. Результат: n/1."},
        {"Дробное в целое",   "Дроби", 1, "Ввод: «-14/3». Только если знаменатель = 1."},
        {"Сложение дробей",   "Дроби", 2, "Ввод: «-14/3». Сложение дробей."},
        {"Вычитание дробей",  "Дроби", 2, "Ввод: «-14/3». Вычитание дробей."},
        {"Умножение дробей",  "Дроби", 2, "Ввод: «-14/3». Умножение дробей."},
        {"Деление дробей",    "Дроби", 2, "Ввод: «-14/3». Деление дробей (делитель ≠ 0)."},
        // Многочлены
        {"Сложение многочленов",   "Многочлены", 2, "Ввод: «x^3+2/5x^2+3x+4». Сложение."},
        {"Вычитание многочленов",  "Многочлены", 2, "Ввод: «x^3+2/5x^2+3x+4». Вычитание."},
        {"Умножение на дробь",     "Многочлены", 2, "Многочлен × рациональное число."},
        {"Умножение на x^k",       "Многочлены", 2, "Второй аргумент — натуральное k или 0."},
        {"Старший коэффициент",    "Многочлены", 1, "Ввод: «x^3+2/5x^2+3x+4». Старший коэффициент."},
        {"Степень многочлена",     "Многочлены", 1, "Ввод: «x^3+2/5x^2+3x+4». Степень."},
        {"НОК и НОД",              "Многочлены", 1, "Вынесение НОК знаменателей и НОД числителей."},
        {"Умножение многочленов",  "Многочлены", 2, "Ввод: «x^3+2/5x^2+3x+4». Умножение."},
        {"Частное деления",        "Многочлены", 2, "Частное при делении с остатком."},
        {"Остаток деления",        "Многочлены", 2, "Остаток при делении с остатком."},
        {"НОД многочленов",        "Многочлены", 2, "НОД двух многочленов."},
        {"Производная",            "Многочлены", 1, "Ввод: «x^3+2/5x^2+3x+4». Производная."},
        {"Кратные в простые",      "Многочлены", 1, "Преобразование: кратные корни → простые."},
    };

    // ── Компоновка ───────────────────────────────────────────────────
    auto* mainLayout = new QHBoxLayout(&window);

    // Левая панель
    auto* leftPanel = new QWidget();
    leftPanel->setFixedWidth(260);
    auto* leftLayout = new QVBoxLayout(leftPanel);

    auto* controlRow = new QHBoxLayout();
    controlRow->addWidget(themeBtn);
    controlRow->addWidget(helpBtn);
    leftLayout->addLayout(controlRow);
    leftLayout->addSpacing(10);

    auto* tabs = new QTabWidget();
    tabs->setStyleSheet("QTabBar::scroller { width: 80px; }");
    leftLayout->addWidget(tabs);

    // Правая панель
    auto* rightArea   = new QWidget();
    auto* rightLayout = new QVBoxLayout(rightArea);

    auto* titleLabel = new QLabel("Выберите модуль");
    titleLabel->setAlignment(Qt::AlignCenter);
    QFont tf = titleLabel->font(); tf.setPointSize(16); tf.setBold(true);
    titleLabel->setFont(tf);
    rightLayout->addWidget(titleLabel);

    auto* moduleArea   = new QWidget();
    auto* moduleLayout = new QVBoxLayout(moduleArea);
    moduleLayout->setAlignment(Qt::AlignTop);
    rightLayout->addWidget(moduleArea);

    // Черновик
    rightLayout->addSpacing(10);
    rightLayout->addWidget(new QLabel("Черновик:"));
    auto* draftField = new QTextEdit();
    draftField->setPlaceholderText("Введите текст — он сохранится между модулями...");
    draftField->setLineWrapMode(QTextEdit::WidgetWidth);
    draftField->setAcceptRichText(false);
    draftField->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    draftField->setMinimumHeight(150);
    rightLayout->addWidget(draftField, 1);

    auto* draftRow   = new QHBoxLayout();
    auto* clearBtn   = new QPushButton("Очистить черновик");
    auto* copyBtn    = new QPushButton("Копировать результат в черновик");
    draftRow->addWidget(clearBtn);
    draftRow->addWidget(copyBtn);
    draftRow->addStretch();
    rightLayout->addLayout(draftRow);

    QObject::connect(clearBtn, &QPushButton::clicked, [=]() { draftField->clear(); });

    // ── Генерация вкладок ────────────────────────────────────────────
    QVector<QString> categories;
    for (const auto& m : modules)
        if (!categories.contains(m.category)) categories.append(m.category);

    for (const QString& cat : categories) {
        auto* tab       = new QWidget();
        auto* tabLayout = new QVBoxLayout(tab);

        auto* catLabel = new QLabel(cat);
        catLabel->setAlignment(Qt::AlignCenter);
        QFont cf = catLabel->font(); cf.setBold(true); catLabel->setFont(cf);
        tabLayout->addWidget(catLabel);

        for (const Module& m : modules) {
            if (m.category != cat) continue;
            auto* btn = new QPushButton(m.name);
            tabLayout->addWidget(btn);
            QObject::connect(btn, &QPushButton::clicked, [=]() {
                showModuleInterface(moduleLayout, titleLabel, draftField, copyBtn, m);
            });
        }
        tabLayout->addStretch();
        tabs->addTab(tab, cat);
    }

    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightArea);

    window.show();
    return app.exec();
}