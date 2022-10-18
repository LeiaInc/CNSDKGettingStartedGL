#pragma once

#include "leia/sdk/api.h"
#include "leia/common/jniTypes.h"

#if defined(LEIA_OS_WINDOWS)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <thread>
#include <cstdint>

struct AInputEvent;
struct ImGuiContext;

namespace leia::sdk {

#if defined(LEIA_OS_WINDOWS)
using GuiSurface = HWND;
#else
using GuiSurface = void*;
#endif

struct GuiInitArgs {
    float fontGlobalScale = 0.0f;
    GuiSurface surface = nullptr;
    void*      d3d11Device = nullptr;
    void*      d3d11DeviceContext = nullptr;
};

class Gui {
public:
    bool isVisible = true;

    LEIASDK_API
    Gui(GuiInitArgs const&);

    LEIASDK_API
    ~Gui();

    struct InputState {
        bool wantCaptureInput = false;     // if true, do not process any input
        bool wantCaptureMouse = false;     // if true, do not process mouse input
        bool wantCaptureKeyboard = false;  // if true, do not process keyboard input
        bool wantTextInput = false;        // if true, open textual keyboard input
    };

#if defined(LEIA_OS_WINDOWS)
    LEIASDK_API
    InputState ProcessInput(GuiSurface surface, uint32_t msg, uint64_t wparam, int64_t lparam);
#elif defined(LEIA_OS_ANDROID)
    LEIASDK_API
    InputState ProcessInput(AInputEvent const* inputEvent);
    InputState ProcessInputMotion(JNIEnv* jniEnv, jobject motionEvent);
#endif

    LEIASDK_API
    bool NewFrame(int width, int height);

    LEIASDK_API
    void Render();

private:
    ImGuiContext* _imGuiContext;
    std::thread::id _owningThreadId;
};

} // namespace leia::sdk
