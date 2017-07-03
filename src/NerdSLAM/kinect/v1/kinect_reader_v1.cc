#include "NerdSLAM/kinect/v1/kinect_reader_v1.h"
#include <chrono>
#include <thread>

namespace nerd {
namespace slam {
namespace v1 {

KinectReaderV1::KinectReaderV1(const KinectConfigV1& config)
    : config_(config), device_(nullptr), running_(false) {}

KinectReaderV1::~KinectReaderV1() {
  // if (device_) delete device_;
}

bool KinectReaderV1::FindDevice() {
  device_ = &freenect_.createDevice<KinectDevice>(0);
  if (!device_) {
    return false;
  }
  return true;
}

void KinectReaderV1::Run() {
  StartDevice();
  running_ = true;
  size_t data_size = FRAME_WIDTH * FRAME_HEIGHT * FRAME_CHANNEL;
  while (running_) {
    if (!pause_) {
      if (config_.rgb()) {
        device_->GetRGB(rgb_);
        assert(rgb_.size() == data_size);
        Frame* rgb = new Frame;
        rgb->set_width(FRAME_WIDTH);
        rgb->set_height(FRAME_HEIGHT);
        rgb->set_data(reinterpret_cast<const char*>(&rgb_[0]), data_size);
        rgb->set_type(Frame::RGB);
        frames_.set_allocated_color_frame(rgb);
      }
      if (config_.depth()) {
        device_->GetDepth(depth_);
        assert(rgb_.size() == FRAME_WIDTH * FRAME_HEIGHT * FRAME_CHANNEL);
        Frame* depth = new Frame;
        depth->set_width(FRAME_WIDTH);
        depth->set_height(FRAME_HEIGHT);
        depth->set_data(reinterpret_cast<const char*>(&depth_[0]), data_size);
        depth->set_type(Frame::RGB);
        frames_.set_allocated_depth_frame(depth);
      }
    }

    if (frame_handler_) {
      frame_handler_(frames_);
    }
    if (config_.timeout() > 0) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(config_.timeout()));
    }
  }
}

void KinectReaderV1::StartDevice() {
  if (device_) {
    if (config_.rgb()) {
      device_->startVideo();
    }
    if (config_.depth()) {
      device_->startDepth();
    }
  }
  pause_ = false;
}

void KinectReaderV1::PauseDevice() {
  pause_ = true;
  if (device_) {
    if (config_.rgb()) {
      device_->stopVideo();
    }
    if (config_.depth()) {
      device_->stopDepth();
    }
  }
}

void KinectReaderV1::StopDevice() {
  PauseDevice();
  running_ = false;
}

} /* end of v1 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */
