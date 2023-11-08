#include <stdio.h>
#include <assert.h>
#include <memory>
#include "framework.h"

#define SOGL_IMPLEMENTATION_WIN32
#define SOGL_MAJOR_VERSION 4
#define SOGL_MINOR_VERSION 3
#define GLE_WGL_ENABLED
#include "simple-opengl-loader.h"

// CNSDK includes
#include "leia/core/cxx/core.hpp"
#include "leia/core/cxx/interlacer.opengl.hpp"

// CNSDKGettingStartedGL includes
#include "CNSDKGettingStartedGL.h"
#include "CNSDKGettingStartedMath.h"

// CNSDK single library
#pragma comment(lib, "CNSDK/lib/leiaCore-faceTrackingInApp.lib")

// OpenGL libraries
#pragma comment(lib, "opengl32.lib")

#include <vector>

enum class eDemoMode { Spinning3DCube, StereoImage };

// Global Variables.
const wchar_t*                          g_windowTitle                  = L"CNSDK Getting Started OpenGL Sample";
const wchar_t*                          g_windowClass                  = L"CNSDKGettingStartedGLWindowClass";
int                                     g_windowWidth                  = 1280;
int                                     g_windowHeight                 = 720;
bool                                    g_fullscreen                   = true;
std::unique_ptr<leia::Core>             g_sdk                          = nullptr;
std::unique_ptr<leia::InterlacerOpenGL> g_interlacer                   = nullptr;
eDemoMode                               g_demoMode                     = eDemoMode::Spinning3DCube;
float                                   g_geometryDist                 = 500;
bool                                    g_perspective                  = true;
float                                   g_perspectiveCameraFiledOfView = 90.0f * 3.14159f / 180.0f;
float                                   g_orthographicCameraHeight     = 500.0f;
bool                                    g_showGUI                      = true;
int                                     g_viewWidth                    = -1;
int                                     g_viewHeight                   = -1;
bool                                    g_sRGB                         = true;

// Global OpenGL Variables.
GLuint g_shaderProgram     = 0;
GLuint g_vao               = 0;
GLuint g_uniformTransform  = 0;
GLuint g_stereoFrameBuffer = 0;
GLuint g_stereoTexture     = 0;
GLuint g_stereoDepthBuffer = 0;
GLuint g_imageTexture      = 0;

void OnError(const wchar_t* msg)
{
    MessageBox(NULL, msg, L"CNSDKGettingStartedGL", MB_ICONERROR | MB_OK); 
    exit(-1);
}

bool ReadEntireFile(const char* filename, bool binary, char*& data, size_t& dataSize)
{
    const int BUFFERSIZE = 4096;
    char buffer[BUFFERSIZE];

    // Open file.
    FILE* f = fopen(filename, binary ? "rb" : "rt");    
    if (f == NULL)
        return false;

    data     = nullptr;
    dataSize = 0;

    while (true)
    {
        // Read chunk into buffer.
        const size_t bytes = (int)fread(buffer, sizeof(char), BUFFERSIZE, f);
        if (bytes <= 0)
            break;

        // Extend allocated memory and copy chunk into it.
        char* newData = new char[dataSize + bytes];
        if (dataSize > 0)
        {
            memcpy(newData, data, dataSize);
            delete [] data;
            data = nullptr;
        }
        memcpy(newData + dataSize, buffer, bytes);
        dataSize += bytes;
        data = newData;
    }

    // Done and close.
    fclose(f);

    return dataSize > 0;
}

