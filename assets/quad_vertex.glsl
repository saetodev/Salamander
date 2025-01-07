#version 410 core

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec4 a_color;
layout(location = 2) in vec2 a_texCoords;
layout(location = 3) in int a_texID;

layout(location = 0) out vec4 v_color;
layout(location = 1) out vec2 v_texCoords;
layout(location = 2) flat out int v_texID;

uniform mat4 u_projection;

void main() {
    v_color     = a_color;
    v_texCoords = a_texCoords;
    v_texID     = a_texID;

    gl_Position = u_projection * a_position;
}
