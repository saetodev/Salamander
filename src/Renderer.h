#ifndef SAL_RENDERER_H
#define SAL_RENDERER_H

#include "Basic.h"

struct Texture2D {
    u32 id;
    u32 width;
    u32 height;
};

namespace Renderer2D {
    void Init();
    void Shutdown();

    void Start(const glm::mat4& projection);
    void End();

    void DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color);
}

#endif
