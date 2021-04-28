#version 330 core

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec4 in_color;

out vec4 vert_color;

void main() {
  gl_Position = vec4(in_pos, 1.0, 1.0);
  vert_color = in_color;
}
