#add compile flags
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lXrender -lXcursor -lXfixes -lXext -lXft -lfontconfig -lXinerama -lpthread -ldl -lm -lX11 -g -std=c++11 -Wall -pthread")

# add other dir sources
AUX_SOURCE_DIRECTORY(../nnero NNERO_SRC)

# generate exec file with sources
ADD_EXECUTABLE(game fltk_1.cpp ${NNERO_SRC})

INCLUDE_DIRECTORIES("/usr/local/include")

# add needed libs 
TARGET_LINK_LIBRARIES(game libfltk.a)
