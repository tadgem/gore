#include "gore.h"

#include "stdio.h"
#include "backends/imgui_impl_opengl3.h"
#include "backends/imgui_impl_sdl3.h"

namespace gore
{
    ctx init()
    {
        ctx info = {};
        info.run = true;
        printf("Hello GORE  !\n");


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


        gladLoadGLES2Loader((GLADloadproc) SDL_GL_GetProcAddress);
        printf("Current context? %d\n", isMadeCurrent);
        printf("GL_VERSION : %s\n", glGetString(GL_VERSION));
        printf("GL_VENDOR  : %s\n", glGetString(GL_VENDOR));
        printf("GL_RENDERER: %s\n", glGetString(GL_RENDERER));

        info.window = window;
        info.gl_ctx = context;


        info.imgui_context = ImGui::CreateContext();
        auto imgui_io = &ImGui::GetIO();
        imgui_io->ConfigFlags |=
            ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
        imgui_io->ConfigFlags |=
            ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
        imgui_io->ConfigFlags |=
            ImGuiConfigFlags_DockingEnable; // Enable imgui window docking

        ImGui_ImplSDL3_InitForOpenGL(window, context);
        ImGui_ImplOpenGL3_Init("#version 320 es");

        return info;
    }

    bool run(ctx *ctx)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            ImGui_ImplSDL3_ProcessEvent(&e);
            if (e.type == SDL_EVENT_QUIT)
            {
                ctx->run = false;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        if (ctx->run)
        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL3_NewFrame();
            ImGui::NewFrame();

        }
        return ctx->run;
    }

    void frame(ctx *ctx)
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(ctx->window);
    }

    void quit(ctx* info)
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL3_Shutdown();

        SDL_GL_DestroyContext(info->gl_ctx);
        SDL_DestroyWindow(info->window);
        SDL_Quit();
    }
}