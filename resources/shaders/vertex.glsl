#version 130

in vec2 in_pos;
in float in_z_coord;
in vec2 in_tex_coords;
in vec3 in_light;
in float in_sun;
in float pos_shift;

uniform mat4 proj_matrix;
uniform vec2 buffer_pos;
uniform float global_sun;
uniform vec2 player_pos;

uniform float tex_height;

out vec2 vert_tex_coords;
out vec3 vert_light;

void main() {
  gl_Position =
      proj_matrix * vec4(float(in_pos.x) + buffer_pos.x,
                         float(in_pos.y) + buffer_pos.y, in_z_coord, 1.0);
  float result_sun = global_sun - (1.0 - in_sun);
  vert_light = vec3(max(in_light.r, result_sun), max(in_light.g, result_sun),
                    max(in_light.b, result_sun));

  float tex_coord_mod =
      mod(in_tex_coords.y, tex_height);
  float tex_coord_y_shift =
      max(min(tex_coord_mod + player_pos.y * pos_shift, tex_height), 0);
  vert_tex_coords =
      vec2(in_tex_coords.x + player_pos.x * pos_shift,
           in_tex_coords.y - tex_coord_mod + tex_coord_y_shift);
}
