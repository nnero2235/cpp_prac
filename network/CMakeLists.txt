#min version
cmake_minimum_required (VERSION 2.8)

#project info
project (network)

#execute file
add_executable(epoll_server epoll.cpp)
