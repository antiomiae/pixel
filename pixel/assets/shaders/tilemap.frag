#version 410

in vec2 _tex_coord;

out vec4 fragColor;

uniform sampler2DArray atlas_tex;
uniform usampler2D map_tex;

uniform vec2 tile_size;

ivec4 decode_id(uint id) {
    return ivec4(id & 0xFu, (id & 0xF0u) >> 4, (id & 0xF00u) >> 8, (id & 0xF000u) >> 12);
}

#define FLIP_H 0x8
#define FLIP_V 0x4
#define FLIP_D 0x2

void main() {
    ivec2 map_coord = ivec2(_tex_coord);
    uint ID = texelFetch(map_tex, map_coord, 0).r;

    /* Fractional part of _tex_coord */
    vec2 tile_pixel = (_tex_coord - map_coord) * tile_size;
    ivec4 atlas_coord = decode_id(ID);
    int flip_flags = atlas_coord.w;

    if ((FLIP_H & flip_flags) > 0) {
        tile_pixel.x = tile_size.x - tile_pixel.x;
    }

    if ((FLIP_V & flip_flags) > 0) {
            tile_pixel.y = tile_size.y - tile_pixel.y;
    }

    if ((FLIP_D & flip_flags) > 0) {
        tile_pixel = tile_pixel.yx;
    }

    fragColor = texelFetch(atlas_tex, ivec3(tile_pixel + atlas_coord.xy * tile_size, atlas_coord.z), 0);

    if (fragColor.a == 0.0) {
        discard;
    }
}
