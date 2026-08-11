/**
 * @file CompilerDriver.hpp
 * @brief This file contains the main class for the vee compiler driver.
 * 
 * This is the main entry point for the vee compiler. Similar to main, it handles
 * all CLI arguments and runs the entire process automatically. For more flexibility, you
 * *can* use the individual components directly (like `Compilation`), but its not recommended
 * (You will have to create the config and context yourself).
 */

#pragma once

#include <iostream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/compilation/CompilationConfig.hpp"
#include "veec/io/IWriter.hpp"
#include "veec/io/StreamWriter.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

/**
 * @class CompilerDriver
 * @brief This is the main entry point for the vee compiler.
 */
class CompilerDriver {
public:
    /**
     * @brief Creates a new CompilerDriver instance.
     * @param outputWriter The writer to use for outputting diagnostics and other information.
     */
    CompilerDriver() = default;

    int run(int argc, const char** argv);
};

} // namespace compilation
VEEC_NAMESPACE_END
