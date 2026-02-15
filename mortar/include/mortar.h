#ifndef MORTAR_H#
#define MORTAR_H
#include "macros.h"
#include "glad/glad.h"
#include "SDL3/SDL.h"
#include "cglm/cglm.h"

typedef struct
{
	SDL_Window*		window;
	SDL_GLContext	gl_ctx;
} mortar_init_info;

mortar_init_info	mortar_init();
void				mortar_quit(mortar_init_info*);

#endif