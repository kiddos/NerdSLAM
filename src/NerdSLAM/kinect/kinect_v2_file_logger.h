#ifndef KINECT_V2_FILE_LOGGER_H
#define KINECT_V2_FILE_LOGGER_H

#include <libfreenect2/logger.h>
#include <fstream>
#include <string>

namespace nerd {
namespace slam {

class KinectV2FileLogger : public libfreenect2::Logger {
 public:
  KinectV2FileLogger(const std::string& filepath,
                     libfreenect2::Logger::Level level);

  bool ok() { return logfile_.is_open() && logfile_.good(); }

  virtual void log(libfreenect2::Logger::Level level,
                   const std::string& message) override;

 private:
  std::ofstream logfile_;
};

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_V2_FILE_LOGGER_H */
