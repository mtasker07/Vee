/**
 * @file ControlFlowValidationPass.hpp
 * @brief This file contains the definition of the ControlFlowValidationPass class,
 * which is responsible for validating the control flow of the program.
 * 
 * Control flow validation mostly checks for:
 * - Break outside of loops
 * - Continue outside of loops
 * - Return outside of functions
 * - Functions returning on all paths (if non-void)
 */

#pragma once

#include <stack>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/sema/FlowInfo.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema_passes {

/**
 * @class ControlFlowValidationPass
 * @brief A semantic analysis pass that validates the control flow of the program.
 */
class ControlFlowValidationPass : public sema::Pass {
public:
    /**
     * @brief Creates a new ControlFlowValidationPass instance with the given context.
     * @param ctx The CompilationContext to use for this pass.
     * @param sema The SemaContext to use for this pass.
     */
    ControlFlowValidationPass(compilation::CompilationContext& ctx, sema::SemaContext& sema)
        : sema::Pass(ctx, sema) {}
        
    virtual ~ControlFlowValidationPass() = default;

    /**
     * @brief Runs the control flow validation pass over a given module node.
     * @param node The CompilationUnitNode AST node to run the control flow validation pass on.
     */
    virtual void run(ast::CompilationUnitNode& node) override {
        walk(node);
    }

protected:
    virtual void visitFunctionDecl(ast::FunctionDeclNode& node) override;
    virtual void visitMethodDecl(ast::MethodDeclNode& node) override;

    virtual void visitReturnStmt(ast::ReturnStmtNode& node) override;

private:
    ast::DeclarationNode* _currentFunctionOrMethod = nullptr;
    ast::BaseLoopStmtNode* _currentLoop = nullptr;

    bool functionReturnsValue(const ast::FunctionDeclNode& node);
    bool methodReturnsValue(const ast::MethodDeclNode& node);

    /// Checks if given node can fall through (i.e. does not guarantee a return or break)
    sema::FlowInfo analyzeFlowInfo(ast::AstNode& node);
};

} // namespace sema_passes
VEEC_NAMESPACE_END
