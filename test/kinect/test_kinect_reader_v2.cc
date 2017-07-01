#include <gtest/gtest.h>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>
#include "NerdSLAM/kinect/kinect_reader_v2.h"

using nerd::slam::KinectReaderV2;
using nerd::slam::KinectConfigV2;
using nerd::slam::FrameMap;
using nerd::slam::Frame;

class TestKinectReaderV2 : public ::testing::Test {
 public:
  TestKinectReaderV2() : reader_(nullptr) {}

  virtual ~TestKinectReaderV2() {
    if (reader_) {
      delete reader_;
    }
  }

  virtual void SetUp() override {
    config_.set_rgb(true);
    config_.set_ir(true);
    config_.set_depth(true);
    config_.set_registration(true);
    reader_ = new KinectReaderV2(config_);
  }

  virtual void TearDown() override {}

  void TestFindDevice() { EXPECT_TRUE(reader_->FindDevice()); }

  void TestCaptureFrame() {
    std::vector<FrameMap> frames;
    constexpr int max_frame_count = 100;
    int frame_count = 0;
    reader_->set_frame_hander([this, &frame_count,
                               &frames](const FrameMap& frame) {
      // test rgb frame
      Frame color = frame.color_frame();
      EXPECT_EQ(color.data().size(), color.width() * color.height() * 4);
      // test ir frame
      Frame ir = frame.ir_frame();
      EXPECT_EQ(ir.data().size(), ir.width() * ir.height() * sizeof(float));
      // test depth frame
      Frame depth = frame.depth_frame();
      EXPECT_EQ(depth.data().size(),
                depth.width() * depth.height() * sizeof(float));

      Frame undistorted = frame.undistored_frame();
      EXPECT_EQ(undistorted.data().size(), depth.width() * depth.height() * 4);

      Frame registered = frame.registered_frame();
      EXPECT_EQ(registered.data().size(), depth.width() * depth.height() * 4);

      frames.push_back(frame);
      frame_count += 1;
      if (frame_count >= max_frame_count) {
        reader_->StopDevice();
      }
    });
    if (reader_->FindDevice()) {
      reader_->StartDevice();
    }
    EXPECT_EQ(frames.size(), max_frame_count);
  }

  void TestCaptureAsync() {
    std::vector<FrameMap> frames;
    constexpr int max_frame_count = 100;
    int frame_count = 0;
    reader_->set_frame_hander([this, &frame_count,
                               &frames](const FrameMap& frame) {
      // test rgb frame
      Frame color = frame.color_frame();
      EXPECT_EQ(color.data().size(), color.width() * color.height() * 4);
      // test ir frame
      Frame ir = frame.ir_frame();
      EXPECT_EQ(ir.data().size(), ir.width() * ir.height() * sizeof(float));
      // test depth frame
      Frame depth = frame.depth_frame();
      EXPECT_EQ(depth.data().size(),
                depth.width() * depth.height() * sizeof(float));

      Frame undistorted = frame.undistored_frame();
      EXPECT_EQ(undistorted.data().size(), depth.width() * depth.height() * 4);

      Frame registered = frame.registered_frame();
      EXPECT_EQ(registered.data().size(), depth.width() * depth.height() * 4);

      frames.push_back(frame);
      frame_count += 1;
      if (frame_count >= max_frame_count) {
        reader_->StopDevice();
      }
    });

    std::thread capture_task([this]() {
      if (reader_->FindDevice()) {
        reader_->StartDevice();
      }
    });

    do {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    } while (reader_->is_running());

    capture_task.join();
    EXPECT_EQ(frames.size(), max_frame_count);
  }

 private:
  KinectConfigV2 config_;
  KinectReaderV2* reader_;
};

TEST_F(TestKinectReaderV2, FindDevice) { TestFindDevice(); }

TEST_F(TestKinectReaderV2, CaptureFrame) { TestCaptureFrame(); }

TEST_F(TestKinectReaderV2, AsyncCaptureFrame) { TestCaptureAsync(); }
