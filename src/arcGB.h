#include <SDL.h>

#include <cstdint>
#include <cstdio>
#include <cstdint>

#include <vector>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include "imgui_memory_editor.h"

#include "ImGuiFileDialog.h"

#if !SDL_VERSION_ATLEAST(2, 0, 17)
#error This backend requires SDL 2.0.17+ because of SDL_RenderGeometry() function
#endif

#define PROJECT_NAME "arcGB"