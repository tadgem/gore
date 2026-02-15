add_library(gore_core
        ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c
        src/gore.cpp
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/backends/imgui_impl_sdl3.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
        include/maths.h
        include/stl.h
        include/json.h
        include/glu.h
        src/glu.cpp
        include/model.h
        src/model.cpp)

target_include_directories(gore_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${EXT_GORE_HEADERS})

target_link_libraries(gore_core SDL3-static SDL3::Headers mimalloc-static)

target_compile_definitions(gore_core PUBLIC "IMGUI_IMPL_OPENGL_ES3")