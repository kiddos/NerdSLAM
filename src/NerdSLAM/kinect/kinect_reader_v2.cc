#include "NerdSLAM/kinect/kinect_reader_v2.h"
#include <string>
#include <vector>

namespace nerd {
namespace slam {

Kinect2Reader::Kinect2Reader()
    : device_(nullptr),
      pipeline_(nullptr),
      listener_(nullptr) {}

Kinect2Reader::Kinect2Reader(const Kinect2Config& config)
    : config_(config),
      device_(nullptr),
      pipeline_(nullptr),
      listener_(nullptr) {}

Kinect2Reader::~Kinect2Reader() {
  StopDevice();
  if (device_) {
    device_->close();
    delete device_;
  }
  if (listener_) delete listener_;
}

bool Kinect2Reader::FindDevice() {
  if (freenect2_.enumerateDevices() == 0) {
    return false;
  }

  std::string serial = freenect2_.getDefaultDeviceSerialNumber();
  switch (config_.packet_pipeline_type()) {
    case Kinect2Config::OPENGL:
      pipeline_ = new libfreenect2::OpenGLPacketPipeline();
      break;
    case Kinect2Config::OPENCL:
      pipeline_ = new libfreenect2::OpenCLPacketPipeline();
      break;
    case Kinect2Config::CPU:
      pipeline_ = new libfreenect2::CpuPacketPipeline();
      break;
    case Kinect2Config::DUMP:
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

void Kinect2Reader::StartDevice() {
  if (config_.rgb() && config_.depth()) {
    running_ = !device_->start();
  } else {
    running_ = !device_->startStreams(config_.rgb(), config_.depth());
  }
  Loop();
}

void Kinect2Reader::Loop() {
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

void Kinect2Reader::ProcessRGBFrame(const libfreenect2::Frame* rgb) {
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

void Kinect2Reader::ProcessIRFrame(const libfreenect2::Frame* ir) {
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

void Kinect2Reader::ProcessDepthFrame(const libfreenect2::Frame* depth) {
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

void Kinect2Reader::PauseDevice() {
  if (device_) {
    device_->stop();
  }
}

void Kinect2Reader::StopDevice() {
  PauseDevice();
  running_ = false;
}

} /* end of slam namespace */
} /* end of nerd namespace */
