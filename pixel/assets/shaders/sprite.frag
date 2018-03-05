#version 410

in vec2 _texture_coord;
flat in int _texture_layer;

out vec4 fragColor;

uniform sampler2DArray tex;

void main() {
    ivec2 t = ivec2(_texture_coord);

    fragColor = texelFetch(tex, ivec3(t, _texture_layer), 0);

    if (fragColor.a == 0.0) {
      discard;
    }
}
