/**
 * @file Pass.hpp
 * @brief This file contains the definition of the semantic analysis pass class.
 * The semantic analysis pass is the base class for all semantic analysis passes
 * which are responsible for analyzing the AST and performing various checks and
 * transformations on it.
 */

#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <filesystem>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/sema/SemaContext.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

/**
 * @class Pass
 * @brief This is the base class for all semantic analysis passes which are responsible for
 * analyzing the AST and performing various checks and transformations on it.
 */
class Pass : public ast::AstWalker {
public:
    virtual ~Pass() = default;

    /**
     * @brief Runs this semantic analysis pass on the given CompilationUnit AST node.
     * This function can be overriden to provide custom behavior, for example
     * resetting internal state before running the pass.
     * 
     * We use a CompilationUnitNode here because all semantic analysis passes should
     * be run on the entire program and not individual nodes which could lead
     * to incorrect behaviour.
     */
    virtual void run(ast::CompilationUnitNode& node) {
        walk(node);
    }

protected:
    CompilationContext& _ctx;
    SemaContext& _sema;

    /**
     * @brief Creates a new Pass instance with the given context.
     * @param ctx The CompilationContext to use for this Pass.
     * @param sema The SemaContext to use for this Pass.
     */
    Pass(CompilationContext& ctx, SemaContext& sema)
        : _ctx(ctx), _sema(sema) {}
};

} // namespace sema
VEEC_NAMESPACE_END
