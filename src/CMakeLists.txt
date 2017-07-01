# required dependencies
find_package(Protobuf REQUIRED)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(PCL REQUIRED COMPONENTS common)
find_package(Eigen3 REQUIRED)
find_library(LUA_LIBRARY lua5.2)
if (LUA_LIBRARY)
  message(STATUS "Lua5.2 found.")
  set(LUA_FOUND TRUE)
else ()
  message(ERROR "Lua5.2 not found")
endif ()

# include protobuf file
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PCL_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

link_libraries(${PROTOBUF_LIBRARIES})
link_libraries(${Boost_LIBRARIES} ${PCL_LIBRARIES})
link_directories(${PCL_LIBRARY_DIRS})

PROTOBUF_GENERATE_CPP(SLAM_PROTO_SOURCE SLAM_PROTO_HEADERS
  NerdSLAM/proto/nerdslam.proto)
file(GLOB_RECURSE SLAM_SOURCE NerdSLAM/*.h NerdSLAM/*.cc)

find_library(freenect2_LIBRARIES freenect2)
if (freenect2_LIBRARIES)
  message(STATUS "found freenect2: ${freenect2_LIBRARIES}")
  find_package(OpenCL REQUIRED)
  file(GLOB KINECT_SOURCE NerdSLAM/kinect/kinect_reader_v2.*
    NerdSLAM/kinect/kinect_v2_console_logger.*
    NerdSLAM/kinect/kinect_v2_file_logger.*)
  include_directories(${OpenCL_INCLUDE_DIRS})
  link_libraries(${freenect2_LIBRARIES} ${OpenCL_LIBRARIES})
else ()
  set(KINECT_SOURCE "")
endif ()

add_library(NerdSLAM ${SLAM_SOURCE} ${KINECT_SOURCE}
  ${SLAM_PROTO_SOURCE} ${SLAM_PROTO_HEADERS})
