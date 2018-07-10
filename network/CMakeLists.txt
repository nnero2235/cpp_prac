#add compile flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -pthread")

# add other dir sources
AUX_SOURCE_DIRECTORY(../nnero NNERO_SRC)
AUX_SOURCE_DIRECTORY(../network NET_SRC)

# generate exec file with sources
ADD_EXECUTABLE(server server.cpp poller.cpp event_loop.cpp tcp_connection.cpp tcp_server.cpp tcp_socket.cpp ${NNERO_SRC})
ADD_EXECUTABLE(client client.cpp tcp_connection.cpp tcp_socket.cpp ${NNERO_SRC})


# add needed libs 
#TARGET_LINK_LIBRARIES(main_test gtest.a gtest_main.a)
