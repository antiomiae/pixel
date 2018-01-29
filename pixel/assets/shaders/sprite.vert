#version 410

in vec3 position;
in vec2 center;
in float angle;
in ivec4 texture_region; // x, y, width, height
in int texture_layer;
in vec2 texture_coord;
in vec2 vertex;

out vec2 _texture_coord;

flat out int _texture_layer;

uniform mat4 projection;

vec2 rotate(vec2 v, float a) {
	float s = sin(a);
	float c = cos(a);
	mat2 m = mat2(c, -s, s, c);
	return m * v;
}

mat2 rotate(float r) {
    float s = sin(r);
    float c = cos(r);
    return mat2(c, -s, s, c);
}

void main() {
    vec2 rotated_vertex = rotate(angle) * (vertex - vec2(0.5)) + vec2(0.5);
    gl_Position = projection * vec4(position.xy + texture_region.zw * (rotated_vertex - center), position.z, 1.0);

    //gl_Position = projection * vec4(position.xy + vertex * texture_region.zw, position.z, 1);

    _texture_coord = texture_region.xy + texture_coord * texture_region.zw;
    _texture_layer = texture_layer;
}
