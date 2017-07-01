#include "NerdSLAM/kinect/kinect_reader_v2.h"
#include <string>
#include <vector>

namespace nerd {
namespace slam {

KinectReaderV2::KinectReaderV2()
    : device_(nullptr),
      pipeline_(nullptr),
      listener_(nullptr) {}

KinectReaderV2::KinectReaderV2(const KinectConfigV2& config)
    : config_(config),
      device_(nullptr),
      pipeline_(nullptr),
      listener_(nullptr) {}

KinectReaderV2::~KinectReaderV2() {
  StopDevice();
  if (device_) {
    device_->close();
    delete device_;
  }
  if (listener_) delete listener_;
}

bool KinectReaderV2::FindDevice() {
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

  listener_ = new libfreenect2::SyncMultiFrameListener(types);
  device_->setColorFrameListener(listener_);
  device_->setIrAndDepthFrameListener(listener_);
  return true;
}

void KinectReaderV2::StartDevice() {
  if (config_.rgb() && config_.depth()) {
    running_ = !device_->start();
  } else {
    running_ = !device_->startStreams(config_.rgb(), config_.depth());
  }
  Loop();
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

    if (frame_handler_) {
      frame_handler_(frame_);
    }
  }
}

void KinectReaderV2::ProcessRGBFrame(const libfreenect2::Frame* rgb) {
  assert((rgb->format == libfreenect2::Frame::Format::BGRX ||
          rgb->format == libfreenect2::Frame::Format::RGBX));
  int w = rgb->width;
  int h = rgb->height;

  // create rgb frame
  RGBFrame* rgb_frame = new RGBFrame;
  rgb_frame->set_width(w);
  rgb_frame->set_height(h);
  std::vector<char> buffer(w * h * 3);
  if (rgb->format == libfreenect2::Frame::Format::BGRX) {
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        buffer[(i * w + j) * 3] = rgb->data[(i * w + j) * 4 + 2];
        buffer[(i * w + j) * 3 + 1] = rgb->data[(i * w + j) * 4 + 1];
        buffer[(i * w + j) * 3 + 2] = rgb->data[(i * w + j) * 4];
      }
    }
  } else if (rgb->format == libfreenect2::Frame::Format::RGBX) {
    for (int i = 0; i < h; ++i) {
      for (int j = 0; j < w; ++j) {
        buffer[(i * w + j) * 3] = rgb->data[(i * w + j) * 4];
        buffer[(i * w + j) * 3 + 1] = rgb->data[(i * w + j) * 4 + 1];
        buffer[(i * w + j) * 3 + 2] = rgb->data[(i * w + j) * 4 + 2];
      }
    }
  }
  rgb_frame->set_data(&buffer[0], buffer.size());
  frame_.set_allocated_rgb_frame(rgb_frame);
}

void KinectReaderV2::ProcessIRFrame(const libfreenect2::Frame* ir) {
  assert(ir->format == libfreenect2::Frame::Format::Float);
  int w = ir->width;
  int h = ir->height;

  IRFrame* ir_frame = new IRFrame;
  ir_frame->set_width(w);
  ir_frame->set_height(h);
  std::vector<float> buffer(w * h);
  const float* data = reinterpret_cast<float*>(ir->data);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      buffer[i * w + j] = data[i * w + j];
    }
  }
  ir_frame->set_data(&buffer[0], buffer.size() * sizeof(float));
  frame_.set_allocated_ir_frame(ir_frame);
}

void KinectReaderV2::ProcessDepthFrame(const libfreenect2::Frame* depth) {
  assert(depth->format == libfreenect2::Frame::Format::Float);
  int w = depth->width;
  int h = depth->height;

  DepthFrame* depth_frame = new DepthFrame;
  depth_frame->set_width(w);
  depth_frame->set_height(h);
  std::vector<float> buffer(w * h);
  const float* data = reinterpret_cast<float*>(depth->data);
  for (int i = 0; i < h; ++i) {
    for (int j = 0; j < w; ++j) {
      buffer[i * w + j] = data[i * w + j];
    }
  }
  depth_frame->set_data(&buffer[0], buffer.size() * sizeof(float));
  frame_.set_allocated_depth_frame(depth_frame);
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

} /* end of slam namespace */
} /* end of nerd namespace */
