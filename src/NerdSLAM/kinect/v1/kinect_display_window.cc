#include "NerdSLAM/kinect/v1/kinect_display_window.h"
#include "NerdSLAM/kinect/v1/kinect_reader_v1.h"
#include <boost/bind.hpp>
#include <iostream>

namespace nerd {
namespace slam {
namespace v1 {

KinectDisplayWindow::KinectDisplayWindow(const OpenGLWindowConfig& config)
    : OpenGLWindow(config) {}

KinectDisplayWindow::~KinectDisplayWindow() {
  if (reader_) delete reader_;
}

bool KinectDisplayWindow::Init() {
  if (!OpenGLWindow::Init()) {
    return false;
  }

  // setup frames
  if (!rgb_frame_.Init("shaders/kinect_frame_2d.vert",
                       "shaders/kinect_frame_rgb.frag")) {
    return false;
  }

  KinectConfigV1 config;
  reader_ = new v1::KinectReaderV1(config);
  if (!reader_->FindDevice()) {
    return false;
  }

  glfwSetKeyCallback(window_, [](GLFWwindow* window, int key, int scancode,
                                 int action, int mods) {
    if (action == GLFW_PRESS) {
      if (key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
      }
    }
  });

  start_time_ = glfwGetTime();
  frames_ = 0;
  reader_->set_frame_hander(
      boost::bind(&KinectDisplayWindow::Render, this, _1));
  return false;
}

void KinectDisplayWindow::Render(const FrameMap& frames) {
  // clear buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  int w, h;
  glfwGetFramebufferSize(window_, &w, &h);
  glViewport(0, 0, w, h);

  rgb_frame_.UpdateFrame(frames.color_frame());
  rgb_frame_.Render();

  glfwSwapBuffers(window_);
  glfwPollEvents();

  double passed = glfwGetTime() - start_time_;
  ++frames_;
  std::cout << "\rfps: " << frames_ / passed << std::flush;

  if (glfwWindowShouldClose(window_)) {
    reader_->StopDevice();
  }
}

void KinectDisplayWindow::MainLoop() {
  if (reader_) {
    reader_->StartDevice();
  }
}

} /* end of v1 namespace */
} /* end of slam namespace */
} /* end of nerd namespace */
