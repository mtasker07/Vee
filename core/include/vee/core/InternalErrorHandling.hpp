/**
 * @file InternalErrorHandling.hpp
 * @brief This file contains internal error handling utilities for the vee project.
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <string>
#include <format>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"

/// @brief If the expression is false, logs an assertion failure message and aborts the program immediately.
#define VEE_ASSERT(expr, fmt, ...) \
    do { \
        if (!(expr)) { \
            VEE_NAMESPACE::internal::handleAssertFailure(#expr, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
        } \
    } while (0)

/// @brief Logs a fatal error message and aborts the program immediately.
#define VEE_FATAL(fmt, ...) \
    do { \
        VEE_NAMESPACE::internal::handleFatalError(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
    } while (0)

#if defined(__GNUC__) || defined(__clang__)
    #define PLATFORM_UNREACHABLE() __builtin_unreachable()
#elif defined(_MSC_VER)
    #define PLATFORM_UNREACHABLE() __assume(false)
#else
    #define PLATFORM_UNREACHABLE() std::abort()
#endif

/**
 * @brief Marks a code path as unreachable, if it's reached, aborts the program and logs an error.
 *
 * On MSVC it uses __assume(false) to hint the compiler that this code path is unreachable,
 * on GCC and Clang it uses __builtin_unreachable().
 */
#define VEE_UNREACHABLE(fmt, ...) \
    do { \
        VEE_NAMESPACE::internal::handleUnreachableCode(__FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__); \
        PLATFORM_UNREACHABLE(); \
    } while (0)

VEE_NAMESPACE_BEGIN
namespace internal {

/**
 * @brief Handles an assertion failure by printing the message to stderr and aborting the program.
 *
 * @tparam Args The types of the arguments to format into the message template.
 * @param file The source file where the assertion failed.
 * @param line The line number where the assertion failed.
 * @param func The function name where the assertion failed.
 * @param fmt The format message.
 * @param args The arguments to format into the message template.
 *
 * @note You shouldn't never really call this directly, use the VEE_ASSERT macro instead.
 */
template<typename... Args>
[[noreturn]] inline void handleAssertFailure(const char* exprStr, const char* file, int line, const char* func, const char* fmt, Args... args) {
    std::string formatted = std::vformat(fmt, std::make_format_args(args...));
    std::string full = std::format("Assertion failed: '{}' at {}:{} in function {}: \"{}\"", exprStr, file, line, func, formatted);

    std::cerr << full << std::endl;
    std::abort();
}

/**
 * @brief Handles a fatal error by printing the message to stderr and aborting the program.
 *
 * @tparam Args The types of the arguments to format into the message template.
 * @param file The source file where the fatal error occurred.
 * @param line The line number where the fatal error occurred.
 * @param func The function name where the fatal error occurred.
 * @param fmt The format message.
 * @param args The arguments to format into the message template.
 *
 * @note You shouldn't never really call this directly, use the VEE_FATAL macro instead.
 */
template<typename... Args>
[[noreturn]] inline void handleFatalError(const char* file, int line, const char* func, const char* fmt, Args... args) {
    std::string formatted = std::vformat(fmt, std::make_format_args(args...));
    std::string full = std::format("Fatal error at {}:{} in function {}: \"{}\"", file, line, func, formatted);

    std::cerr << full << std::endl;
    std::abort();
}

/**
 * @brief Handles unreachable code by printing the message to stderr and aborting the program.
 *
 * @tparam Args The types of the arguments to format into the message template.
 * @param file The source file where the unreachable code was hit.
 * @param line The line number where the unreachable code was hit.
 * @param func The function name where the unreachable code was hit.
 * @param fmt The format message.
 * @param args The arguments to format into the message template.
 *
 * @note You shouldn't never really call this directly, use the VEE_UNREACHABLE macro instead.
 */
template<typename... Args>
[[noreturn]] inline void handleUnreachableCode(const char* file, int line, const char* func, const char* fmt, Args... args) {
    std::string formatted = std::vformat(fmt, std::make_format_args(args...));
    std::string full = std::format("Unreachable code hit at {}:{} in function {}: \"{}\"", file, line, func, formatted);

    std::cerr << full << std::endl;
    std::abort();
}

} // namespace internal
VEE_NAMESPACE_END
