#ifndef SAL_TEXTURE_H
#define SAL_TEXTURE_H

#include "Basic.h"

struct Texture2D {
    u32 id;
    u32 width;
    u32 height;
};

namespace Renderer2D {
    Texture2D CreateTexture(u32 width, u32 height, void* pixels);
    void DestroyTexture(const Texture2D& tex);

    Texture2D LoadTexture(const char* filename);
}

#endif
