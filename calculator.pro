QT += core gui widgets

CONFIG += c++11

# Главный исходный файл
SOURCES += main.cpp \
    lib/src/algstructures/Integer.cpp \
    lib/src/algstructures/Natural.cpp \
    lib/src/algstructures/Polynom.cpp \
    lib/src/algstructures/Rational.cpp  \
    lib/src/Validator/Validator.cpp \
    lib/src/Validator/Utils/Lexer.cpp \
    lib/src/Validator/Utils/Parser.cpp \
    lib/src/Validator/Utils/Token.cpp

# Заголовочные файлы
HEADERS += \
    lib/src/algstructures/Integer.h \
    lib/src/algstructures/Natural.h \
    lib/src/algstructures/Polynom.h \
    lib/src/algstructures/Rational.h  \
    lib/src/Validator/Validator.h \
    lib/src/Exceptions/UniversalStringException.h \
    lib/src/Validator/Utils/Lexer.h \
    lib/src/Validator/Utils/Parser.h \
    lib/src/Validator/Utils/Token.h \
    lib/src/Validator/Utils/Monom.h