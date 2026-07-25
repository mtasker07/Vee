/**
 * @file Compilation.hpp
 * @brief This file contains the main interface for the vee compiler.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/CompilationConfig.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

VEEC_NAMESPACE_BEGIN

struct CompilationResult {
    bool success = false;
    std::vector<diagnostics::UserDiagnostic> diagnostics;
};

/**
 * @class Compilation
 * @brief The Compilation class represents a single compilation process in the vee compiler.
 * The compilation is responsible for converting source code into machine code.
 */
class Compilation {
public:
    /**
     * @brief Creates a new Compilation instance with a default config.
     */
    Compilation() = default;
    /**
     * @brief Creates a new Compilation instance with the provided config.
     * @param cfg The configuration for this compilation.
     */
    Compilation(const CompilationConfig& cfg)
        : _config(cfg), _ctx() {}


    /**
     * @brief Runs the compilation process.
     * @return A CompilationResult containing the success
     * status and any diagnostics generated during compilation.
     */
    CompilationResult run();

private:
    CompilationConfig _config;
    CompilationContext _ctx;
};

VEEC_NAMESPACE_END
