#include "mortar.h"
#include "stdio.h"

mortar_init_info mortar_init()
{
    mortar_init_info info = {
        .window = nullptr,
        .gl_ctx = nullptr
    };
    printf("Hello Mortar!\n");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS)) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    // Create an SDL window with OpenGL ES context
    SDL_Window* window = SDL_CreateWindow("OpenGL ES 2.0 Triangle", 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!context) {
        printf("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    int isMadeCurrent = SDL_GL_MakeCurrent(window, context);


    gladLoadGLES2Loader(SDL_GL_GetProcAddress);
    printf("Current context? %d\n", isMadeCurrent);
    printf("GL_VERSION : %s\n", glGetString(GL_VERSION));
    printf("GL_VENDOR  : %s\n", glGetString(GL_VENDOR));
    printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));

    info.window = window;
    info.gl_ctx = context;

	return info;
}

void mortar_quit(mortar_init_info* info)
{
    SDL_GL_DestroyContext(info->gl_ctx);
    SDL_DestroyWindow(info->window);
    SDL_Quit();
}