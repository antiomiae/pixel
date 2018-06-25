#version 410

in vec4 pos__last_pos;
out vec4 _pos__last_pos;
in vec4 vel__acc;
out vec4 _vel__acc;
in vec2 mass__inv_mass;
out vec2 _mass__inv_mass;

uniform float dt;

void main() {
    gl_Position = vec4(1.0, 1.0, 1.0, 1.0);

    vec2 new_pos = pos__last_pos.xy + dt*vel__acc.xy + 0.5*(dt*dt)*vel__acc.zw;
    _pos__last_pos = vec4(new_pos, pos__last_pos.xy);

    vec2 total_force = vec2(0);

    // do mutual gravity

    vec2 new_acc = total_force * mass__inv_mass.y;

    _vel__acc.xy = vel__acc.xy + 0.5*(vel__acc.zw + new_acc)*dt;
    _vel__acc.zw = new_acc;

    _mass__inv_mass = mass__inv_mass;
}
