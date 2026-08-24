/**
 * @file Compilation.hpp
 * @brief This file contains the main interface for the vee compiler.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/compilation/CompilationConfig.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/cli/CLIRawArgs.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

/**
 * @struct CompilationResult
 * @brief Stores the result of a compilation process.
 */
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
        : _config(cfg) {}

    /**
     * @brief Gets the compilation context for this compilation. The context holds all the state
     * and data structures used during the compilation process (read-only).
     * @return A reference to the compilation context.
     */
    inline const CompilationContext& getContext() const { return _ctx; }
    /**
     * @brief Gets the compilation context for this compilation. The context holds all the state
     * and data structures used during the compilation process.
     * @return A reference to the compilation context.
     */
    inline CompilationContext& getContext() { return _ctx; }

    /**
     * @brief Gets the compilation config for this compilation (read-only).
     * @return A reference to the compilation config.
     */
    inline const CompilationConfig& getConfig() const { return _config; }
    /**
     * @brief Gets the compilation config for this compilation.
     * @return A reference to the compilation config.
     */
    inline CompilationConfig& getConfig() { return _config; }
    /**
     * @brief Sets the compilation config for this compilation.
     * @param cfg The new compilation config.
     */
    inline void setConfig(const CompilationConfig& cfg) { _config = cfg; }

    /**
     * @brief Gets the diagnostics generated during the compilation process (read-only).
     * @return A list of diagnostics generated during the compilation process.
     */
    inline const std::vector<diagnostics::UserDiagnostic>& getDiagnostics() const {
        return _ctx.diagnostics.getDiagnostics();
    }

    /**
     * @brief Runs the compilation process.
     * @return A CompilationResult containing the success
     * status and any diagnostics generated during compilation.
     */
    CompilationResult compile();

private:
    CompilationConfig _config;
    CompilationContext _ctx;

    void loadSourcesFromConfig();

    template<typename Fn>
    inline bool runForEachFile(Fn fn, bool stopOnFail = false);
    template<typename Fn>
    inline bool runForEachUnit(Fn fn, bool stopOnFail = false);

    void registerBuiltins();
    bool tokenize();
    bool parse();
    bool analyze();
    bool generateMir();
};

template<typename Fn>
inline bool Compilation::runForEachFile(Fn fn, bool stopOnFail) {
    bool success = true;
    for (source::SourceFile* sourceFile : _ctx.sources.getAllFiles()) {
        if (!fn(sourceFile)) {
            success = false;
            if (stopOnFail) {
                break;
            }
        }
    }
    return success;
}
template<typename Fn>
inline bool Compilation::runForEachUnit(Fn fn, bool stopOnFail) {
    bool success = true;
    for (compilation::TranslationUnit* unit : _ctx.units.getAllUnits()) {
        if (!fn(unit)) {
            success = false;
            if (stopOnFail) {
                break;
            }
        }
    }
    return success;
}

} // namespace compilation
VEEC_NAMESPACE_END
