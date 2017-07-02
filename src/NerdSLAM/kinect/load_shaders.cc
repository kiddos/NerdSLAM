#include "NerdSLAM/kinect/load_shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace nerd {
namespace slam {

bool ReadShaderFile(const std::string& fpath, std::string& content) {
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

bool LoadShaders(const std::string& vertex_shader_fpath,
                 const std::string& fragment_shader_fpath,
                 GLuint& program) {
  // Create the shaders
  GLuint vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Shader code
  std::string vcode;
  ReadShaderFile(vertex_shader_fpath, vcode);
  std::string fcode;
  ReadShaderFile(fragment_shader_fpath, fcode);

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
  program = glCreateProgram();
  glAttachShader(program, vertex_shader_id);
  glAttachShader(program, fragment_shader_id);
  glLinkProgram(program);

  // Check the program
  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
  if (!result && info_log_length > 0) {
    std::string error_msg(info_log_length + 1, ' ');
    glGetProgramInfoLog(program, info_log_length, nullptr, &error_msg[0]);
    std::cerr << error_msg << '\n';
    return false;
  }

  glDetachShader(program, vertex_shader_id);
  glDetachShader(program, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);
  return true;
}

} /* end of slam namespace */
} /* end of nerd namespace */
