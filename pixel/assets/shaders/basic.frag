#version 410

in vec3 _tex_coord;
in vec4 _color;

out vec4 fragColor;

uniform sampler2DArray atlas_tex;

void main() {

    vec4 i = texelFetch(atlas_tex, ivec3(_tex_coord), 0);
    /*fragColor = texelFetch(atlas_tex, ivec3(_tex_coord), 0);

    if (fragColor.a == 0.0) {
        discard;
    }
    */
    fragColor = vec4(0.5, 0.5, 0.5, 1.0);
}
