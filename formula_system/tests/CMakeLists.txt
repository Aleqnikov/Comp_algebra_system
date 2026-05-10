include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/**/*_test.cpp"
)

set(FORMULA_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/../formula_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../tokenizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module01_to_prefix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module02_from_postfix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module03_diff.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module04_tree_to_postfix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module05_postfix_to_str.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module06_tree_to_str.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module07_assoc.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module08_priority.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module09_simplify_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module10_simplify_postfix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module11_like_terms_postfix.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module12_like_terms_tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../module13_commutative.cpp
)

add_executable(run_formula_tests ${TEST_SOURCES} ${FORMULA_SOURCES})

target_link_libraries(run_formula_tests
    gtest_main
    cas
)

target_include_directories(run_formula_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}/lib/src
)
