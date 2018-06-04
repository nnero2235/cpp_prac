#min version
cmake_minimum_required (VERSION 2.8)

#project info
project (network)

#include path
include_directories(/home/nnero/project/cplusplus/cpp_prac)

#execute file
add_executable(epoll_server epoll.cpp)
add_executable(client socket_client.cpp)
