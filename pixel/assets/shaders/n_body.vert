#version 410

in vec4 position__last_position;
out vec4 _position__last_position;
in vec4 vec__acc;
out vec4 _vec__acc;
in vec2 mass__inv_mass;
out vec2 _mass__inv_mass;

void main() {
    gl_Position = vec4(1.0, 1.0, 1.0, 1.0);

    _position__last_position = position__last_position;
    _vec__acc = vec__acc;
    _mass__inv_mass = mass__inv_mass;
}
