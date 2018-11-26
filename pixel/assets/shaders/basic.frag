#version 410

in vec3 _tex_coord;
in vec4 _color;

out vec4 fragColor;

uniform sampler2DArray atlas_tex;

void main() {
    fragColor = texelFetch(atlas_tex, ivec3(_tex_coord), 0);

    if (fragColor.a == 0.0) {
        discard;
    }
}
