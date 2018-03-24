#version 410

in vec2 _texture_coord;
flat in int _texture_layer;
flat in int _flip_flags;
flat in vec4 _color;
flat in vec4 _tint;

out vec4 fragColor;

uniform sampler2DArray tex;


void main() {
    ivec2 t = ivec2(_texture_coord);

    vec4 c = texelFetch(tex, ivec3(t, _texture_layer), 0);
    c *= _color;
    c.xyz += _tint.xyz;

    fragColor = c;

    if (fragColor.a == 0.0) {
      discard;
    }
}
