#pragma once

#if defined(_WIN32) || defined(_WIN64)
# define LEIA_OS_WINDOWS
#elif defined(__ANDROID__)
# define LEIA_OS_ANDROID
#else
# error "Unsupported platform"
#endif

#if defined(_MSC_VER)
# define LEIA_COMPILER_MSVC
#elif defined(__clang__)
# define LEIA_COMPILER_CLANG
#else
# error "Unsupported compiler"
#endif

#if defined(LEIA_OS_WINDOWS)
# if defined(LEIA_COMPILER_MSVC)
#  define LEIA_EXPORT __declspec(dllexport)
#  define LEIA_IMPORT __declspec(dllimport)
#  define LEIA_HIDDEN
# else
#  define LEIA_EXPORT __attribute__((dllexport))
#  define LEIA_IMPORT __attribute__((dllimport))
#  define LEIA_HIDDEN
# endif
#else
# define LEIA_EXPORT __attribute__((visibility("default")))
# define LEIA_IMPORT
# define LEIA_HIDDEN __attribute__((visibility("hidden")))
#endif

#ifdef __cplusplus
#define BEGIN_CAPI_DECL extern "C" {
#define END_CAPI_DECL }
#else
#define BEGIN_CAPI_DECL
#define END_CAPI_DECL
#endif

// Graphics API
//#define LEIA_USE_NEW_RENDERER
#define LEIA_USE_OPENGL
//#define LEIA_USE_DIRECTX
//#define LEIA_USE_DIRECTX12
//#define LEIA_USE_VULKAN
