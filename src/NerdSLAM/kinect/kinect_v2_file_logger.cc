#include "NerdSLAM/kinect/kinect_v2_file_logger.h"
#include <chrono>
#include <sstream>

namespace nerd {
namespace slam {

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

void KinectV2FileLogger::log(libfreenect2::Logger::Level level,
                             const std::string& message) {
  if (ok()) {
    logfile_ << "[" << libfreenect2::Logger::level2str(level) << "] "
             << message << std::endl;
  }
}

} /* end of slam namespace */
} /* end of nerd namespace */
