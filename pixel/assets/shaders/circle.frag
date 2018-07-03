#version 410

flat in vec2 _center;
flat in vec4 _color;

out vec4 fragColor;

void main() {
    fragColor = _color;

    if (fragColor.a == 0.0) {
      discard;
    }
}
