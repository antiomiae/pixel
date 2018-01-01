#version 410

in vec3 vertex_position;
in vec2 vertex_texture_coord;

out vec2 texture_coord;

uniform mat4 projection;
uniform mat4 view;

void main() {
  mat4 mv = projection * view;
  gl_Position = mv * vec4(vertex_position, 1.0);
  texture_coord = vertex_texture_coord;
}
