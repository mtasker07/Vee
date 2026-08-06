/**
 * @file Compilation.hpp
 * @brief This file contains the main interface for the vee compiler.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/compilation/CompilationConfig.hpp"
#include "veec/fs/Path.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/source/SourceFileId.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

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
        : _config(cfg), _ctx() {}

    /**
     * @brief Gets the source manager for this compilation. Use this for adding
     * your source files to the compilation process.
     * @return A reference to the source manager.
     */
    inline source::SourceManager& sources() { return _ctx.sources; }

    /**
     * @brief Runs the compilation process.
     * @return A CompilationResult containing the success
     * status and any diagnostics generated during compilation.
     */
    CompilationResult compile();

private:
    CompilationConfig _config;
    CompilationContext _ctx;

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
    for (source::SourceFileId fileId : _ctx.sources.getAllFileIds()) {
        if (!fn(fileId)) {
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
    for (TranslationUnit* unit : _ctx.units.getAllUnits()) {
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
