#version 410

in vec2 position;
in vec4 color;

in vec2 center;

out vec2 _center;
out vec4 _color;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(position, 0, 1);
    _color = color;
    _center = (projection * vec4(center, 0, 1)).xy;
}
