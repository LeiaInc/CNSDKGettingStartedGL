#pragma once

#include "leia/common/api.h"

#define IMGUI_API LEIA_COMMON_API

struct ImGuiContext;
extern thread_local ImGuiContext* g_ImGuiTLS;
#define GImGui g_ImGuiTLS
