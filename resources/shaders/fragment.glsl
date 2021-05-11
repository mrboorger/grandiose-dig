#version 330 core

in vec2 vert_tex_coords;
in vec3 vert_light;

uniform sampler2D texture;

out vec4 o_frag_color;

void main() {
  o_frag_color = vec4(vert_light, 1.0) * texture2D(texture, vert_tex_coords);
}
