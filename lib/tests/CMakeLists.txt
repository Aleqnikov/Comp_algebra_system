include(FetchContent)

FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

# Собираем все тестовые файлы автоматически из подпапок
file(GLOB_RECURSE TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/**/*_test.cpp"
)

add_executable(run_tests ${TEST_SOURCES})

target_link_libraries(run_tests
        gtest_main
        cas
)

target_include_directories(run_tests PRIVATE ${CMAKE_SOURCE_DIR}/lib/src)