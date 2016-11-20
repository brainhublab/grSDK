//
// Created by vtimofeev on 06/11/16.
//

#ifndef VISUALIZATION_IMGUI_IMPL_SDL_H
#define VISUALIZATION_IMGUI_IMPL_SDL_H

#include "ui/imgui/imgui.h"

struct SDL_Window;
typedef union SDL_Event SDL_Event;

IMGUI_API bool        ImGui_ImplSdl_Init(SDL_Window* window);
IMGUI_API void        ImGui_ImplSdl_Shutdown();
IMGUI_API void        ImGui_ImplSdl_NewFrame(SDL_Window* window);
IMGUI_API bool        ImGui_ImplSdl_ProcessEvent(SDL_Event* event);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplSdl_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplSdl_CreateDeviceObjects();

#endif //SOME_IMGUI_IMPL_SDL_H
