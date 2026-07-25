/**
 * @file TopLevelUseResolutionPass.hpp
 * @brief This file contains the definition of the TopLevelUseResolutionPass class,
 * which is responsible for resolving all top-level use statements in the program.
 */

#pragma once

#include <stack>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/Scope.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast_passes {

/**
 * @class TopLevelUseResolutionPass
 * @brief A semantic analysis pass that resolves all top-level use statements in the program.
 */
class TopLevelUseResolutionPass : public sema::Pass {
public:
    /**
     * @brief Creates a new TopLevelUseResolutionPass instance with the given context.
     * @param ctx The CompilationContext to use for this pass.
     * @param sema The SemaContext to use for this pass.
     */
    TopLevelUseResolutionPass(CompilationContext& ctx, sema::SemaContext& sema)
        : sema::Pass(ctx, sema) {}
        
    virtual ~TopLevelUseResolutionPass() = default;

    /**
     * @brief Runs the top-level use resolution pass over a given module node.
     * @param node The CompilationUnitNode AST node to run this pass on.
     */
    virtual void run(ast::CompilationUnitNode& node) override {
        walk(node);
    }

protected:
    virtual void visitCompilationUnit(ast::CompilationUnitNode& node) override;
    virtual void visitBlockStmt(ast::BlockStmtNode& node) override;

    virtual void visitUseStmt(ast::UseStmtNode& node) override;

private:
    sema::Scope* _currentScope = nullptr;

    void resolveImports(ast::UseStmtNode& node);
};

} // namespace ast_passes
VEEC_NAMESPACE_END
