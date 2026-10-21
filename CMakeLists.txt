cmake_minimum_required(VERSION 3.16)
project(game C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_subdirectory(lib/SDL EXCLUDE_FROM_ALL)

add_executable(game
    src/main.c
    src/engine/engine.c
    src/engine/input.c
    src/engine/renderer.c
    src/game/player.c
    src/game/scene.c
    src/graphics/mesh.c
    src/math/mat4.c
    src/math/vec2.c
    src/math/vec3.c
    src/math/vec4.c
    src/graphics/light.c
)

target_include_directories(game PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(game PRIVATE SDL3::SDL3 m)
