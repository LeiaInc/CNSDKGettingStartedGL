#ifndef CNSDK_LEIA_CORE_CXX_EXCEPTION_HPP
#define CNSDK_LEIA_CORE_CXX_EXCEPTION_HPP

/*!
 * @addtogroup core
 * @{
 */

#ifdef LEIA_DOXYGEN
/**
 * Define LEIA_WRAPPER_DISABLE_EXCEPTION to disable exceptions in the C++ wrappers.
 * Use corresponding methods to check objects validity (e.g. IsValid).
 */
#    define LEIA_WRAPPER_DISABLE_EXCEPTION
#endif

#if !defined(LEIA_WRAPPER_DISABLE_EXCEPTION)
#    include <stdexcept>
#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION

namespace leia {

#if !defined(LEIA_WRAPPER_DISABLE_EXCEPTION)

/**
 * @brief Base exception type thrown by C++ wrappers.
 */
struct CoreException : public std::exception {
    const char* what() const noexcept override
    {
        return "core generic error";
    }
};

/**
 * @brief Exception thrown on initialization error.
 */
struct CoreInitFailed : public CoreException {
    const char* what() const noexcept override
    {
        return "initialization failed";
    }
};

#endif // !LEIA_WRAPPER_DISABLE_EXCEPTION

} // namespace leia

/*!
 * @}
 */

#endif // CNSDK_LEIA_CORE_CXX_EXCEPTION_HPP
