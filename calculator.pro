QT += core gui widgets

CONFIG += c++20

INCLUDEPATH += lib/src

SOURCES += main.cpp \
    lib/src/algstructures/Integer.cpp \
    lib/src/algstructures/Natural.cpp \
    lib/src/algstructures/Polynom.cpp \
    lib/src/algstructures/Rational.cpp \
    lib/src/Validator/Validator.cpp \
    lib/src/Validator/Utils/Lexer.cpp \
    lib/src/Validator/Utils/Parser.cpp \
    lib/src/Validator/Utils/Token.cpp \
    formula_system/formula_tree.cpp \
    formula_system/tokenizer.cpp \
    formula_system/module01_to_prefix.cpp \
    formula_system/module02_from_postfix.cpp \
    formula_system/module03_diff.cpp \
    formula_system/module04_tree_to_postfix.cpp \
    formula_system/module05_postfix_to_str.cpp \
    formula_system/module06_tree_to_str.cpp \
    formula_system/module07_assoc.cpp \
    formula_system/module08_priority.cpp \
    formula_system/module09_simplify_tree.cpp \
    formula_system/module10_simplify_postfix.cpp \
    formula_system/module11_like_terms_postfix.cpp \
    formula_system/module12_like_terms_tree.cpp \
    formula_system/module13_commutative.cpp

HEADERS += \
    lib/src/algstructures/Integer.h \
    lib/src/algstructures/Natural.h \
    lib/src/algstructures/Polynom.h \
    lib/src/algstructures/Rational.h \
    lib/src/Validator/Validator.h \
    lib/src/Exceptions/UniversalStringException.h \
    lib/src/Validator/Utils/Lexer.h \
    lib/src/Validator/Utils/Parser.h \
    lib/src/Validator/Utils/Token.h \
    lib/src/Validator/Utils/Monom.h \
    formula_system/formula_tree.h \
    formula_system/tokenizer.h \
    formula_system/rational_utils.h \
    formula_system/module01_to_prefix.h \
    formula_system/module02_from_postfix.h \
    formula_system/module03_diff.h \
    formula_system/module04_tree_to_postfix.h \
    formula_system/module05_postfix_to_str.h \
    formula_system/module06_tree_to_str.h \
    formula_system/module07_assoc.h \
    formula_system/module08_priority.h \
    formula_system/module09_simplify_tree.h \
    formula_system/module10_simplify_postfix.h \
    formula_system/module11_like_terms_postfix.h \
    formula_system/module12_like_terms_tree.h \
    formula_system/module13_commutative.h