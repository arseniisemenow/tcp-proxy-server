cmake_minimum_required(VERSION 3.14)
project(TCPProxyServer)

set(CMAKE_CXX_STANDARD 17)
set(BOOST_ASIO_ENABLE_HANDLER_TRACKING ON)

find_package(Boost 1.70 REQUIRED COMPONENTS system)

add_executable(TCPProxyServer src/main.cc src/proxy_server.cc)
target_include_directories(TCPProxyServer PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(TCPProxyServer PRIVATE ${Boost_LIBRARIES})
