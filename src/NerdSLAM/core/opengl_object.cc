#include "NerdSLAM/core/opengl_object.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace nerd {
namespace slam {
namespace gl {

OpenGLObject::OpenGLObject() : vao_(0), vbo_(0), program_(0) {}

OpenGLObject::~OpenGLObject() {
  glDeleteBuffers(1, &vbo_);
  glDeleteVertexArrays(1, &vao_);
  glDeleteProgram(program_);
}

bool OpenGLObject::Init(const std::string& vert_shader,
                        const std::string& frag_shader) {
  // Create the shaders
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Shader code
  std::string vcode, fcode;
  if (!ReadShaderFile(vert_shader, vcode) ||
      !ReadShaderFile(frag_shader, fcode)) {
    return false;
  }

  // Compile Vertex Shader
  const char* const vc = vcode.c_str();
  glShaderSource(vertex_shader_id, 1, &vc, nullptr);
  glCompileShader(vertex_shader_id);

  // Check Vertex Shader
  GLint result = GL_FALSE;
  int info_log_length = 0;
  glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (!result && info_log_length > 0) {
    std::string error_msg(info_log_length + 1, ' ');
    glGetShaderInfoLog(vertex_shader_id, info_log_length, nullptr,
                       &error_msg[0]);
    std::cerr << error_msg << '\n';
    return false;
  }

  // Compile Fragment Shader
  const char* const fc = fcode.c_str();
  glShaderSource(fragment_shader_id, 1, &fc, nullptr);
  glCompileShader(fragment_shader_id);

  // Check Fragment Shader
  glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (!result && info_log_length > 0) {
    std::string error_msg(info_log_length + 1, ' ');
    glGetShaderInfoLog(fragment_shader_id, info_log_length, nullptr,
                       &error_msg[0]);
    std::cerr << error_msg << '\n';
    return false;
  }

  // Link the program
  program_ = glCreateProgram();
  glAttachShader(program_, vertex_shader_id);
  glAttachShader(program_, fragment_shader_id);
  glLinkProgram(program_);

  // Check the program
  glGetProgramiv(program_, GL_LINK_STATUS, &result);
  glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &info_log_length);
  if (!result && info_log_length > 0) {
    std::string error_msg(info_log_length + 1, ' ');
    glGetProgramInfoLog(program_, info_log_length, nullptr, &error_msg[0]);
    std::cerr << error_msg << '\n';
    return false;
  }

  glDetachShader(program_, vertex_shader_id);
  glDetachShader(program_, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  // create vertex array object
  glGenVertexArrays(1, &vao_);
  // create vertex buffer object
  glGenBuffers(1, &vbo_);
  return true;
}

bool OpenGLObject::ReadShaderFile(const std::string& fpath,
                                  std::string& content) {
  std::string vertex_shader_code;
  std::ifstream vf(fpath, std::ios::in);
  if (vf.is_open()) {
    std::stringstream ss;
    ss << vf.rdbuf();
    content = ss.str();
    return true;
  } else {
    return false;
  }
}

} /* end of gl namespace */
} /* end of slam namespace */
} /* end of nerd namespace */
