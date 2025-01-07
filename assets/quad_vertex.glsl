#version 410 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;

layout(location = 0) out vec4 v_color;

uniform mat4 u_projection;

void main() {
    v_color     = a_color;
    gl_Position = u_projection * a_position;
}
