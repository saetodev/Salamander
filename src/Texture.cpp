#include "Texture.h"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture2D Renderer2D::CreateTexture(u32 width, u32 height, void* pixels) {
    Texture2D tex = {
        .width  = width,
        .height = height, 
    };

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    return tex;
}

void Renderer2D::DestroyTexture(const Texture2D& tex) {
    glDeleteTextures(1, &tex.id);
}

Texture2D Renderer2D::LoadTexture(const char* filename) {
    int width, height, comp;
    u8* pixels = stbi_load(filename, &width, &height, &comp, 4);

    if (!pixels) {
        //TODO: log this error
        return Texture2D{};
    }

    Texture2D tex = CreateTexture(width, height, pixels);
    stbi_image_free(pixels);

    return tex;
}
