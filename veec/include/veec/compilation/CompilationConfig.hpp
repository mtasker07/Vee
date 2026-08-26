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
    class CLICommandOptions;
}

namespace compilation {

/**
 * @struct CompilationConfig
 * @brief Used to configure the compilation process.
 */
struct CompilationConfig {
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
