/**
 * @file TypeResolutionPass.hpp
 * @brief This file contains the definition of the TypeResolutionPass class,
 * which is responsible for resolving all explicit types in the program and
 * assigning that information to the declaration symbols where necessary
 * so that it can be used in later passes.
 */

#pragma once

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
 * @class TypeResolutionPass
 * @brief A semantic analysis pass that resolves all explicit types in the program.
 */
class TypeResolutionPass : public sema::Pass {
public:
    /**
     * @brief Creates a new TypeResolutionPass instance with the given context.
     * @param ctx The CompilationContext to use for this pass.
     * @param sema The SemaContext to use for this pass.
     */
    TypeResolutionPass(CompilationContext& ctx, sema::SemaContext& sema)
        : sema::Pass(ctx, sema) {}
        
    virtual ~TypeResolutionPass() = default;

    /**
     * @brief Runs the type resolution pass over a given module node.
     * @param node The CompilationUnitNode AST node to run this pass on.
     */
    virtual void run(ast::CompilationUnitNode& node) override {
        walk(node);
    }

protected:
    // Declaration type resolution
    virtual void visitFunctionDecl(ast::FunctionDeclNode& node) override;
    virtual void visitParameterDecl(ast::ParameterDeclNode& node) override;
    virtual void visitClassDecl(ast::ClassDeclNode& node) override;
    virtual void visitMethodDecl(ast::MethodDeclNode& node) override;
    virtual void visitFieldDecl(ast::FieldDeclNode& node) override;
    virtual void visitVariableDecl(ast::VariableDeclNode& node) override;

    // Concrete type resolution
    virtual void visitType(ast::TypeNode& node) override;
    virtual void visitBuiltinType(ast::BuiltinTypeNode& node) override;
    virtual void visitNamedType(ast::NamedTypeNode& node) override;
};

} // namespace ast_passes
VEEC_NAMESPACE_END
