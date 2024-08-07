cmake_minimum_required(VERSION 3.14)
project(TCPProxyServer)

set(CMAKE_CXX_STANDARD 17)

# Add the main executable
add_executable(TCPProxyServer src/main.cc src/proxy_server.cc)

# Fetch Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)


find_package(PkgConfig REQUIRED)
pkg_check_modules(PQXX REQUIRED libpqxx)
include_directories(${PQXX_INCLUDE_DIRS})
link_directories(${PQXX_LIBRARY_DIRS})

# Enable testing
enable_testing()

# Add test executable
add_executable(tests
        src/proxy_server.cc
        test/test_proxy_server.cc)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tests gtest gtest_main ${PQXX_LIBRARIES})
add_test(NAME proxy_server_tests COMMAND tests)

target_link_libraries(tests gtest gtest_main)
