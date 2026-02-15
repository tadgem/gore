cmake_minimum_required(VERSION 3.12)
project(mortar)

# configure third party libraries

set(SDL_SHARED OFF CACHE INTERNAL "")
set(SDL_STATIC ON  CACHE INTERNAL "")
set(SDL_TEST_LIBRARY OFF CACHE INTERNAL "")

# add third party libraries

add_subdirectory(submodules/sdl)
add_subdirectory(submodules/cglm)

# mortar
add_subdirectory(mortar)

add_executable(mortar_entry main.c)
target_link_libraries(mortar_entry mortar_core)