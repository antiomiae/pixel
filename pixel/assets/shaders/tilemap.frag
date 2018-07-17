#version 410

in vec2 _tex_coord;

out vec4 fragColor;

uniform sampler2DArray atlas_tex;
uniform usampler2D map_tex;

uniform ivec2 tile_size;
uniform ivec2 tile_count;

ivec4 decode_id(uint id) {
    return ivec4(id & 0xFu, (id & 0xF0u) >> 4, (id & 0xF00u) >> 8, (id & 0xF000u) >> 12);
}

#define FLIP_H 0x8
#define FLIP_V 0x4
#define FLIP_D 0x2

void main() {
    vec2 tex_coord = _tex_coord;
    ivec2 map_coord = ivec2(tex_coord);

    uint ID = texelFetch(map_tex, map_coord, 0).r;

    /* Fractional part of _tex_coord */
    ivec2 tile_pixel = ivec2((tex_coord - map_coord) * tile_size);
    tile_pixel.y = tile_size.y - 1 - tile_pixel.y;

    ivec4 atlas_coord = decode_id(ID);
    int flip_flags = atlas_coord.w;

    if (flip_flags > 0x1) {
        if ((FLIP_H & flip_flags) > 0) {
            tile_pixel.x = tile_size.x - 1 - tile_pixel.x;
        }

        if ((FLIP_V & flip_flags) > 0) {
            tile_pixel.y = tile_size.y - 1 - tile_pixel.y;
        }

        if ((FLIP_D & flip_flags) > 0) {
            tile_pixel = tile_pixel.yx;
        }
    }

    fragColor = texelFetch(atlas_tex, ivec3(tile_pixel + atlas_coord.xy * tile_size, atlas_coord.z), 0);

    if (fragColor.a == 0.0) {
        discard;
    }
}
