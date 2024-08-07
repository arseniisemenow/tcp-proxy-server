cmake_minimum_required(VERSION 3.14)
project(proxy-server)

set(CMAKE_CXX_STANDARD 17)

add_executable(proxy-server src/proxy-server/main.cc src/proxy-server/proxy-server/proxy_server.cc
        src/proxy-server/logger/logger.cc
        src/proxy-server/logger/logger.h)
add_executable(client
        src/client/main.cc)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})

enable_testing()

# Add test executable
add_executable(tests test/test_main.cc
        src/proxy-server/proxy-server/proxy_server.cc
        src/proxy-server/logger/logger.cc
        test/logger_test.cc

)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tests gtest gtest_main ${SQLite3_LIBRARIES})
add_test(NAME proxy_server_tests COMMAND tests)
