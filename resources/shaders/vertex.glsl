#version 330 core

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec2 in_tex_coords;
layout(location = 2) in vec3 in_light;

uniform mat4 proj_matrix;
uniform vec2 buffer_pos;

out vec2 vert_tex_coords;
out vec3 vert_light;

void main() {
  gl_Position = proj_matrix * vec4(float(in_pos.x) + buffer_pos.x,
                                   float(in_pos.y) + buffer_pos.y, 1.0, 1.0);
  vert_light = in_light;
  vert_tex_coords = in_tex_coords;
}