bool ReadTGA(const char* filename, int& width, int& height, GLint& format, char*& data, int& dataSize)
{
    char* ptr = nullptr;
    size_t fileSize = 0;
    if (!ReadEntireFile(filename, true, ptr, fileSize))
    {
        OnError(L"Failed to read TGA file.");
        return false;
    }

    static std::uint8_t DeCompressed[12] = { 0x0, 0x0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
    static std::uint8_t IsCompressed[12] = { 0x0, 0x0, 0xA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

    typedef union PixelInfo
    {
        std::uint32_t Colour;
        struct
        {
            std::uint8_t R, G, B, A;
        };
    } *PPixelInfo;

    // Read header.
    std::uint8_t Header[18] = { 0 };
    memcpy(&Header, ptr, sizeof(Header));
    ptr += sizeof(Header);

    int bitsPerPixel = 0;

    if (!std::memcmp(DeCompressed, &Header, sizeof(DeCompressed)))
    {
        bitsPerPixel = Header[16];
        width        = Header[13] * 256 + Header[12];
        height       = Header[15] * 256 + Header[14];
        dataSize     = ((width * bitsPerPixel + 31) / 32) * 4 * height;

        if ((bitsPerPixel != 24) && (bitsPerPixel != 32))
        {
            OnError(L"Invalid TGA file isn't 24/32-bit.");
            return false;
        }

        format = (bitsPerPixel == 24) ? GL_BGR : GL_BGRA;

        data = new char[dataSize];
        memcpy(data, ptr, dataSize);
    }
    else if (!std::memcmp(IsCompressed, &Header, sizeof(IsCompressed)))
    {
        bitsPerPixel = Header[16];
        width        = Header[13] * 256 + Header[12];
        height       = Header[15] * 256 + Header[14];
        dataSize     = width * height * sizeof(PixelInfo);

        if ((bitsPerPixel != 24) && (bitsPerPixel != 32))
        {
            OnError(L"Invalid TGA file isn't 24/32-bit.");
            return false;
        }

        format = (bitsPerPixel == 24) ? GL_BGR : GL_BGRA;

        PixelInfo Pixel = { 0 };
        int CurrentByte = 0;
        std::size_t CurrentPixel = 0;
        std::uint8_t ChunkHeader = { 0 };
        int BytesPerPixel = (bitsPerPixel / 8);

        data = new char[dataSize];

        do
        {
            memcpy(&ChunkHeader, ptr, sizeof(ChunkHeader));
            ptr += sizeof(ChunkHeader);

            if (ChunkHeader < 128)
            {
                ++ChunkHeader;
                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
                {
                    memcpy(&Pixel, ptr, BytesPerPixel);
                    ptr += BytesPerPixel;

                    data[CurrentByte++] = Pixel.B;
                    data[CurrentByte++] = Pixel.G;
                    data[CurrentByte++] = Pixel.R;
                    if (bitsPerPixel > 24)
                        data[CurrentByte++] = Pixel.A;
                }
            }
            else
            {
                ChunkHeader -= 127;
                memcpy(&Pixel, ptr, BytesPerPixel);
                ptr += BytesPerPixel;

                for (int I = 0; I < ChunkHeader; ++I, ++CurrentPixel)
                {
                    data[CurrentByte++] = Pixel.B;
                    data[CurrentByte++] = Pixel.G;
                    data[CurrentByte++] = Pixel.R;
                    if (bitsPerPixel > 24)
                        data[CurrentByte++] = Pixel.A;
                }
            }
        } while (CurrentPixel < (width * height));
    }
    else
    {
        OnError(L"Invalid TGA file isn't 24/32-bit.");
        return false;
    }
   
    return true;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // Allow CNSDK debug menu to see window messages
    if ((g_interlacer != nullptr) && g_showGUI)
    {
        auto io = g_interlacer->ProcessGuiInput(hWnd, message, wParam, lParam);
        if (io.wantCaptureInput)
            return 0;
    }

    switch (message)
    {

    // Handle keypresses.
    case WM_KEYDOWN:
        switch (wParam) {
            case VK_ESCAPE:
                PostQuitMessage(0);
                break;
        }
        break;

    // Keep track of window size.
    case WM_SIZE:
        g_windowWidth = LOWORD(lParam);
        g_windowHeight = HIWORD(lParam);
        PostMessage(hWnd, WM_PAINT, 0, 0);
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}

float GetSRGB(float value)
{
    // If already in sRGB, no change.
    if (g_sRGB)
        return value;

    // Convert linear->sRGB.
    if (value <= 0.0f)
        return 0.0f;
    else if (value >= 1.0f)
        return 1.0f;
    else if (value <= 0.0031308f)
        return value * 12.92f;
    else
        return 1.055f * pow(value, 1.0f / 2.4f) - 0.055f;
}

BOOL CALLBACK GetDefaultWindowStartPos_MonitorEnumProc(__in  HMONITOR hMonitor, __in  HDC hdcMonitor, __in  LPRECT lprcMonitor, __in  LPARAM dwData)
{
    std::vector<MONITORINFOEX>& infoArray = *reinterpret_cast<std::vector<MONITORINFOEX>*>(dwData);
    MONITORINFOEX info;
    ZeroMemory(&info, sizeof(info));
    info.cbSize = sizeof(info);
    GetMonitorInfo(hMonitor, &info);
    infoArray.push_back(info);
    return TRUE;
}

bool GetNonPrimaryDisplayTopLeftCoordinate(int& x, int& y)
{
    // Get connected monitor info.
    std::vector<MONITORINFOEX> mInfo;
    mInfo.reserve(::GetSystemMetrics(SM_CMONITORS));
    EnumDisplayMonitors(NULL, NULL, GetDefaultWindowStartPos_MonitorEnumProc, reinterpret_cast<LPARAM>(&mInfo));

    // If we have multiple monitors, select the first non-primary one.
    if (mInfo.size() > 1)
    {
        for (int i = 0; i < mInfo.size(); i++)
        {
            const MONITORINFOEX& mi = mInfo[i];

            if (0 == (mi.dwFlags & MONITORINFOF_PRIMARY))
            {
                x = mi.rcMonitor.left;
                y = mi.rcMonitor.top;
                return true;
            }
        }
    }

    // Didn't find a non-primary, there is only one display connected.
    x = 0;
    y = 0;
    return false;
}

HWND CreateGraphicsWindow(HINSTANCE hInstance)
{
    // Create window.
    HWND hWnd = NULL;
    {
        int defaultX = 0;
        int defaultY = 0;
        GetNonPrimaryDisplayTopLeftCoordinate(defaultX, defaultY);

        DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;            // Window Extended Style
        DWORD dwStyle = WS_OVERLAPPEDWINDOW;                            // Windows Style

        RECT WindowRect;
        WindowRect.left = (long)defaultX;
        WindowRect.right = (long)(defaultX + g_windowWidth);
        WindowRect.top = (long)defaultY;
        WindowRect.bottom = (long)(defaultY + g_windowHeight);
        //AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);        // Adjust Window To True Requested Size

        hWnd = CreateWindowEx
        (
            dwExStyle,
            g_windowClass,                         // Class Name
            g_windowTitle,                         // Window Title
            dwStyle |                              // Defined Window Style
            WS_CLIPSIBLINGS |                      // Required Window Style
            WS_CLIPCHILDREN,                       // Required Window Style
            WindowRect.left,                       // Window left
            WindowRect.top,                        // Window top
            WindowRect.right - WindowRect.left,    // Calculate Window Width
            WindowRect.bottom - WindowRect.top,    // Calculate Window Height
            NULL,                                  // No Parent Window
            NULL,                                  // No Menu
            hInstance,                             // Instance
            NULL                                   // Dont Pass Anything To WM_CREATE
        );

        if (!hWnd)
            OnError(L"Failed to create window.");
    }
    return hWnd;
}

void SetFullscreen(HWND hWnd, bool fullscreen)
{
    static int windowPrevX = 0;
    static int windowPrevY = 0;
    static int windowPrevWidth = 0;
    static int windowPrevHeight = 0;

    DWORD style = GetWindowLong(hWnd, GWL_STYLE);
    if (fullscreen)
    {
        RECT rect;
        MONITORINFO mi = { sizeof(mi) };
        GetWindowRect(hWnd, &rect);

        windowPrevX = rect.left;
        windowPrevY = rect.top;
        windowPrevWidth = rect.right - rect.left;
        windowPrevHeight = rect.bottom - rect.top;

        GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi);
        SetWindowLong(hWnd, GWL_STYLE, style & ~WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
            mi.rcMonitor.right - mi.rcMonitor.left,
            mi.rcMonitor.bottom - mi.rcMonitor.top,
            SWP_NOOWNERZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }
    else
    {
        MONITORINFO mi = { sizeof(mi) };
        UINT flags = SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW;
        GetMonitorInfo(MonitorFromWindow(hWnd, MONITOR_DEFAULTTOPRIMARY), &mi);
        SetWindowLong(hWnd, GWL_STYLE, style | WS_OVERLAPPEDWINDOW);
        SetWindowPos(hWnd, HWND_NOTOPMOST, windowPrevX, windowPrevY, windowPrevWidth, windowPrevHeight, flags);
    }
}

HGLRC InitializeOpenGL(HWND hWnd, HDC hDC)
{
    HGLRC context = NULL;

    PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARBFunc = nullptr;
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARBFunc = nullptr;
    {
        // First create a context for the purpose of getting access to wglChoosePixelFormatARB / wglCreateContextAttribsARB.
        PIXELFORMATDESCRIPTOR pfd;
        memset(&pfd, 0, sizeof(pfd));
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cDepthBits = 16;
        int pf = ChoosePixelFormat(hDC, &pfd);
        if (pf == 0)
            OnError(L"Failed to choose pixel format.");

        if (!SetPixelFormat(hDC, pf, &pfd))
            OnError(L"Failed to set pixel format.");

        HGLRC context = wglCreateContext(hDC);
        if (context == 0)
            OnError(L"wglCreateContextfailed failed.");

        if (!wglMakeCurrent(hDC, context))
            OnError(L"wglMakeCurrent failed.");

        wglChoosePixelFormatARBFunc = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
        wglCreateContextAttribsARBFunc = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

        wglDeleteContext(context);

        if (wglChoosePixelFormatARBFunc == nullptr || wglCreateContextAttribsARBFunc == nullptr)
            OnError(L"wglChoosePixelFormatARB and/or wglCreateContextAttribsARB missing.");
    }

    // Now create the real context that we will be using.
    const int iAttributes[] =
    {
        // WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
        WGL_COLOR_BITS_ARB, 32,
        WGL_DEPTH_BITS_ARB, 16,
        WGL_DOUBLE_BUFFER_ARB, GL_TRUE,
        WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB, GL_TRUE,
        0, 0
    };

    const float fAttributes[] = { 0, 0 };
    UINT  numFormats = 0;
    int   pf = 0;
    if (!wglChoosePixelFormatARBFunc(hDC, iAttributes, fAttributes, 1, &pf, &numFormats))
        OnError(L"wglChoosePixelFormatARBFunc failed.");

    PIXELFORMATDESCRIPTOR pfd;
    memset(&pfd, 0, sizeof(pfd));
    if (!SetPixelFormat(hDC, pf, &pfd))
        OnError(L"SetPixelFormat failed.");

#ifdef _DEBUG
    bool UseDebugContext = true;
#else
    bool UseDebugContext = false;
#endif

    // Crete context attributes.
    GLint attribs[16];
    {
        int attribCount = 0;
        if (UseDebugContext)
        {
            attribs[attribCount++] = WGL_CONTEXT_FLAGS_ARB;
            attribs[attribCount++] = WGL_CONTEXT_DEBUG_BIT_ARB;
        }

        attribs[attribCount++] = WGL_CONTEXT_MAJOR_VERSION_ARB;
        attribs[attribCount++] = 3;

        attribs[attribCount++] = WGL_CONTEXT_MINOR_VERSION_ARB;
        attribs[attribCount++] = 0;

        attribs[attribCount++] = GL_CONTEXT_PROFILE_MASK;
        attribs[attribCount++] = GL_CONTEXT_CORE_PROFILE_BIT;

        attribs[attribCount] = 0;
    }

    context = wglCreateContextAttribsARBFunc(hDC, 0, attribs);
    if (!wglMakeCurrent(hDC, context))
        OnError(L"wglMakeCurrent failed.");

    if (!sogl_loadOpenGL()) {
        const char** failures = sogl_getFailures();
        while (*failures) {
            char debugMessage[256];
            snprintf(debugMessage, 256, "SOGL WIN32 EXAMPLE: Failed to load function %s\n", *failures);
            OutputDebugStringA(debugMessage);
            failures++;
        }
        exit(-1);
    }

    if (g_sRGB)
        glEnable(GL_FRAMEBUFFER_SRGB);
       
    return context;
}

void InitializeCNSDK(HWND hWnd, HGLRC context)
{
    // Initialize SDK.
    leia::CoreInitConfiguration coreConfig(nullptr);
    coreConfig.SetFaceTrackingServerLogLevel(kLeiaLogLevelTrace);
    coreConfig.SetFaceTrackingEnable(true);
    coreConfig.SetPlatformLogLevel(kLeiaLogLevelDebug);
    g_sdk = std::make_unique<leia::Core>(coreConfig);

    // Initialize interlacer.
    leia::InterlacerInitConfiguration interlacerConfig;
    interlacerConfig.SetUseAtlasForViews(true);
    g_interlacer = std::make_unique<leia::InterlacerOpenGL>(*g_sdk, interlacerConfig, context);
    g_interlacer->SetSourceViewsSRGB(g_sRGB);

    // Initialize interlacer GUI.
    if (g_showGUI)
    {
        leia::InterlacerDebugMenuConfiguration debugMenuInitArgs = {};
        debugMenuInitArgs.gui.surface = hWnd;
        debugMenuInitArgs.gui.graphicsAPI = LEIA_GRAPHICS_API_OPENGL;
        g_interlacer->InitializeGui(&debugMenuInitArgs, g_sRGB);
    }

    // Set stereo sliding mode.
    const int numViews = g_interlacer->GetNumViews();
    if (numViews != 2)
        OnError(L"Unexpected number of views");

    leia::device::Config* config = g_sdk->GetDeviceConfig();
    g_viewWidth = config->viewResolution[0];
    g_viewHeight = config->viewResolution[1];
    g_sdk->ReleaseDeviceConfig(config);
}

void CompileShader(const char* source, GLuint shader) 
{
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status == 0) 
    {
        char shaderInfoLog[4096] = {};
        glGetShaderInfoLog(shader, 4096, NULL, shaderInfoLog);

        wchar_t msg[4096] = {};
        wsprintf(msg, L"Error compiling shader: %S\nThe shader was:\n%S", shaderInfoLog, source);

        OnError(msg);
    }
}

void LinkShader(unsigned int program)
{
    glLinkProgram(program);

    int status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (!status) 
    {
        char programInfoLog[4096] = {};
        glGetProgramInfoLog(program, 4096, NULL, programInfoLog);

        wchar_t msg[4096] = {};
        wsprintf(msg, L"Error linking shader program: %S", programInfoLog);

        OnError(msg);
    }
}

void LoadScene()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
#endif

    if (g_demoMode == eDemoMode::Spinning3DCube)
    {
        const float cubeWidth  = 200.0f;
        const float cubeHeight = 200.0f;
        const float cubeDepth  = 200.0f;

        const float l = -cubeWidth / 2.0f;
	    const float r = l + cubeWidth;
	    const float b = -cubeHeight / 2.0f;
	    const float t = b + cubeHeight;
	    const float n = -cubeDepth / 2.0f;
	    const float f = n + cubeDepth;

        const int cubeVertsCount = 8;

        const float cubeVerts[cubeVertsCount][3] =
        {
            {l, n, b}, // Left Near Bottom
            {l, f, b}, // Left Far Bottom
            {r, f, b}, // Right Far Bottom
            {r, n, b}, // Right Near Bottom
            {l, n, t}, // Left Near Top
            {l, f, t}, // Left Far Top
            {r, f, t}, // Right Far Top
            {r, n, t}  // Right Near Top
        };

        static const int faces[6][4] =
        {
            {0,1,2,3}, // bottom
            {1,0,4,5}, // left
            {0,3,7,4}, // front
            {3,2,6,7}, // right
            {2,1,5,6}, // back
            {4,7,6,5}  // top
        };

        float c = GetSRGB(0.5f);

        static const float faceColors[6][3] =
        {
            {c,0,0},
            {0,c,0},
            {0,0,c},
            {c,c,0},
            {0,c,c},
            {c,0,c}
        };

        std::vector<float> verts;
        std::vector<float> colors;
        std::vector<unsigned short> indices;
        for (int i = 0; i < 6; i++)
        {
            const int i0 = faces[i][0];
            const int i1 = faces[i][1];
            const int i2 = faces[i][2];
            const int i3 = faces[i][3];

            // Add indices.
            const int startIndex = (int)verts.size()/3;
            indices.emplace_back(startIndex + 0);
            indices.emplace_back(startIndex + 1);
            indices.emplace_back(startIndex + 2);
            indices.emplace_back(startIndex + 0);
            indices.emplace_back(startIndex + 2);
            indices.emplace_back(startIndex + 3);

            verts.emplace_back(cubeVerts[i0][0]);
            verts.emplace_back(cubeVerts[i0][1]);
            verts.emplace_back(cubeVerts[i0][2]);

            verts.emplace_back(cubeVerts[i1][0]);
            verts.emplace_back(cubeVerts[i1][1]);
            verts.emplace_back(cubeVerts[i1][2]);

            verts.emplace_back(cubeVerts[i2][0]);
            verts.emplace_back(cubeVerts[i2][1]);
            verts.emplace_back(cubeVerts[i2][2]);

            verts.emplace_back(cubeVerts[i3][0]);
            verts.emplace_back(cubeVerts[i3][1]);
            verts.emplace_back(cubeVerts[i3][2]);

            colors.emplace_back(faceColors[i][0]);
            colors.emplace_back(faceColors[i][1]);
            colors.emplace_back(faceColors[i][2]);

            colors.emplace_back(faceColors[i][0]);
            colors.emplace_back(faceColors[i][1]);
            colors.emplace_back(faceColors[i][2]);

            colors.emplace_back(faceColors[i][0]);
            colors.emplace_back(faceColors[i][1]);
            colors.emplace_back(faceColors[i][2]);

            colors.emplace_back(faceColors[i][0]);
            colors.emplace_back(faceColors[i][1]);
            colors.emplace_back(faceColors[i][2]);
        }

        const unsigned int vertexPositionsAttributeIndex  = 0;
        const unsigned int vertexColorsAttributeIndex     = 1;

        glGenVertexArrays(1, &g_vao);
        glBindVertexArray(g_vao);

        // Create index buffer.
        GLuint indexBuffer = 0;
        glGenBuffers(1, &indexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), indices.data(), GL_STATIC_DRAW);

        // Create vertex positions buffer.
        GLuint vertexPositionsBuffer = 0;
        glGenBuffers(1, &vertexPositionsBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexPositionsBuffer);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(vertexPositionsAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexPositionsAttributeIndex);

        // Create vertex colors buffer.
        GLuint vertexColorsBuffer = 0;
        glGenBuffers(1, &vertexColorsBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexColorsBuffer);
        glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(vertexColorsAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(vertexColorsAttributeIndex);

        // Unbind
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        const char* szVertexShader =
            "#version 330 core\n"
            "layout(location = 0) in vec3 inPos; \n"
            "layout(location = 1) in vec3 inColor; \n"
            "out vec3 color; \n"
            "uniform mat4 transform; \n"
            "void main() {\n"
            "  gl_Position = transform * vec4(inPos, 1.0); \n"
            "  color = inColor; \n"
            "}\n";

        GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        CompileShader(szVertexShader, vertex_shader);

        const char* szFragmentShader =
            "#version 330 core\n"
            "in vec3 color;\n"
            "out vec4 frag_color;\n"
            "void main() {\n"
            "    frag_color = vec4(color, 1.0);\n"
            "}\n";

        GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        CompileShader(szFragmentShader, fragment_shader);

        g_shaderProgram = glCreateProgram();
        glAttachShader(g_shaderProgram, vertex_shader);
        glAttachShader(g_shaderProgram, fragment_shader);
        LinkShader(g_shaderProgram);

        g_uniformTransform = glGetUniformLocation(g_shaderProgram, "transform");
    }
    else if (g_demoMode == eDemoMode::StereoImage)
    {
        // Load stereo image.
        int width = 0;
        int height = 0;
        GLint format = 0;
        char* data = nullptr;
        int dataSize = 0;
        ReadTGA("StereoBeerGlass.tga", width, height, format, data, dataSize);

        // Generate and bind new texture.
        glGenTextures(1, &g_imageTexture);
        glBindTexture(GL_TEXTURE_2D, g_imageTexture);

        // Set texture properties and image data.
        GLenum aa= glGetError();
        glTexImage2D(GL_TEXTURE_2D, 0, g_sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        GLenum bb = glGetError();

        // Set wrap mode.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Set filter mode.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }    
}

void InitializeOffscreenFrameBuffer()
{    
    // Create a single double-wide offscreen framebuffer. 
    // When rendering, we will do two passes, like a typical VR application.
    // On pass 1 we render to the left and on pass 2 we render to the right.
    {
        // Use Leia's pre-defined view size (you can use a different size to suit your application).
        const int width  = g_viewWidth * 2;
        const int height = g_viewHeight;

        // Create frame buffer
        GLuint newFrameBuffer = 0;
        {
            glGenFramebuffers(1, &newFrameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, newFrameBuffer);
        }

        // Create texture
        GLuint newTexture = 0;
        {
            // Generate and bind new texture.
            glGenTextures(1, &newTexture);
            glBindTexture(GL_TEXTURE_2D, newTexture);

            // Set texture properties.
            glTexImage2D(GL_TEXTURE_2D, 0, g_sRGB ? GL_SRGB8_ALPHA8 : GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

            // Set wrap mode.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Set filter mode.
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }

        // Bind texture to frame buffer
        {
            // Set "newTexture" as our colour attachement #0
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, newTexture, 0);

            // Set the list of draw buffers.
            GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
            glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
        }

        // Create and bind depth buffer.
        GLuint newDepthRenderBuffer = 0;
        {
            // Generate and bind new depthbuffer
            glGenRenderbuffers(1, &newDepthRenderBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, newDepthRenderBuffer);

            // Set depthbuffer properties.
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

            // Bind depthbuffer to framebuffer.
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, newDepthRenderBuffer);
        }

        // Check for errors.
        const bool success = glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
        if (success)
        {
            g_stereoFrameBuffer = newFrameBuffer;
            g_stereoTexture = newTexture;
            g_stereoDepthBuffer = newDepthRenderBuffer;
        }
        else
        {
            OnError(L"Failed to create offscreen frame buffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}

void RotateOrientation(mat3f& orientation, float x, float y, float z)
{
    mat3f rx, ry, rz;
    rx.setAxisAngleRotation(vec3f(1.0, 0.0, 0.0), x);
    ry.setAxisAngleRotation(vec3f(0.0, 1.0, 0.0), y);
    rz.setAxisAngleRotation(vec3f(0.0, 0.0, 1.0), z);
    orientation = orientation * (rx * ry * rz);
}

void Render(HDC hDC, float elapsedTime) 
{    
    const int   viewWidth   = g_viewWidth;
    const int   viewHeight  = g_viewHeight;
    const float aspectRatio = (float)viewWidth / (float)viewHeight;

    GLuint viewsTexture;

    if (g_demoMode == eDemoMode::StereoImage)
    {
        // Clear backbuffer to green.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(GetSRGB(0.0f), GetSRGB(0.25f), GetSRGB(0.0f), GetSRGB(1.0f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewsTexture = g_imageTexture;
    }
    else if (g_demoMode == eDemoMode::Spinning3DCube)
    {
        // geometry transform.
        mat4f geometryTransform;
        {
            // Place cube at specified distance.
            vec3f geometryPos = vec3f(0, g_geometryDist, 0);

            mat3f geometryOrientation;
            geometryOrientation.setIdentity();
            RotateOrientation(geometryOrientation, 0.1f * elapsedTime, 0.2f * elapsedTime, 0.3f * elapsedTime);
            geometryTransform.create(geometryOrientation, geometryPos);
        }

        // Clear backbuffer to green.
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClearColor(GetSRGB(0.0f), GetSRGB(0.25f), GetSRGB(0.0f), GetSRGB(1.0f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Clear offscreen render-target to blue
        glBindFramebuffer(GL_FRAMEBUFFER, g_stereoFrameBuffer);
        glClearColor(GetSRGB(0.0f), GetSRGB(0.0f), GetSRGB(0.25f), GetSRGB(1.0f));
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render stereo views.
        for (int i = 0; i < 2; i++)
        {
            // Get camera properties.
            vec3f camPos = vec3f(0, 0, 0);
            vec3f camDir = vec3f(0, 1, 0);
            vec3f camUp  = vec3f(0, 0, 1);

            // Compute view position and projection matrix for view.
            vec3f viewPos = vec3f(0, 0, 0);
            mat4f cameraProjection;
            if (g_perspective)
            {
                g_interlacer->GetConvergedPerspectiveViewInfo(i, {camPos.e, 3}, {camDir.e, 3}, {camUp.e, 3}, g_perspectiveCameraFiledOfView, aspectRatio, 1.0f, 10000.0f, {viewPos.e, 3}, {cameraProjection.m, 16}, nullptr, nullptr, nullptr);
            }
            else
            {
                g_interlacer->GetConvergedOrthographicViewInfo(i, {camPos.e, 3}, {camDir.e, 3}, {camUp.e, 3}, g_orthographicCameraHeight * aspectRatio, g_orthographicCameraHeight, 1.0f, 10000.0f, {viewPos.e, 3}, {cameraProjection.m, 16}, nullptr, nullptr);
            }

            // Get camera transform.
            mat4f cameraTransform;
            cameraTransform.lookAt(viewPos, viewPos + camDir, camUp);

            // Compute combined matrix.
            const mat4f mvp = cameraProjection * cameraTransform * geometryTransform;

            // Set viewport to render to left, then right.
            glViewport(i * viewWidth, 0, viewWidth, viewHeight);

            glUseProgram(g_shaderProgram);
            glUniformMatrix4fv(g_uniformTransform, 1, GL_FALSE, &mvp.m[0]);
            glBindVertexArray(g_vao);
            glEnableVertexAttribArray(0);
            glEnableVertexAttribArray(1);
            const unsigned int triangles = 6 * 2;
            glDrawElements(GL_TRIANGLES, triangles * 3, GL_UNSIGNED_SHORT, NULL);
        }    

        //
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, g_windowWidth, g_windowHeight);

        viewsTexture = g_stereoTexture;
    }

    // Perform interlacing.
    g_interlacer->SetOutputRenderTarget(0);
    g_interlacer->SetSourceViewsSize(viewWidth, viewHeight, true);
    g_interlacer->SetSourceViews(viewsTexture);
    g_interlacer->DoPostProcess(g_windowWidth, g_windowHeight, false);

    // 
    glFlush();
    SwapBuffers(hDC);
}

void UpdateWindowTitle(HWND hWnd, double curTime) 
{
    static double prevTime = 0;
    static int frameCount = 0;

    frameCount++;

    if (curTime - prevTime > 0.25) 
    {
        const double fps = frameCount / (curTime - prevTime);

        wchar_t newWindowTitle[128];
        swprintf(newWindowTitle, 128, L"%s (%.1f FPS)", g_windowTitle, fps);
        SetWindowText(hWnd, newWindowTitle);

        prevTime = curTime;
        frameCount = 0;
    }
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    // Register window class.
    WNDCLASSEXW wcex;
    wcex.cbSize        = sizeof(WNDCLASSEX);
    wcex.style         = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc   = WndProc;
    wcex.cbClsExtra    = 0;
    wcex.cbWndExtra    = 0;
    wcex.hInstance     = hInstance;
    wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CNSDKGETTINGSTARTEDGL));
    wcex.hCursor       = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName  = NULL;
    wcex.lpszClassName = g_windowClass;
    wcex.hIconSm       = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    RegisterClassExW(&wcex);

    // Create window.
    HWND hWnd = CreateGraphicsWindow(hInstance);

    // Get DC.
    HDC hDC = GetDC(hWnd);

    // Switch to fullscreen if requested.
    if (g_fullscreen)
        SetFullscreen(hWnd, true);

    // Initialize OpenGL.
    HGLRC context = InitializeOpenGL(hWnd, hDC);

    // Initialize CNSDK.
    InitializeCNSDK(hWnd, context);

    // Create our stereo (double-wide) frame buffer.
    if (g_demoMode == eDemoMode::Spinning3DCube)
        InitializeOffscreenFrameBuffer();

    // Prepare everything to draw.
    LoadScene();

    // Show window.
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Enable Leia display backlight.
    g_sdk->SetBacklight(true);

    // Main loop.
    bool finished = false;
    while (!finished)
    {
        // Empty all messages from queue.
        MSG msg = {};
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (WM_QUIT == msg.message)
            {
                finished = true;
                break;
            }
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        // Perform app logic.
        if (!finished)
        {
            // Get timing.
            const ULONGLONG  curTick      = GetTickCount64();
            static ULONGLONG prevTick     = curTick;
            const ULONGLONG  elapsedTicks = curTick - prevTick;
            const double     elapsedTime  = (double)elapsedTicks / 1000.0;
            const double     curTime      = (double)curTick / 1000.0;

            // Render.
            Render(hDC, (float)elapsedTime);

            // Update window title with FPS.
            UpdateWindowTitle(hWnd, curTime);
        }
    }

    // Disable Leia display backlight.
    g_sdk->SetBacklight(false);

    // Cleanup.
    wglDeleteContext(context);

    return 0;
}