/**
 * @file PassManager.hpp
 * @brief This file contains the definition of the PassManager class.
 * The PassManager class is responsible for managing all semantic analysis passes on the AST.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

/**
 * @class PassManager
 * @brief Responsible for managing all semantic analysis passes over the AST.
 * Prefer this class over manually running each pass individually.
 */
class PassManager {
public:
    /**
     * @brief Creates a new PassManager instance with the given context.
     * @param ctx The context to use for all semantic analysis passes managed by this pass manager.
     */
    PassManager(CompilationContext& ctx, SemaContext& sema)
        : _ctx(ctx), _sema(sema) {}

    ~PassManager() = default;

    /**
     * @brief Adds a pass of the specified type to this semantic pass manager.
     * @tparam PassType The type of the pass to add, which must be derived
     * from Pass.
     * @param args The arguments to forward to the constructor of the pass.
     * @note This function automatically injects the context into the constructor
     * of the pass, so you don't need to provide it as an argument.
     */
    template<typename PassType, typename... Args>
    inline void addPass(Args&&... args) {
        static_assert(std::is_base_of_v<Pass, PassType>, "PassType must be derived from Pass");
        _passes.push_back(std::make_unique<PassType>(_ctx, _sema, std::forward<Args>(args)...));
    }

    /**
     * @brief Runs all passes in this semantic pass manager on the given ModuleNode AST node.
     * @param root The ModuleNode AST node to run all passes on.
     * @return True if all passes ran successfully without reporting any errors, false if any pass reported an error
     * (accounting for the current error level).
     */
    inline bool runAll(ast::CompilationUnitNode& root) {
        for (const auto& pass : _passes) {
            pass->run(root);
            if (_ctx.diagnostics.hasErrors()) {
                return false;
            }
        }
        return true;
    }

private:
    CompilationContext& _ctx;
    SemaContext& _sema;
    std::vector<std::unique_ptr<Pass>> _passes;
};

} // namespace sema
VEEC_NAMESPACE_END
