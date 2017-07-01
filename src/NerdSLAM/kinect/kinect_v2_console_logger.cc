#include "NerdSLAM/kinect/kinect_v2_console_logger.h"
#include <iomanip>
#include <iostream>
#include <sstream>

namespace nerd {
namespace slam {

KinectV2ConsoleLogger::KinectV2ConsoleLogger(
    libfreenect2::Logger::Level level) {
  level_ = level;
  start_ = std::chrono::high_resolution_clock::now();
}

void KinectV2ConsoleLogger::log(libfreenect2::Logger::Level level,
                                const std::string& message) {
  if (level > level_ || level == libfreenect2::Logger::Level::None) return;

  switch (level) {
    case libfreenect2::Logger::Level::None:
      std::cout << "[None] [" << GetTimeString() << "]" << message
                << std::endl;
      break;
    case libfreenect2::Logger::Level::Debug:
      std::cout << "[Debug] [" << GetTimeString() << "]" << message
                << std::endl;
      break;
    case libfreenect2::Logger::Level::Info:
      std::cout << "[Info] [" << GetTimeString() << "]" << message
                << std::endl;
      break;
    case libfreenect2::Logger::Level::Warning:
      std::cout << "[\x1B[33mWarning\x1B[0m] [" << GetTimeString() << "]"
                << message << std::endl;
      break;
    case libfreenect2::Logger::Level::Error:
      std::cout << "[\x1B[31mError\x1B[0m] [" << GetTimeString() << "]"
                << message << std::endl;
      break;
  }
}

std::string KinectV2ConsoleLogger::GetTimeString() {
  auto current = std::chrono::high_resolution_clock::now();
  auto passed = current - start_;
  std::stringstream ss;
  ss << std::setw(2)
     << std::chrono::duration_cast<std::chrono::hours>(passed).count() << ":"
     << std::setw(2)
     << std::chrono::duration_cast<std::chrono::minutes>(passed).count() << ":"
     << std::setw(2)
     << std::chrono::duration_cast<std::chrono::seconds>(passed).count() << "."
     << std::setw(3)
     << std::chrono::duration_cast<std::chrono::milliseconds>(passed).count();
  return ss.str();
}

} /* end of slam namespace */
} /* end of nerd namespace */
