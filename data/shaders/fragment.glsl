#version 330 core
out vec4 o_color;
in vec4 v_color;
in vec2 v_uv;
in vec3 v_normal;

uniform float u_alpha;
uniform vec3 u_diffuse;
uniform vec3 u_ambient;
uniform float u_ambient_power;

uniform sampler2D u_texture;

void main() {
    vec4 ambient = u_ambient_power * vec4(u_ambient, 1.0);
    vec4 final_light_color = ambient;
    o_color = texture(u_texture, v_uv) * v_color;
}
