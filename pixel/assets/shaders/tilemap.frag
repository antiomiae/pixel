#version 410

in vec2 _tex_coord;

out vec4 fragColor;

uniform sampler2D tex;

void main() {
  fragColor = texelFetch(tex, ivec2(_tex_coord), 0);

  if (fragColor.a == 0.0) {
    discard;
  }
}
