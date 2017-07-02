#ifndef KINECT_FRAME_H
#define KINECT_FRAME_H

#include "NerdSLAM/core/opengl_object.h"

namespace nerd {
namespace slam {

class KinectFrame : public gl::OpenGLObject {
 public:
  KinectFrame();
  ~KinectFrame();
  bool Init(const std::string& vert_shader,
            const std::string& frag_shader) override;
  void UpdateFrame(const Frame& frame);
  void Render() override;

 private:
  GLuint texture_;
};

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: KINECT_FRAME_H */
