#ifndef KINECT_DEVICE_H
#define KINECT_DEVICE_H

#include <libfreenect.hpp>
#include <vector>
#include <mutex>

namespace nerd {
namespace slam {
namespace v1 {

class KinectDevice : public Freenect::FreenectDevice {
 public:
  KinectDevice(freenect_context* context, int index);
  void VideoCallback(void* rgb, uint32_t timestamp) override;
  void DepthCallback(void* depth, uint32_t timestamp) override;
  void GetRGB(std::vector<uint8_t>& rgb);
  void GetDepth(std::vector<uint8_t>& depth);

 private:
  std::vector<uint8_t> rgb_, depth_;
  std::vector<uint16_t> gamma_;
  std::mutex rgb_mutex_, depth_mutex_;
};

} /* end of v1 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_DEVICE_H */
