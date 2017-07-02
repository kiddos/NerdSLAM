#version 450 core

uniform sampler2DRect sampler;

in VertexData {
  vec2 texture_coord;
} texture_in;

out vec3 color;

void main(void) {
  color = texture(sampler, texture_in.texture_coord).rgb;
}
