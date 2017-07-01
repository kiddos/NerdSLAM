# add link directory from build
link_directories(${CMAKE_BINARY_DIR}/src)

# google test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/src)
link_libraries(${GTEST_BOTH_LIBRARIES} pthread)

if (TARGET NerdSLAM)
  link_libraries(NerdSLAM)
  if (freenect2_LIBRARIES)
    message(STATUS "build test for kinect reader v2...")
    add_executable(test_kinect_reader_v2 kinect/test_kinect_reader_v2.cc)
  endif ()
endif ()

# if (TARGET icalslam)
#   set(TEST_LIBS ${GTEST_BOTH_LIBRARIES} pthread icalslam)

#   # add_executable(test_target test_src.cpp)
#   # target_link_libraries(test_target ${TEST_LIBS})
#   # add_test(Test1 test_target)
# endif ()

# find_library(BENCHMARK_LIBRARIES benchmark)
# if (BENCHMARK_LIBRARIES)
#   message(STATUS "Using Google Benchmark ${BENCHMARK_LIBRARIES}")
# endif ()
