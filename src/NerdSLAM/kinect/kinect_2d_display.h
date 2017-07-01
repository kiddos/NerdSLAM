#ifndef KINECT_2D_DISPLAY_H
#define KINECT_2D_DISPLAY_H

#include "NerdSLAM/kinect/kinect_reader.h"
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {

class Kinect2DDisplay {
 public:
  Kinect2DDisplay();
  ~Kinect2DDisplay();
  void ShowFrame(const FrameMap& frames);
  void Run();

 private:
  KinectReader* reader_;
  std::string color_window_, depth_window_, ir_window_;
  std::string undistorted_window_, registered_window_;
};

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_2D_DISPLAY_H */
