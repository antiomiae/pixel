#version 410

in vec2 texture_coord;
//uniform vec4 inputColour;
out vec4 fragColor;

uniform sampler2D tex;

void main() {
  fragColor = texture(tex, texture_coord, 0);
  if (fragColor.a == 0.0) {
    discard;
  }
}
