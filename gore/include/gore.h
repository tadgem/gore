#pragma once
#include "imgui.h"
#include "macros.h"
#include "glu.h"
#include "SDL3/SDL.h"
#include "maths.h"
#include "stl.h"
#include "json.h"

namespace gore
{
	struct ctx
	{
		SDL_Window*		window = nullptr;
		SDL_GLContext	gl_ctx = nullptr;
		ImGuiContext*	imgui_context = nullptr;
		bool			run = true;
	};

	ctx					init();
	bool				run(ctx* ctx);
	void				frame(ctx* ctx);
	void				quit(ctx* ctx);
}