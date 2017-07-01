#include "NerdSLAM/kinect/kinect_reader_v2.h"
#include <chrono>
#include <cstdlib>
#include <string>
#include <thread>
#include <vector>

namespace nerd {
namespace slam {

KinectReaderV2::KinectReaderV2()
    : device_(nullptr),
      pipeline_(nullptr),
      listener_(nullptr),
      registration_(nullptr),
      undistorted_(nullptr),
      registered_(nullptr) {}

KinectReaderV2::KinectReaderV2(const KinectConfigV2& config)
    : config_(config),
      device_(nullptr),
      pipeline_(nullptr),
      listener_(nullptr),
      registration_(nullptr),
      undistorted_(nullptr),
      registered_(nullptr) {}

KinectReaderV2::~KinectReaderV2() {
  StopDevice();
  if (undistorted_) delete undistorted_;
  if (registered_) delete registered_;
  if (registration_) delete registration_;
  if (listener_) delete listener_;
  if (device_) {
    device_->close();
    delete device_;
  }
}

bool KinectReaderV2::FindDevice() {
  if (device_) {
    return true;
  }

  if (freenect2_.enumerateDevices() == 0) {
    return false;
  }

  std::string serial = freenect2_.getDefaultDeviceSerialNumber();
  switch (config_.packet_pipeline_type()) {
    case KinectConfigV2::OPENGL:
      pipeline_ = new libfreenect2::OpenGLPacketPipeline();
      break;
    case KinectConfigV2::OPENCL:
      pipeline_ = new libfreenect2::OpenCLPacketPipeline();
      break;
    case KinectConfigV2::CPU:
      pipeline_ = new libfreenect2::CpuPacketPipeline();
      break;
    case KinectConfigV2::DUMP:
      pipeline_ = new libfreenect2::DumpPacketPipeline();
      break;
  }

  device_ = freenect2_.openDevice(serial, pipeline_);

  int types = 0;
  if (config_.rgb()) {
    types |= libfreenect2::Frame::Color;
  }
  if (config_.depth()) {
    types |= libfreenect2::Frame::Depth;
  }
  if (config_.ir()) {
    types |= libfreenect2::Frame::Ir;
  }
  if (config_.registration()) {
    types |= libfreenect2::Frame::Color;
    types |= libfreenect2::Frame::Depth;
    registration_ = new libfreenect2::Registration(
        device_->getIrCameraParams(), device_->getColorCameraParams());
  }

  listener_ = new libfreenect2::SyncMultiFrameListener(types);
  device_->setColorFrameListener(listener_);
  device_->setIrAndDepthFrameListener(listener_);
  return true;
}

void KinectReaderV2::StartDevice() {
  if (config_.rgb() && config_.depth()) {
    running_ = device_->start();
  } else {
    running_ = device_->startStreams(config_.rgb(), config_.depth());
  }
  Loop();
}

void KinectReaderV2::PauseDevice() {
  if (device_) {
    device_->stop();
  }
}

void KinectReaderV2::StopDevice() {
  PauseDevice();
  running_ = false;
}

void KinectReaderV2::Loop() {
  while (running_) {
    if (!listener_->waitForNewFrame(frame_map_, config_.timeout())) {
      continue;
    }

    // process frames
    if (config_.rgb()) {
      libfreenect2::Frame* rgb = frame_map_[libfreenect2::Frame::Color];
      ProcessRGBFrame(rgb);
    }
    if (config_.ir()) {
      libfreenect2::Frame* ir = frame_map_[libfreenect2::Frame::Ir];
      ProcessIRFrame(ir);
    }
    if (config_.depth()) {
      libfreenect2::Frame* depth = frame_map_[libfreenect2::Frame::Depth];
      ProcessDepthFrame(depth);
    }
    if (config_.registration()) {
      Register(frame_map_[libfreenect2::Frame::Color],
               frame_map_[libfreenect2::Frame::Depth]);
    }

    if (frame_handler_) {
      frame_handler_(frame_);
    }
    listener_->release(frame_map_);

    if (config_.timeout() > 0) {
      std::this_thread::sleep_for(
          std::chrono::milliseconds(config_.timeout()));
    }
  }
}

void KinectReaderV2::ProcessRGBFrame(const libfreenect2::Frame* rgb) {
  assert((rgb->format == libfreenect2::Frame::Format::BGRX ||
          rgb->format == libfreenect2::Frame::Format::RGBX));
  int w = rgb->width;
  int h = rgb->height;

  // create rgb frame
  Frame* color_frame = new Frame;
  color_frame->set_width(w);
  color_frame->set_height(h);
  color_frame->set_data(rgb->data, w * h * 4);
  frame_.set_allocated_color_frame(color_frame);
}

void KinectReaderV2::ProcessIRFrame(const libfreenect2::Frame* ir) {
  assert(ir->format == libfreenect2::Frame::Format::Float);
  int w = ir->width;
  int h = ir->height;

  Frame* ir_frame = new Frame;
  ir_frame->set_width(w);
  ir_frame->set_height(h);
  ir_frame->set_data(ir->data, w * h * sizeof(float));
  frame_.set_allocated_ir_frame(ir_frame);
}

void KinectReaderV2::ProcessDepthFrame(const libfreenect2::Frame* depth) {
  assert(depth->format == libfreenect2::Frame::Format::Float);
  int w = depth->width;
  int h = depth->height;

  Frame* depth_frame = new Frame;
  depth_frame->set_width(w);
  depth_frame->set_height(h);
  depth_frame->set_data(depth->data, w * h * sizeof(float));
  frame_.set_allocated_depth_frame(depth_frame);
}

void KinectReaderV2::Register(const libfreenect2::Frame* rgb,
                              const libfreenect2::Frame* depth) {
  int w = depth->width;
  int h = depth->height;
  if (!undistorted_) {
    undistorted_ = new libfreenect2::Frame(w, h, 4);
  }
  if (!registered_) {
    registered_ = new libfreenect2::Frame(w, h, 4);
  }
  registration_->apply(rgb, depth, undistorted_, registered_);

  Frame* undistored_frame = new Frame;
  undistored_frame->set_width(w);
  undistored_frame->set_height(h);
  undistored_frame->set_data(undistorted_->data, w * h * 4);
  frame_.set_allocated_undistored_frame(undistored_frame);

  Frame* registered_frame = new Frame;
  registered_frame->set_width(w);
  registered_frame->set_height(h);
  registered_frame->set_data(registered_->data, w * h * 4);
  frame_.set_allocated_registered_frame(registered_frame);
}

} /* end of slam namespace */
} /* end of nerd namespace */
