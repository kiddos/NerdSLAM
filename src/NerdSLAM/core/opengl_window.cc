#include "NerdSLAM/core/opengl_window.h"
#include <iostream>

namespace nerd {
namespace slam {
namespace gl {

OpenGLWindow::OpenGLWindow(const OpenGLWindowConfig& config)
    : config_(config), window_(nullptr) {}

OpenGLWindow::~OpenGLWindow() {
  if (window_) glfwDestroyWindow(window_);
  glfwTerminate();
}

bool OpenGLWindow::Init() {
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

  glewExperimental = GL_TRUE;
  GLenum error = glewInit();
  if (error != GLEW_OK) {
    return false;
  }

  return false;
}

} /* end of gl namespace */
} /* end of slam namespace */
} /* end of nerd namespace */
