
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall -pthread")

# add other dir sources
AUX_SOURCE_DIRECTORY(../nnero NNERO_SRC)

find_package(SDL2 REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# generate exec file with sources
ADD_EXECUTABLE(game1 move_cure.cpp ${NNERO_SRC})

# add needed libs 
TARGET_LINK_LIBRARIES(game1 ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES})
