#version 410

in vec3 position;
in vec2 center;
in float angle;
in ivec4 texture_region; // x, y, width, height
in int flipped;
in int texture_layer;
in vec2 texture_coord;
in vec2 vertex;

out vec2 _texture_coord;
flat out int _texture_layer;


uniform mat4 projection;

mat2 rotate(float r) {
    float s = sin(r);
    float c = cos(r);
    return mat2(c, s, -s, c);
}

void main() {
    vec2 rotated_vertex = rotate(angle) * (texture_region.zw * (vertex - vec2(0.5))) + texture_region.zw * 0.5;
    gl_Position = projection * vec4(position.xy + rotated_vertex - center * texture_region.zw, position.z, 1.0);

    if (flipped > 0) {
        _texture_coord = texture_region.xy + texture_coord.yx * texture_region.wz;
    } else {
        _texture_coord = texture_region.xy + texture_coord.xy * texture_region.zw;
    }

    _texture_layer = texture_layer;
}
