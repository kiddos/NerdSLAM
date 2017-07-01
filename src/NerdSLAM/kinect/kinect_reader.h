#ifndef KINECT_READER_H
#define KINECT_READER_H

#include <boost/function.hpp>
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {

class KinectReader {
 public:
  KinectReader() : running_(false) {};
  ~KinectReader() {}
  virtual bool FindDevice() = 0;
  virtual void StartDevice() = 0;
  virtual void PauseDevice() = 0;
  virtual void StopDevice() = 0;

  void set_frame_hander(boost::function<void(FrameMap)> handler) {
    frame_handler_ = std::move(handler);
  }
  bool is_running() { return running_; }

 protected:
  bool running_;
  boost::function<void(const FrameMap&)> frame_handler_;
  FrameMap frame_;
};

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_READER_H */
