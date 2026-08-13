/**
 * @file CoreDefines.hpp
 * @brief This file contains core macros used throughout the vee compiler.
 */

#pragma once

#define VEEC_NAMESPACE veec
#define VEEC_NAMESPACE_BEGIN namespace VEEC_NAMESPACE {
#define VEEC_NAMESPACE_END }

// VEEC_DEBUG
#if !defined(VEEC_DEBUG)
#ifdef NDEBUG
#define VEEC_DEBUG 0
#else
#define VEEC_DEBUG 1
#endif
#endif

// VEEC_WINDOWS/VEEC_LINUX/VEEC_OSX
#if !defined(VEEC_WINDOWS) && !defined(VEEC_LINUX) && !defined(VEEC_OSX)
#if defined(_WIN32) || defined(_WIN64)
#define VEEC_WINDOWS 1
#define VEEC_LINUX 0
#define VEEC_OSX 0
#elif defined (__linux__)
#define VEEC_WINDOWS 0
#define VEEC_LINUX 1
#define VEEC_OSX 0
#elif defined (__APPLE__) || defined(__MACH__)
#define VEEC_WINDOWS 0
#define VEEC_LINUX 0
#define VEEC_OSX 1
#else
#error "Unsupported platform"
#endif
#endif

