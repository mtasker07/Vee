/**
 * @file TypeCheckerPass.hpp
 * @brief This file contains the definition of the TypeCheckerPass class,
 * which is responsible for type checking all expressions and statements in the program.
 */

#pragma once

#include <span>
#include <vector>

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
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast_passes {

/**
 * @class TypeCheckerPass
 * @brief A semantic analysis pass that type checks all expressions and statements in the program.
 */
class TypeCheckerPass : public sema::Pass {
public:
    /**
     * @brief Creates a new TypeCheckerPass instance with the given context.
     * @param ctx The CompilationContext to use for this pass.
     * @param sema The SemaContext to use for this pass.
     */
    TypeCheckerPass(CompilationContext& ctx, sema::SemaContext& sema)
        : sema::Pass(ctx, sema) {}
        
    virtual ~TypeCheckerPass() = default;

    /**
     * @brief Runs the type checker pass over a given module node.
     * @param node The CompilationUnitNode AST node to run this pass on.
     */
    virtual void run(ast::CompilationUnitNode& node) override {
        walk(node);
    }

protected:
    virtual void visitExpression(ast::ExpressionNode& node) override;
    virtual void visitParenthesizedExpr(ast::ParenthesizedExprNode& node) override;
    virtual void visitIntLiteralExpr(ast::IntLiteralExprNode& node) override;
    virtual void visitFloatLiteralExpr(ast::FloatLiteralExprNode& node) override;
    virtual void visitStringLiteralExpr(ast::StringLiteralExprNode& node) override;
    virtual void visitBoolLiteralExpr(ast::BoolLiteralExprNode& node) override;
    virtual void visitUnaryExpr(ast::UnaryExprNode& node) override;
    virtual void visitBinaryExpr(ast::BinaryExprNode& node) override;
    virtual void visitAssignmentExpr(ast::AssignmentExprNode& node) override;
    virtual void visitNameExpr(ast::NameExprNode& node) override;
    virtual void visitCallExpr(ast::CallExprNode& node) override;
    virtual void visitIndexExpr(ast::IndexExprNode& node) override;
    virtual void visitMemberAccessExpr(ast::MemberAccessExprNode& node) override;
    virtual void visitConstructExpr(ast::ConstructExprNode& node) override;

private:
    //
    // Overload resolution
    //

    u32 implicitConversionCost(types::Type* from, types::Type* to);

    template<typename T, typename CostFn>
    std::vector<T*> findBestCandidates(std::span<T* const> candidates, CostFn&& costFn);

    //
    // Enum converters
    //

    symbols::UnaryOperatorKind astToSemaUnaryOp(ast::UnaryOp op);
    symbols::BinaryOperatorKind astToSemaBinaryOp(ast::BinaryOp op);

    //
    // Lookup helpers
    //
    
    std::vector<symbols::OperatorSymbol*> lookupUnaryOperator(symbols::UnaryOperatorKind kind, types::Type* operandType);
    std::vector<symbols::OperatorSymbol*> lookupBinaryOperator(symbols::BinaryOperatorKind kind, types::Type* lhsType, types::Type* rhsType);
    std::vector<symbols::FunctionSymbol*> lookupOverload(symbols::FunctionSetSymbol* set, const std::vector<types::Type*>& argTypes);

    // 
    // Diagnostic helpers
    //
    
    void emitImplicitConversionDiagnostics(
        const source::SourceRange& range,
        types::Type* fromType,
        types::Type* toType
    );
};

} // namespace ast_passes
VEEC_NAMESPACE_END
