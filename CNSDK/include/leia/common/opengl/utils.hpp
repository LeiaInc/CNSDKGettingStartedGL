#pragma once

#include "leia/common/opengl/api.h"
#include "leia/common/opengl/shader.hpp"

// OpenGL
#include <GL/CAPI_GLE.h>

#if defined(__ANDROID__)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>
//#include <GLES2/gl2platform.h>

#define GLFW_INCLUDE_ES3
typedef EGLContext HGLRC;
#endif

namespace leia::opengl {

struct SavedOpenGLState
{
    static const int scMaxVertexAttribArray = 16;
    static const int scMaxBoundTextures     = 16;

    struct SavedVertexAttribArray
    {
        GLint Index      = 0;
        GLint Enabled    = 0;
        GLint Size       = 0;
        GLint Type       = 0;
        GLint Normalized = 0;
        GLint Stride     = 0;
        void* Pointer    = nullptr;
    };

    GLint Program                           = 0;
    GLint VAO                               = 0;
    GLint ArrayBuffer                       = 0;
    GLint ElementArrayBuffer                = 0;
    GLint FrameBuffer                       = 0;
    GLint RenderBuffer                      = 0;
    int   MaxSavedVertexAttribArray         = 0;
    GLint Viewport[4]                       = { 0,0,0,0 };
    int   MaxBoundTextures                  = 0;
    GLint BoundTextures[scMaxBoundTextures] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    GLint ActiveTexture                     = 0;
    SavedVertexAttribArray VertexAttribArray[scMaxVertexAttribArray];
};

LEIA_COMMON_OPENGL_API
bool CreateRenderTarget(int width, int height, bool includeDepthBuffer, GLuint* frameBuffer, GLuint* texture, GLuint* depthBuffer, bool isProtectedTarget);
LEIA_COMMON_OPENGL_API
void SaveOpenGLState(SavedOpenGLState& state, int maxVertexAttribArray, int maxBoundTextures);
LEIA_COMMON_OPENGL_API
void RestoreOpenGLState(SavedOpenGLState& state);
LEIA_COMMON_OPENGL_API
bool UpdateBuffers(int newWidth, int newHeight, int newCount, int& width, int& height, int& count, GLuint* textures, GLuint* depthBuffers, GLuint* renderTargets, bool isProtectedBuffer = false);
LEIA_COMMON_OPENGL_API
void CreateRectangularTexCoordVertexArray(float left, float bottom, float top, float right, GLuint& vertexArray, GLuint& vertexBuffer, GLuint& indexBuffer);

} // namespace leia::opengl
