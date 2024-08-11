cmake_minimum_required(VERSION 3.14)
project(proxy-server)

set(CMAKE_CXX_STANDARD 17)

set(SERVER_SOURCES
        src/proxy-server/proxy-server/proxy_server.cc
        src/proxy-server/logger/logger.cc
        src/proxy-server/logger/logger.h
        src/proxy-server/constants.h
        src/proxy-server/db_shell/db_shell.cc
        src/proxy-server/db_shell/db_shell.h
)

add_executable(proxy-server src/proxy-server/main.cc ${SERVER_SOURCES}

)
add_executable(client
        src/clients/client/main.cc)
add_executable(client-manual
        src/clients/client-manual/main.cc)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

find_package(SQLite3 REQUIRED)
include_directories(${SQLite3_INCLUDE_DIRS})
target_link_libraries(proxy-server SQLite::SQLite3)


enable_testing()

# Add test executable
add_executable(tests test/test_main.cc
        ${SERVER_SOURCES}
        test/logger_test.cc

)
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(tests gtest gtest_main ${SQLite3_LIBRARIES})
add_test(NAME proxy_server_tests COMMAND tests)
