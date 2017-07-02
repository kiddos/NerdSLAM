#ifndef OPENGL_OBJECT_H
#define OPENGL_OBJECT_H

#include <GL/glew.h>
#include <string>
#include "nerdslam.pb.h"

namespace nerd {
namespace slam {
namespace gl {

class OpenGLObject {
 public:
  OpenGLObject();
  virtual ~OpenGLObject();
  virtual bool Init(const std::string& vert_shader,
                    const std::string& frag_shader);
  virtual void Render() = 0;

 protected:
  bool ReadShaderFile(const std::string& fpath, std::string& content);
  GLuint vao_, vbo_;
  GLuint program_;
};

} /* end of gl namespace */
} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: OPENGL_OBJECT_H */
