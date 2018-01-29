#version 410

in vec2 _texture_coord;
flat in int _texture_layer;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
  fragColor = texelFetch(tex, ivec2(_texture_coord), 0);
  //fragColor = vec4(0.5);

  int i = _texture_layer;

  if (fragColor.a == 0.0) {
    discard;
  }
}
