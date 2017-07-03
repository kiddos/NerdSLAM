#include "NerdSLAM/kinect/v1/kinect_device.h"

namespace nerd {
namespace slam {
namespace v1 {

KinectDevice::KinectDevice(freenect_context* context, int index)
    : Freenect::FreenectDevice(context, index), gamma_(2048) {}

void KinectDevice::VideoCallback(void* rgb, uint32_t timestamp) {
  rgb_mutex_.lock();
  const uint8_t* rgb_data = static_cast<uint8_t*>(rgb);
  int rgb_size = getVideoBufferSize();
  if (rgb_.size() == 0) {
    rgb_ = std::vector<uint8_t>(rgb_size);
  }
  std::copy(rgb_data, rgb_data + rgb_size, rgb_.begin());
  rgb_mutex_.unlock();
}

void KinectDevice::DepthCallback(void* depth, uint32_t timestamp) {
  depth_mutex_.lock();
  const uint16_t* depth_value = static_cast<uint16_t*>(depth);
  int depth_size = getDepthBufferSize();
  int pix_size = depth_size / 3;
  if (depth_.size() == 0) {
    depth_ = std::vector<uint8_t>(depth_size);
  }
  for (int i = 0; i < pix_size; ++i) {
    int pval = gamma_[depth_value[i]];
    int lb = pval & 0xff;
    switch (pval >> 8) {
      case 0:
        depth_[3 * i + 0] = 255;
        depth_[3 * i + 1] = 255 - lb;
        depth_[3 * i + 2] = 255 - lb;
        break;
      case 1:
        depth_[3 * i + 0] = 255;
        depth_[3 * i + 1] = lb;
        depth_[3 * i + 2] = 0;
        break;
      case 2:
        depth_[3 * i + 0] = 255 - lb;
        depth_[3 * i + 1] = 255;
        depth_[3 * i + 2] = 0;
        break;
      case 3:
        depth_[3 * i + 0] = 0;
        depth_[3 * i + 1] = 255;
        depth_[3 * i + 2] = lb;
        break;
      case 4:
        depth_[3 * i + 0] = 0;
        depth_[3 * i + 1] = 255 - lb;
        depth_[3 * i + 2] = 255;
        break;
      case 5:
        depth_[3 * i + 0] = 0;
        depth_[3 * i + 1] = 0;
        depth_[3 * i + 2] = 255 - lb;
        break;
      default:
        depth_[3 * i + 0] = 0;
        depth_[3 * i + 1] = 0;
        depth_[3 * i + 2] = 0;
        break;
    }
  }
  depth_mutex_.unlock();
}

void KinectDevice::GetRGB(std::vector<uint8_t>& rgb) {
  if (rgb_.size() > 0) {
    rgb_mutex_.lock();
    if (rgb.size() < rgb_.size()) {
      rgb = std::vector<uint8_t>(rgb_.size());
    }
    std::copy(rgb_.begin(), rgb_.end(), rgb.begin());
    rgb_mutex_.unlock();
  }
}

void KinectDevice::GetDepth(std::vector<uint8_t>& depth) {
  if (depth_.size() > 0) {
    depth_mutex_.lock();
    if (depth.size() < depth_.size()) {
      depth = std::vector<uint8_t>(depth_.size());
      std::copy(depth.begin(), depth.end(), depth_.begin());
    }
    depth_mutex_.unlock();
  }
}

} /* end of v1 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */
