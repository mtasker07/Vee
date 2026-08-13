/**
 * @file CompilationConfig.hpp
 * @brief This file contains the definition of the CompilationConfig struct,
 * which is used to configure the compilation process.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/fs/Path.hpp"

VEEC_NAMESPACE_BEGIN

namespace cli {
    namespace args {
        class CLIOptions;
    }
}

namespace compilation {

/**
 * @struct CompilationConfig
 * @brief Used to configure the compilation process.
 */
struct CompilationConfig {
    /**
     * @brief Creates a CompilationConfig from the given CLIOptions. The options passed
     * must be valid as this function asserts otherwise.
     * @param options The CLIOptions to use for generating the CompilationConfig.
     * @return A CompilationConfig generated from the given CLIOptions.
     */
    static CompilationConfig fromCLIOptions(const cli::args::CLIOptions& options);

    /**
     * @brief A list of input files to use for the compilation process.
     */
    std::vector<fs::Path> inputFiles;
    /**
     * @brief The final output file generated from the compilation of the source files.
     */
    fs::Path outputFile;

    /**
     * @brief The directory specified to output mir. Empty indicates no mir output is desired.
     */
    fs::Path outputMirDirectory;
};

} // namespace compilation
VEEC_NAMESPACE_END
