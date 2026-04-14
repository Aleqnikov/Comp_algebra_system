cmake_minimum_required(VERSION 3.14)
project(cas)

set(CMAKE_CXX_STANDARD 20)

# 1. Собираем все .cpp файлы библиотеки
file(GLOB_RECURSE LIB_SOURCES "lib/src/**/*.cpp")

# Удаляем внутренний main.cpp из списка файлов библиотеки (ты это уже делал, молодец)
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_SOURCE_DIR}/lib/src/main.cpp")

# 2. Создаем основную библиотеку БЕЗ корневого main.cpp!
add_library(cas ${LIB_SOURCES})

# 3. Настраиваем пути поиска заголовочных файлов
target_include_directories(cas PUBLIC "${CMAKE_SOURCE_DIR}/lib/src")

# 4. Создаем исполняемый файл для консоли (тот, что внутри lib)
add_executable(cas_run "lib/src/main.cpp")
target_link_libraries(cas_run PRIVATE cas)

# 4.1 Создаем отдельный исполняемый файл для твоего GUI (тот, что в корне)
add_executable(cas_gui "main.cpp")
target_link_libraries(cas_gui PRIVATE cas)

# 5. Включаем поддержку тестов в CMake и подтягиваем директорию
enable_testing()
add_subdirectory(lib/tests)