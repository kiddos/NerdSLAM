#include "NerdSLAM/kinect/kinect_frame.h"
#include <vector>
#include <iostream>

namespace nerd {
namespace slam {

KinectFrame::KinectFrame() : texture_(0) {}

KinectFrame::~KinectFrame() {
  glDeleteTextures(1, &texture_);
}

bool KinectFrame::Init(const std::string& vert_shader,
                       const std::string& frag_shader) {
  OpenGLObject::Init(vert_shader, frag_shader);
  // create texture object
  glGenTextures(1, &texture_);
  return true;
}

void KinectFrame::UpdateFrame(const Frame& frame) {
  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  struct Vertex {
    GLfloat x, y, u, v;
  };

  float w = frame.width();
  float h = frame.height();
  Vertex bl = {-1.0f, -1.0f, 0.0f, h};
  Vertex tl = {-1.0f, 1.0f, 0.0f, 0};
  Vertex br = {1.0f, -1.0f, w, h};
  Vertex tr = {1.0f, 1.0f, w, 0};
  Vertex vertexes[] = {bl, tl, tr, tr, br, bl};
  // push vertices to OpenGL pipeline
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes),
               &vertexes[0], GL_STATIC_DRAW);

  // push texture to pipeline
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_RECTANGLE, texture_);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_RECTANGLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_RECTANGLE, 0, GL_RGB, w, h, 0, GL_BGRA,
               GL_UNSIGNED_BYTE, frame.data().c_str());
}

void KinectFrame::Render() {
  glUseProgram(program_);

  glBindVertexArray(vao_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid*)0);

  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat),
                        (GLvoid*)(2 * sizeof(float)));
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

} /* end of slam namespace */
} /* end of nerd namespace */
