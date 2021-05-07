#version 330 core

in vec2 vert_tex_coords;

uniform sampler2D texture;

out vec4 o_frag_color;

void main() {
  o_frag_color = texture2D(texture, vert_tex_coords);
}
