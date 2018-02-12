#version 410

in vec2 position;
in vec2 tex_coord;

out vec2 _tex_coord;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position.xy, 1.0, 1.0);
    _tex_coord = tex_coord;
}
