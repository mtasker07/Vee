/**
 * @file CompilationContext.hpp
 * @brief This file contains the definition of the CompilationContext struct,
 * which is used to hold context for the compilation process.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

VEEC_NAMESPACE_BEGIN

/**
 * @struct CompilationContext
 * @brief Used to hold context for the compilation process.
 */
struct CompilationContext {
    basic::StringPool strings;
    source::SourceManager sources;
    diagnostics::DiagnosticEngine diagnostics;
};

VEEC_NAMESPACE_END
