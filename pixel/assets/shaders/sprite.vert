#version 410

in vec3 position;
in vec2 center;
in float angle;
in vec4 color;
in vec4 tint;
in ivec4 texture_region; // x, y, width, height
in int flip_flags;
in int flipped;
in int texture_layer;
in vec2 texture_coord;
in vec2 vertex;

out vec2 _texture_coord;
flat out int _texture_layer;
flat out int _flip_flags;

flat out vec4 _color;
flat out vec4 _tint;

uniform mat4 projection;

#define FLIP_H 0x8
#define FLIP_V 0x4
#define FLIP_D 0x2

mat3 rotate(float r) {
    float s = sin(r);
    float c = cos(r);
    return mat3(
        c, s, 0,
        -s, c, 0,
        0, 0, 1
    );
}

mat3 translate(vec2 v) {
    return mat3(
        1, 0, 0,
        0, 1, 0,
        v.x, v.y, 1
    );
}

void main() {
    vec2 basic_vertex = texture_region.zw * vertex;
    vec3 rotated_vertex =
        translate(texture_region.zw * 0.5) *
        rotate(angle) *
        translate(-texture_region.zw * 0.5) *
        vec3(basic_vertex, 1);

    gl_Position = projection * vec4(floor(position.xy + rotated_vertex.xy - center * texture_region.zw) + 0.375, position.z, 1.0);

    vec2 tex_coord = texture_coord;

    if (flip_flags > 0x1) {
        if ((FLIP_H & flip_flags) > 0) {
            tex_coord.x = 1.0 - tex_coord.x;
        }

        if ((FLIP_V & flip_flags) > 0) {
            tex_coord.y = 1.0 - tex_coord.y;
        }

        if ((FLIP_D & flip_flags) > 0) {
            tex_coord = tex_coord.yx;
        }
    }

    if (flipped > 0) {
        _texture_coord = texture_region.xy + tex_coord.yx * texture_region.wz;
    } else {
        _texture_coord = texture_region.xy + tex_coord.xy * texture_region.zw;
    }

    _texture_layer = texture_layer;
    _flip_flags = flip_flags;
    _color = color;
    _tint = tint;
}
