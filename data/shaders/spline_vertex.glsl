#version 330 core
layout (location=0) in vec3 l_position;
layout (location=1) in vec3 l_color;

uniform mat4 u_mvp;
out vec4 v_color;

void main() {
    v_color = vec4(l_color, 1.0f);
    gl_Position = u_mvp * vec4(l_position, 1.0f);
}