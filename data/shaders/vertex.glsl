#version 330 core
layout (location=0) in vec3 l_position;
layout (location=1) in vec3 l_normal;
layout (location=2) in vec2 l_uv;
layout (location=3) in uint l_color;

uniform float u_backdrop_color_usage;
uniform mat4 u_mvp;
out vec4 v_color;
out vec2 v_uv;
out vec3 v_normal;

const vec4 WHITE = vec4(1.0, 1.0, 1.0, 1.0);

vec4 split_color(uint color)
{
    vec4 result;
    result.r = ((color >> 8) & uint(0xff)) / 256.0;
    result.b = ((color >> 16) & uint(0xff)) / 256.0;
    result.g = ((color >> 24) & uint(0xff)) / 256.0;
    result.a = float(color & uint(0xff));
    return result;
}

void main() {
    vec4 color = mix(split_color(l_color), WHITE, u_backdrop_color_usage);
    v_uv = l_uv;
    v_normal = l_normal;
    v_color = color;
    gl_Position = u_mvp * vec4(l_position, 1.0f);
}
