#add compile flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -pthread")

# add other dir sources
AUX_SOURCE_DIRECTORY(../nnero NNERO_SRC)
AUX_SOURCE_DIRECTORY(../algorithm ALGORITHM_SRC)
AUX_SOURCE_DIRECTORY(../network NET_SRC)

# generate exec file with sources
#ADD_EXECUTABLE(main_test algorithm_unittest.cpp ${ALGORITHM_SRC})
#ADD_EXECUTABLE(main_test nnero_unittest.cpp ${NNERO_SRC})
ADD_EXECUTABLE(main_test net_test.cpp ${NET_SRC} ${NNERO_SRC})


# add needed libs 
TARGET_LINK_LIBRARIES(main_test gtest.a gtest_main.a)
