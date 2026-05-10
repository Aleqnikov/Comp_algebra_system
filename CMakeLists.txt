cmake_minimum_required(VERSION 3.14)
project(cas)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE LIB_SOURCES "lib/src/**/*.cpp")
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/lib/src/main.cpp")

add_library(cas ${LIB_SOURCES})
target_include_directories(cas PUBLIC "${CMAKE_SOURCE_DIR}/lib/src")

add_executable(cas_gui "main.cpp")
target_link_libraries(cas_gui PRIVATE cas)

add_subdirectory(formula_system)

enable_testing()
add_subdirectory(lib/tests)
add_subdirectory(formula_system/tests)
