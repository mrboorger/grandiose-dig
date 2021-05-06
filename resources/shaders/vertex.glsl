#version 330 core

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec4 in_color;

uniform mat4 proj_matrix;
uniform vec2 buffer_pos;

out vec4 vert_color;

void main() {
  gl_Position = proj_matrix * vec4(float(in_pos.x) + buffer_pos.x,
                                   float(in_pos.y) + buffer_pos.y, 1.0, 1.0);
  vert_color = in_color;
}
