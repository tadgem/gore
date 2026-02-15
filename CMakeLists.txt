cmake_minimum_required(VERSION 3.12)
project(gore)

# configure third party libraries
set(CMAKE_CXX_STANDARD 17)

set(SDL_SHARED OFF CACHE INTERNAL "")
set(SDL_STATIC ON  CACHE INTERNAL "")
set(SDL_TEST_LIBRARY OFF CACHE INTERNAL "")
set(MI_BUILD_TESTS OFF CACHE INTERNAL "")


# add third party libraries
add_subdirectory(submodules/sdl)
add_subdirectory(third_party/mimalloc)

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui)

set(EXT_GORE_HEADERS
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/imgui
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/mimalloc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party/glm
        ${CMAKE_CURRENT_SOURCE_DIR}/submodules/sdl/include
)


# gore
add_subdirectory(gore)

add_executable(mortar_entry main.cpp)
target_link_libraries(mortar_entry gore_core)