#version 410

in vec3 vertex_position;
in vec2 vertex_texture_coord;
in mat2 single_mat;
in mat2x3 model_mat[2];
in int some_var;
in int int_arr[4];

out vec2 texture_coord;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
  mat4 pv = projection * view;
  float i = float(some_var) * model_mat[0][0][0] * single_mat[0][0] + float(int_arr[0]);
  gl_Position = pv * model * vec4(vertex_position, 1.0);
  texture_coord = vertex_texture_coord;
}
