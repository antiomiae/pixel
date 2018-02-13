#version 410

in vec2 _tex_coord;

out vec4 fragColor;

uniform sampler2DArray atlas_tex;
uniform usampler2D map_tex;

uniform vec2 tile_size;

ivec3 decode_id(uint id) {
    return ivec3(id & 0xFu, (id & 0xF0u) >> 4, (id & 0xF00u) >> 8);
}

void main() {
    ivec2 map_coord = ivec2(_tex_coord);
    uint ID = texelFetch(map_tex, map_coord, 0).r;
    /* Fractional part of _tex_coord */
    vec2 tile_pixel = (_tex_coord - map_coord) * tile_size;

    ivec3 atlas_coord = decode_id(ID);

    fragColor = texelFetch(atlas_tex, ivec3(tile_pixel + atlas_coord.xy * tile_size, atlas_coord.z), 0);

    if (fragColor.a == 0.0) {
        discard;
    }
}
