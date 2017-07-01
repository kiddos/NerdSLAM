#include "NerdSLAM/kinect/kinect_display_window.h"
#include <boost/bind.hpp>
#include <iostream>

namespace nerd {
namespace slam {

KinectDisplayWindow::KinectDisplayWindow(const OpenGLWindowConfig& config)
    : config_(config), window_(nullptr) {}

KinectDisplayWindow::~KinectDisplayWindow() {
  if (window_) glfwDestroyWindow(window_);
  glfwTerminate();
}

bool KinectDisplayWindow::Init() {
  if (!glfwInit()) {
    return false;
  }

  glfwSetErrorCallback([](int error, const char* desc) {
    std::cerr << "[\x1B[31mERROR\x1B[0m] "
              << "error code: " << error << " | " << desc << '\n';
  });

  // set API version
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, config_.major_version());
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, config_.minor_version());
  glfwWindowHint(GLFW_SAMPLES, 16);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  if (config_.debug()) {
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
  }
  // glfw window
  window_ = glfwCreateWindow(config_.width(), config_.height(),
                             config_.title().c_str(), nullptr, nullptr);
  if (!window_) {
    return false;
  }
  // set current context
  glfwMakeContextCurrent(window_);

  glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetKeyCallback(window_,
                     [](GLFWwindow* window, int key, int, int action, int) {
                       if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
                         glfwSetWindowShouldClose(window, GL_TRUE);
                       }
                     });

  GLenum error = glewInit();
  if (error != GLEW_OK) {
    return false;
  }
  return true;
}

void KinectDisplayWindow::MainLoop() {
  // get time
  double start_time = glfwGetTime();
  long frames = 0;
  // main loop
  while (!glfwWindowShouldClose(window_)) {
    // get view port
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    glViewport(0, 0, width, height);

    // clear buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    glfwSwapBuffers(window_);
    glfwPollEvents();
    double passed  = glfwGetTime() - start_time;
    ++frames;
    std::cout << "\rfps: " << frames / passed << std::flush;
  }
}

} /* end of slam namespace */
} /* end of nerd namespace */
