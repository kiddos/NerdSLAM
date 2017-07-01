#ifndef KINECT_V2_CONSOLE_LOGGER_H
#define KINECT_V2_CONSOLE_LOGGER_H

#include <libfreenect2/logger.h>
#include <chrono>

namespace nerd {
namespace slam {
namespace v2 {

class KinectV2ConsoleLogger : public libfreenect2::Logger {
 public:
  KinectV2ConsoleLogger(libfreenect2::Logger::Level level);
  virtual ~KinectV2ConsoleLogger() {}

  virtual void log(libfreenect2::Logger::Level level,
                   const std::string& message) override;

 private:
  std::chrono::high_resolution_clock::time_point start_;
  std::string GetTimeString();
};

} /* end of v2 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_V2_CONSOLE_LOGGER_H */
