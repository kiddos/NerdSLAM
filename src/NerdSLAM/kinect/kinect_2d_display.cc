#include "NerdSLAM/kinect/kinect_2d_display.h"
#include <boost/bind.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <cstdlib>
#include "NerdSLAM/kinect/kinect_reader_v2.h"

namespace nerd {
namespace slam {

Kinect2DDisplay::Kinect2DDisplay()
    : reader_(nullptr),
      color_window_("Color Image"),
      depth_window_("Depth Image"),
      ir_window_("IR Image"),
      undistorted_window_("Undistorted"),
      registered_window_("Registered") {
  KinectConfigV2 config;
  config.set_rgb(true);
  config.set_depth(true);
  config.set_ir(true);
  config.set_registration(true);

  reader_ = new KinectReaderV2(config);
  if (reader_->FindDevice()) {
    reader_->set_frame_hander(
        boost::bind(&Kinect2DDisplay::ShowFrame, this, _1));
  }
}

Kinect2DDisplay::~Kinect2DDisplay() {
  if (reader_) delete reader_;
}

void Kinect2DDisplay::ShowFrame(const FrameMap& frames) {
  // color frame
  Frame color_frame = frames.color_frame();
  cv::Mat color(color_frame.height(), color_frame.width(), CV_8UC4);
  std::memcpy(color.data, color_frame.data().c_str(),
              color_frame.height() * color_frame.width() * 4);
  cv::imshow(color_window_, color);

  Frame depth_frame = frames.depth_frame();
  cv::Mat depth(depth_frame.height(), depth_frame.width(), CV_32FC1);
  std::memcpy(depth.data, depth_frame.data().c_str(),
              depth_frame.height() * depth_frame.width() * sizeof(float));
  cv::imshow(depth_window_, depth);

  char key = cv::waitKey(10);
  if (key == 'q' || key == 'Q' || key == 27) {
    reader_->StopDevice();
  }
}

void Kinect2DDisplay::Run() { reader_->StartDevice(); }

} /* end of slam namespace */
} /* end of nerd namespace */
