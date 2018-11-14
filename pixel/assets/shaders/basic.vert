#version 410

in vec3 position;
in vec3 tex_coord;
in vec4 color;

out vec3 _tex_coord;
out vec4 _color;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position.xyz, 1.0);
    _tex_coord = tex_coord;
    _color = color;
}
