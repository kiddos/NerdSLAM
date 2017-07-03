# required dependencies
find_package(Protobuf REQUIRED)
find_package(PCL REQUIRED COMPONENTS common)
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)
find_package(Eigen3 REQUIRED)
find_library(LUA_LIBRARY lua5.2)
if (LUA_LIBRARY)
  message(STATUS "Lua5.2 found.")
  set(LUA_FOUND TRUE)
else ()
  message(FATAL_ERROR "Lua5.2 not found")
endif ()
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_library(GLFW_LIBRARIES glfw)
if (GLFW_LIBRARIES)
  set(GLFW_FOUND TRUE)
else ()
  set(GLFW_FOUND FALSE)
endif ()
if (NOT GLFW_FOUND)
  message(FATAL_ERROR "glfw not found")
endif ()

# include protobuf file
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PCL_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

link_libraries(${PROTOBUF_LIBRARIES})
link_libraries(${Boost_LIBRARIES} ${PCL_LIBRARIES})
link_directories(${PCL_LIBRARY_DIRS})
link_libraries(${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

PROTOBUF_GENERATE_CPP(SLAM_PROTO_SOURCE SLAM_PROTO_HEADERS
  NerdSLAM/proto/nerdslam.proto)
file(GLOB SLAM_SOURCE NerdSLAM/*.h NerdSLAM/*.cc
  NerdSLAM/core/*.h NerdSLAM/core/*.cc)

find_library(freenect_LIBRARIES freenect)
if (freenect_LIBRARIES)
  message(STATUS "building kinect v1 support with ${freenect_LIBRARIES}")
  find_path(LIBUSB_INCLUDE_DIRS libusb.h /usr/include/libusb-1.0)
  if (NOT LIBUSB_INCLUDE_DIRS)
    message(FATAL_ERROR "libusb 1.0 not found")
  endif ()
  include_directories(${LIBUSB_INCLUDE_DIRS})
  link_libraries(${freenect_LIBRARIES})

  file(GLOB KINECT_V1_SOURCE
    NerdSLAM/kinect/v1/*.h
    NerdSLAM/kinect/v1/*.cc)

  message(STATUS "building kinect v1 viewer...")
  add_executable(kinect_display_v1 NerdSLAM/kinect/v1/kinect_display.cc)
endif ()

find_library(freenect2_LIBRARIES freenect2)
if (freenect2_LIBRARIES)
  message(STATUS "building kinect v2 support with ${freenect2_LIBRARIES}")
  find_package(OpenCL REQUIRED)
  include_directories(${OpenCL_INCLUDE_DIRS})
  link_libraries(${freenect2_LIBRARIES} ${OpenCL_LIBRARIES})

  file(GLOB KINECT_V2_SOURCE
    NerdSLAM/kinect/v2/*.h
    NerdSLAM/kinect/v2/*.cc
    NerdSLAM/kinect/kinect_display_window.*
    NerdSLAM/kinect/kinect_frame.*)

  message(STATUS "building kinect v2 viewer...")
  add_executable(kinect_display_v2 NerdSLAM/kinect/v2/kinect_display.cc)
else ()
  set(KINECT_SOURCE "")
endif ()

# build core library
add_library(NerdSLAM
  ${SLAM_SOURCE}
  ${KINECT_V1_SOURCE}
  ${KINECT_V2_SOURCE}
  ${SLAM_PROTO_SOURCE}
  ${SLAM_PROTO_HEADERS})

target_link_libraries(kinect_display_v1 NerdSLAM)
target_link_libraries(kinect_display_v2 NerdSLAM)
