#include <chrono>
#include <sstream>
#include "NerdSLAM/kinect/v2/kinect_file_logger.h"

namespace nerd {
namespace slam {
namespace v2 {

KinectV2FileLogger::KinectV2FileLogger(const std::string& filepath,
                                       libfreenect2::Logger::Level level) {
  if (filepath.length() == 0) {
    std::stringstream ss;
    ss << "/tmp/NerdSLAM_kinect_"
       << std::chrono::high_resolution_clock::now().time_since_epoch().count()
       << ".log";
    logfile_.open(ss.str(), std::ios::out);
  }
  level_ = level;
}

KinectV2FileLogger::~KinectV2FileLogger() {
  if (logfile_.is_open()) {
    logfile_.close();
  }
}

void KinectV2FileLogger::log(libfreenect2::Logger::Level level,
                             const std::string& message) {
  if (level > level_ || level == libfreenect2::Logger::Level::None) return;
  if (ok()) {
    logfile_ << "[" << libfreenect2::Logger::level2str(level) << "] "
             << message << std::endl;
  }
}

} /* end of v2 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */
