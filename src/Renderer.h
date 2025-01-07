#ifndef SAL_RENDERER_H
#define SAL_RENDERER_H

#include "Basic.h"
#include "Texture.h"

namespace Renderer2D {
    void Init();
    void Shutdown();

    void Start(const glm::mat4& projection);
    void End();

    void DrawRect(glm::vec2 position, glm::vec2 size, glm::vec4 color);
    void DrawTexture(const Texture2D& tex, glm::vec2 position, glm::vec2 size, glm::vec4 color);
}

#endif
