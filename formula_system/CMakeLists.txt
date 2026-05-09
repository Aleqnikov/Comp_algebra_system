cmake_minimum_required(VERSION 3.14)

set(SOURCES
        formula_tree.cpp
        tokenizer.cpp
        module01_to_prefix.cpp
        module02_from_postfix.cpp
        module03_diff.cpp
        module04_tree_to_postfix.cpp
        module05_postfix_to_str.cpp
        module06_tree_to_str.cpp
        module07_assoc.cpp
        module08_priority.cpp
        module09_simplify_tree.cpp
        module10_simplify_postfix.cpp
        module11_like_terms_postfix.cpp
        module12_like_terms_tree.cpp
        module13_commutative.cpp
)

add_executable(formula_demo main_formula.cpp ${SOURCES})

# Берём cas (Natural/Integer/Rational) из родительского проекта
target_link_libraries(formula_demo PRIVATE cas)

# Заголовки lib/src доступны через cas, но на всякий случай явно:
target_include_directories(formula_demo PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/lib/src
)