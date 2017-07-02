#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {
namespace gl {

class OpenGLWindow {
 public:
  OpenGLWindow(const OpenGLWindowConfig& config);
  virtual ~OpenGLWindow();
  virtual bool Init();
  virtual void MainLoop() = 0;

 protected:
  OpenGLWindowConfig config_;
  GLFWwindow* window_;
  double start_time_;
  long frames_;
};

} /* end of gl namespace */
} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: OPENGL_WINDOW_H */
