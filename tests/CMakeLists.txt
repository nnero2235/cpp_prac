#add compile flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -pthread")

# add other dir sources
AUX_SOURCE_DIRECTORY(../nnero NNERO_SRC)

# generate exec file with sources
ADD_EXECUTABLE(main_test nnero_unittest.cpp ${NNERO_SRC})

# add needed libs 
TARGET_LINK_LIBRARIES(main_test gtest.a gtest_main.a)
