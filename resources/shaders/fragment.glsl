#version 330 core

in vec4 vert_color;

out vec4 o_frag_color;

void main() {
  o_frag_color = vert_color;
}
