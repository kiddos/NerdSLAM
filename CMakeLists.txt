cmake_minimum_required(VERSION 3.0)

project(NerdSLAM)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra ")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -g -O0 ")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 ")
set(CMAKE_CXX_STANDARD 11)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif ()

enable_testing()
add_subdirectory(src)
add_subdirectory(test)

# copy shaders
file(COPY shaders DESTINATION ${CMAKE_BINARY_DIR})
