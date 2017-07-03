#include "NerdSLAM/kinect/v1/kinect_reader_v1.h"

namespace nerd {
namespace slam {
namespace v1 {

KinectReaderV1::KinectReaderV1(const KinectConfigV1& config)
    : config_(config), device_(nullptr) {}

KinectReaderV1::~KinectReaderV1() {
  if (device_) delete device_;
}

bool KinectReaderV1::FindDevice() {
  device_ = &freenect_.createDevice<KinectDevice>(0);
  if (!device_) {
    return false;
  }
  return true;
}

void KinectReaderV1::StartDevice() {
  if (device_) {
    device_->startVideo();
    device_->startDepth();
  }
}

void KinectReaderV1::PauseDevice() {
  if (device_) {
    device_->stopVideo();
    device_->stopDepth();
  }
}

void KinectReaderV1::StopDevice() {
  PauseDevice();
}

} /* end of v1 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */
