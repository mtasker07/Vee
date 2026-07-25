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