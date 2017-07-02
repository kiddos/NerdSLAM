#ifndef LOAD_SHADERS_H
#define LOAD_SHADERS_H

#include <GL/glew.h>
#include <string>

namespace nerd {
namespace slam {

bool LoadShaders(const std::string& vert_shader,
                 const std::string& frag_shader, GLuint& program);

} /* end of slam namespace */
} /* end of nerd namespace */

#endif /* end of include guard: LOAD_SHADERS_H */
