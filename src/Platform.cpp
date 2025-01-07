#include "Platform.h"

#include <stdio.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <SDL2/SDL.h>

static SDL_Window* m_window;
static SDL_GLContext m_context;
static bool m_running;

static Settings m_settings;
static Input m_input;

void Platform::Init(const Settings& settings) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("ERROR: %s\n", SDL_GetError());
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 

    m_window = SDL_CreateWindow(settings.windowTitle.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.windowWidth, settings.windowHeight, SDL_WINDOW_SHOWN);

    if (!m_window) {
        printf("ERROR: %s\n", SDL_GetError());
        exit(-1);
    }

    m_context = SDL_GL_CreateContext(m_window);

    if (!m_context) {
        printf("ERROR: %s\n", SDL_GetError());
        exit(-1);
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("ERROR: Failed to load OpenGL");
        exit(-1);
    }

    Renderer2D::Init();

    m_settings = settings;
    m_running  = true;
}

void Platform::Shutdown() {
    Renderer2D::Shutdown();

    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);

    SDL_Quit();
}

void Platform::Run() {
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f, 0.0f, 1.0f);

    while (m_running) {
        m_input.Clear();
        
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT: {
                    m_running = false;
                    break;
                }

                case SDL_KEYDOWN:
                case SDL_KEYUP: {
                    int keyCode = event.key.keysym.sym;

                    bool isDown  = (event.key.state == SDL_PRESSED);
                    bool wasDown = (event.key.state == SDL_RELEASED) || (event.key.repeat != 0); 

                    m_input.SetKey(keyCode, isDown, wasDown);

                    break;
                }
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        Renderer2D::Start(projection); 
        
        if (m_settings.updateFunc) {
            m_settings.updateFunc(m_input);
        }

        Renderer2D::End();

        SDL_GL_SwapWindow(m_window);
    }
}