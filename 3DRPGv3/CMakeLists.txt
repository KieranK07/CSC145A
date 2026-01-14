cmake_minimum_required(VERSION 3.10)
project(RPG_Scratch)

set(CMAKE_CXX_STANDARD 17)

# Library Paths
include_directories(${CMAKE_SOURCE_DIR}/libs/glad/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glfw/include)
include_directories(${CMAKE_SOURCE_DIR}/libs/glm)
include_directories(${CMAKE_SOURCE_DIR}/libs/stb)
include_directories(${CMAKE_SOURCE_DIR}/libs/imgui)
include_directories(${CMAKE_SOURCE_DIR}/libs/imgui/backends)

link_directories(${CMAKE_SOURCE_DIR}/libs/glfw/lib)

# Source Files
file(GLOB_RECURSE SOURCES "src/*.cpp")
set(GLAD_SOURCE "${CMAKE_SOURCE_DIR}/libs/glad/src/glad.c")

# ImGui Source Files
set(IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/libs/imgui/backends/imgui_impl_opengl3.cpp
)

add_executable(RPG_Scratch ${SOURCES} ${GLAD_SOURCE} ${IMGUI_SOURCES})

# Link
target_link_libraries(RPG_Scratch PRIVATE glfw3 opengl32)