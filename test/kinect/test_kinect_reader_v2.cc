#include <gtest/gtest.h>
#include <thread>
#include "NerdSLAM/kinect/kinect_reader_v2.h"

using nerd::slam::Kinect2Reader;
using nerd::slam::Kinect2Config;
using nerd::slam::Frame;
using nerd::slam::RGBFrame;
using nerd::slam::IRFrame;
using nerd::slam::DepthFrame;

TEST(TestKinectReaderV2, FindDevice) {
  Kinect2Config config;
  Kinect2Reader reader(config);
  EXPECT_TRUE(reader.FindDevice());
}

TEST(TestKinectReaderV2, CaptureFrame) {
  Kinect2Config config;
  config.set_rgb(true);
  config.set_ir(true);
  config.set_depth(true);
  Kinect2Reader reader(config);

  int frame_count = 0;
  reader.set_frame_hander([&reader, &frame_count](const Frame& f) -> void {
    // test rgb frame
    RGBFrame rgb = f.rgb_frame();
    EXPECT_EQ(rgb.data().size(), rgb.width() * rgb.height() * 3);
    // test ir frame
    IRFrame ir = f.ir_frame();
    EXPECT_EQ(ir.data().size(), ir.width() * ir.height());
    // test depth frame
    DepthFrame depth = f.depth_frame();
    EXPECT_EQ(depth.data().size(), depth.width() * depth.height());

    frame_count += 1;
    if (frame_count >= 100) {
      reader.StopDevice();
    }
  });
  if (reader.FindDevice()) {
    reader.StartDevice();
  }
}

TEST(TestKinectReaderV2, AsyncCaptureFrame) {
}
