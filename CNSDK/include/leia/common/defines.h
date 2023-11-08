#pragma once

#if defined(_WIN32) || defined(_WIN64)
#    define LEIA_OS_WINDOWS
#elif defined(__ANDROID__)
#    define LEIA_OS_ANDROID
#else
#    error "Unsupported platform"
#endif

#if defined(_MSC_VER)
#    define LEIA_COMPILER_MSVC
#elif defined(__clang__)
#    define LEIA_COMPILER_CLANG
#else
#    error "Unsupported compiler"
#endif

#if defined(LEIA_OS_WINDOWS)
#    if defined(LEIA_COMPILER_MSVC)
#        define LEIA_EXPORT __declspec(dllexport)
#        define LEIA_IMPORT __declspec(dllimport)
#        define LEIA_HIDDEN
#    else
#        define LEIA_EXPORT __attribute__((dllexport))
#        define LEIA_IMPORT __attribute__((dllimport))
#        define LEIA_HIDDEN
#    endif
#else
#    define LEIA_EXPORT __attribute__((visibility("default")))
#    define LEIA_IMPORT
#    define LEIA_HIDDEN __attribute__((visibility("hidden")))
#endif

#if defined(LEIA_OS_ANDROID)
#    define LEIA_CLASS_EXPORT LEIA_EXPORT
#    define LEIA_CLASS_IMPORT LEIA_IMPORT
#else
#    define LEIA_CLASS_EXPORT
#    define LEIA_CLASS_IMPORT
#endif

#if defined(LEIA_COMPILER_MSVC)
#    define LEIA_DEPRECATED __declspec(deprecated)
#elif defined(LEIA_COMPILER_CLANG)
#    define LEIA_DEPRECATED __attribute__((__deprecated__))
#else
#    define LEIA_DEPRECATED
#endif

#if defined(LEIA_COMPILER_MSVC)
#    define LEIA_FORCE_INLINE __forceinline
#elif defined(LEIA_COMPILER_CLANG)
#    define LEIA_FORCE_INLINE inline __attribute__((always_inline))
#else
#    define LEIA_FORCE_INLINE inline
#endif

#if !defined(LEIA_FUNCTION)
#    if defined(LEIA_COMPILER_CLANG)
#        define LEIA_FUNCTION __PRETTY_FUNCTION__
#    elif defined(LEIA_COMPILER_MSVC)
#        define LEIA_FUNCTION __FUNCSIG__
#    else
#        define LEIA_FUNCTION __FUNCTION__
#    endif
#endif // !LEIA_FUNCTION

#ifdef __cplusplus
#    define BEGIN_CAPI_DECL extern "C" {
#    define END_CAPI_DECL }
#else
#    define BEGIN_CAPI_DECL
#    define END_CAPI_DECL
#endif

#if defined(__has_cpp_attribute)
#    if __has_cpp_attribute(nodiscard)
#        define LEIA_NODISCARD [[nodiscard]]
#    endif
#endif
#ifndef LEIA_NODISCARD
#    if defined(_MSC_VER) && _MSC_VER >= 1700
#        define LEIA_NODISCARD _Check_return_
#    else
#        define LEIA_NODISCARD __attribute__((warn_unused_result))
#    endif
#endif

#define _LEIA_STRINGIZE(x) #x
#define LEIA_STRINGIZE(x) _LEIA_STRINGIZE(x)

#if defined(__LP64__) || defined(_WIN64) || (defined(__x86_64__) && !defined(__ILP32__)) || defined(_M_X64) || defined(__ia64) || defined(_M_IA64) || \
    defined(__aarch64__) || defined(__powerpc64__)
#    define LEIA_X64
#else
#    define LEIA_X32
#endif

/**
 * The OWNING tag marks the pointer as an owning one.
 * The receiver takes ownership over it and
 * must release it before it goes out of scope.
 */
#define OWNING(ptr) ptr

/**
 * The NONOWNING tag marks the pointer as non-owning.
 */
#define NONOWNING(ptr) ptr

#define LEIA_USE_OPENGL

#if defined(LEIA_X64)
#    define LEIA_USE_VULKAN // Vulkan is only supported in x64 builds
#endif

#ifdef LEIA_OS_WINDOWS
#    define LEIA_USE_DIRECTX
#    define LEIA_USE_DIRECTX12 // Disabled until CI issues resolved
#endif
