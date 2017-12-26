#version 410

in vec3 vertex_position;
in vec2 vertex_texture_coord;

out vec2 texture_coord;

void main() {
  gl_Position = vec4(vertex_position * vec3(0.5), 1.0);
  //gl_Position = vec4(0);
  texture_coord = vertex_texture_coord;
}
