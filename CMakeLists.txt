cmake_minimum_required(VERSION 3.10)

# set the project name
project(arcGB)

find_package(SDL2 REQUIRED)
include_directories( ${SDL2_INCLUDE_DIRS} ext/imgui ext/imgui/backends ext/imgui_club/imgui_memory_editor/ ext/ImGuiFileDialog/ImGuiFileDialog/ )

# add the executable
add_executable(arcGB src/arcGB.cpp  src/arcGB.h  src/cartridge.cpp  src/cartridge.h  src/cpu.cpp  src/cpu.h 
    ext/imgui/imgui.cpp 
    ext/imgui/imgui_draw.cpp 
    ext/imgui/imgui_tables.cpp 
    ext/imgui/imgui_widgets.cpp 
    ext/imgui/imgui_demo.cpp 
    ext/imgui_club/imgui_memory_editor/imgui_memory_editor.h 
    ext/imgui/backends/imgui_impl_sdl.cpp 
    ext/imgui/backends/imgui_impl_sdlrenderer.cpp 
    ext/ImGuiFileDialog/ImGuiFileDialog/ImGuiFileDialog.cpp
    )

    target_link_libraries(arcGB ${SDL2_LIBRARIES})