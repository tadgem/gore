add_library(mortar_core ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c "src/mortar.c")

target_include_directories(mortar_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(mortar_core SDL3-static SDL3::Headers cglm_headers)