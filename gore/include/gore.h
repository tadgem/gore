#pragma once
#include "imgui.h"
#include "macros.h"
#include "glad/glad.h"
#include "SDL3/SDL.h"
#include "maths.h"

struct gore_ctx
{
	SDL_Window*		window = nullptr;
	SDL_GLContext	gl_ctx = nullptr;
	ImGuiContext*	imgui_context = nullptr;
	bool			run = true;
};

gore_ctx			gore_init();
bool				gore_run(gore_ctx* ctx);
void				gore_frame(gore_ctx* ctx);
void				gore_quit(gore_ctx* ctx);
