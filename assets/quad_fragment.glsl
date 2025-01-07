#version 410 core

#define MAX_TEXTURE_COUNT 8

layout(location = 0) in vec4 v_color;
layout(location = 1) in vec2 v_texCoords;
layout(location = 2) flat in int v_texID;

out vec4 out_color;

uniform sampler2D u_textureSlots[MAX_TEXTURE_COUNT];

void main() {
    out_color = v_color;

    switch (v_texID) {
        case 0: out_color *= texture(u_textureSlots[0], v_texCoords); break;
        case 1: out_color *= texture(u_textureSlots[1], v_texCoords); break;
        case 2: out_color *= texture(u_textureSlots[2], v_texCoords); break;
        case 3: out_color *= texture(u_textureSlots[3], v_texCoords); break;
        case 4: out_color *= texture(u_textureSlots[4], v_texCoords); break;
        case 5: out_color *= texture(u_textureSlots[5], v_texCoords); break;
        case 6: out_color *= texture(u_textureSlots[6], v_texCoords); break;
        case 7: out_color *= texture(u_textureSlots[7], v_texCoords); break;
    }
}
