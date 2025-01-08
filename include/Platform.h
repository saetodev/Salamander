#ifndef SAL_PLATFORM_H
#define SAL_PLATFORM_H

#include "Input.h"
#include "Renderer.h"

#include <string>
#include <functional>

struct Settings {
    int windowWidth;
    int windowHeight;
    std::string windowTitle;

    std::function<void(const Input&)> updateFunc;
};

namespace Platform {
    void Init(const Settings& settings);
    void Shutdown();

    void Run();
}

#endif
