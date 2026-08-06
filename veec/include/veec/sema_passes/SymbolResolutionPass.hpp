/**
 * @file SymbolResolutionPass.hpp
 * @brief This file contains the definition of the SymbolResolutionPass class,
 * which is responsible for resolving all symbols in the program.
 */

#pragma once

#include <stack>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/Scope.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema_passes {

/**
 * @class SymbolResolutionPass
 * @brief A semantic analysis pass that resolves all symbols in the program.
 */
class SymbolResolutionPass : public sema::Pass {
public:
    /**
     * @brief Creates a new SymbolResolutionPass instance with the given context.
     * @param ctx The CompilationContext to use for this pass.
     * @param sema The SemaContext to use for this pass.
     */
    SymbolResolutionPass(compilation::CompilationContext& ctx, sema::SemaContext& sema)
        : sema::Pass(ctx, sema) {}
        
    virtual ~SymbolResolutionPass() = default;

    /**
     * @brief Runs the symbol resolution pass over a given module node.
     * @param node The CompilationUnitNode AST node to run this pass on.
     */
    virtual void run(ast::CompilationUnitNode& node) override {
        walk(node);
    }

protected:
    virtual void visitCompilationUnit(ast::CompilationUnitNode& node) override;
    virtual void visitBlockStmt(ast::BlockStmtNode& node) override;
    virtual void visitFunctionDecl(ast::FunctionDeclNode& node) override;

    virtual void visitNameExpr(ast::NameExprNode& node) override;

private:
    sema::Scope* _currentScope = nullptr;

    void resolveQualifiedName(ast::QualifiedNameNode& node);
    symbols::Symbol* lookupNameInScope(basic::StringId nameId, sema::Scope* scope);
};

} // namespace sema_passes
VEEC_NAMESPACE_END
