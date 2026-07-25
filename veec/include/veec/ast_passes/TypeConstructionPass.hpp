/**
 * @file TypeConstructionPass.hpp
 * @brief This file contains the definition of the TypeConstructionPass class,
 * which is responsible for constructing all types in the program and
 * adding them to the type table.
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
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast_passes {

/**
 * @class TypeConstructionPass
 * @brief A semantic analysis pass that constructs all types in the program.
 */
class TypeConstructionPass : public sema::Pass {
public:
    /**
     * @brief Creates a new TypeConstructionPass instance with the given context.
     * @param ctx The CompilationContext to use for this pass.
     * @param sema The SemaContext to use for this pass.
     */
    TypeConstructionPass(CompilationContext& ctx, sema::SemaContext& sema)
        : sema::Pass(ctx, sema) {}
        
    virtual ~TypeConstructionPass() = default;

    /**
     * @brief Runs the type construction pass over a given module node.
     * @param node The CompilationUnitNode AST node to run this pass on.
     */
    virtual void run(ast::CompilationUnitNode& node) override {
        walk(node);
    }

protected:
    virtual void visitClassDecl(ast::ClassDeclNode& node) override;
    virtual void visitMethodDecl(ast::MethodDeclNode& node) override;
    virtual void visitFieldDecl(ast::FieldDeclNode& node) override;

private:
    symbols::ClassSymbol* _currentClass = nullptr;
};

} // namespace ast_passes
VEEC_NAMESPACE_END
