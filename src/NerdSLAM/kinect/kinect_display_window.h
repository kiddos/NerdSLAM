#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "NerdSLAM/kinect/kinect_reader.h"
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {

class KinectDisplayWindow {
 public:
  KinectDisplayWindow(const OpenGLWindowConfig& config);
  ~KinectDisplayWindow();
  bool Init();
  void Render();
  void MainLoop();
  void KeyEvent(unsigned char key, int x, int y);

 private:
  OpenGLWindowConfig config_;
  GLFWwindow* window_;
  KinectReader* reader_;
};

} /* end of slam namespace */
} /* end of nerd namespace */
