#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "NerdSLAM/core/opengl_window.h"
#include "NerdSLAM/kinect/kinect_frame.h"
#include "NerdSLAM/kinect/kinect_reader.h"
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {

class KinectDisplayWindow : public gl::OpenGLWindow {
 public:
  KinectDisplayWindow(const OpenGLWindowConfig& config);
  ~KinectDisplayWindow();
  bool Init() override;
  void Render(const FrameMap& frames);
  void MainLoop() override;

 private:
  KinectReader* reader_;
  KinectFrame rgb_frame_, depth_frame_;
};

} /* end of slam namespace */
} /* end of nerd namespace */
