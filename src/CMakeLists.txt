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
file(GLOB SLAM_SOURCE NerdSLAM/*.h NerdSLAM/*.cc
  NerdSLAM/core/*.h NerdSLAM/core/*.cc)

find_library(freenect2_LIBRARIES freenect2)
if (freenect2_LIBRARIES)
  message(STATUS "building kinect v2 support with ${freenect2_LIBRARIES}")
  find_package(OpenCL REQUIRED)
  file(GLOB KINECT_SOURCE NerdSLAM/kinect/v2/*.h NerdSLAM/kinect/v2/*.cc)
  include_directories(${OpenCL_INCLUDE_DIRS})
  link_libraries(${freenect2_LIBRARIES} ${OpenCL_LIBRARIES})

  # opengl viewer
  find_package(OpenGL)
  find_package(GLEW)
  find_library(GLFW_LIBRARIES glfw)
  if (GLFW_LIBRARIES)
    set(GLFW_FOUND TRUE)
  else ()
    set(GLFW_FOUND FALSE)
  endif ()
  if (OPENGL_FOUND AND GLFW_FOUND AND GLEW_FOUND)
    include_directories(${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
    link_libraries(${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLFW_LIBRARIES})

    file(GLOB KINECT_DISPLAY_SOURCE NerdSLAM/kinect/kinect_display_window.*
      NerdSLAM/kinect/kinect_frame.*)
  endif ()
else ()
  set(KINECT_SOURCE "")
endif ()

# build core library
add_library(NerdSLAM ${SLAM_SOURCE}
  ${KINECT_SOURCE} ${KINECT_DISPLAY_SOURCE}
  ${SLAM_PROTO_SOURCE} ${SLAM_PROTO_HEADERS})

if (OPENGL_FOUND AND GLFW_FOUND AND GLEW_FOUND)
  message(STATUS "building kinect viewer...")
  add_executable(kinect_display_2d NerdSLAM/kinect/kinect_display_2d.cc)
  target_link_libraries(kinect_display_2d NerdSLAM)
endif ()

# optionals
# find_library(OpenCV_CORE opencv_core)
# find_library(OpenCV_HIGHGUI opencv_highgui)
# find_library(OpenCV_IMGCODECS opencv_imgcodecs)
# find_library(OpenCV_VIDEO opencv_videostab)
# find_library(OpenCV_VIDEOIO opencv_videoio)
# if (OpenCV_CORE AND OpenCV_HIGHGUI AND OpenCV_VIDEOIO)
#   set(OpenCV_LIBRARIES ${OpenCV_CORE} ${OpenCV_HIGHGUI}
#     ${OpenCV_IMGCODECS} ${OpenCV_VIDEOIO} ${OpenCV_VIDEO})
# endif ()
# if (OpenCV_LIBRARIES AND freenect2_LIBRARIES)
#   message(STATUS "building kinect display and sampler: ${OpenCV_LIBRARIES}")
#   include_directories(${OpenCV_INCLUDE_DIRS})
#   link_libraries(${OpenCV_LIBRARIES})

#   file(GLOB KINECT_2D_DISPLAY_SOURCE
#     NerdSLAM/kinect/kinect_2d_display*)
#   add_executable(kinect_2d_display ${KINECT_2D_DISPLAY_SOURCE})
#   target_link_libraries(kinect_2d_display NerdSLAM)
# endif ()
