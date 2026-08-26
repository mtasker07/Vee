/**
 * @file AstToMirLowerer.hpp
 * @brief This file contains the definition of the AstToMirLowerer class.
 * The AstToMirLowerer class is responsible for lowering the AST to MIR.
 */

#pragma once

#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirBuilder.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mirgen {

class AstToMirLowerer : private ast::ConstAstWalker {
public:
    AstToMirLowerer(compilation::CompilationContext& ctx)
        : _ctx(ctx), _mir(ctx.mir), _builder(ctx) {}

    ~AstToMirLowerer() = default;

    /**
     * @brief Lowers the given AST node to MIR.
     * @param node The AST node to lower.
     * @return A pointer to the resulting MIR module.
     */
    mir::Module* lower(ast::CompilationUnitNode& node);    

private:
    compilation::CompilationContext& _ctx;
    mir::MirContext& _mir;
    mir::MirBuilder _builder;

    mir::Module* _currentModule = nullptr;
    mir::Function* _currentFunction = nullptr;

    // The last value produced by an expression, used for returning values from expressions
    mir::Value* _lastValue = nullptr;

    std::unordered_map<const symbols::FunctionSymbol*, mir::Function*> _functionMap;
    std::unordered_map<const symbols::VariableSymbol*, mir::Value*> _variableMap;

    // Entry point
    void lowerAllFunctions(const ast::AstNode& node);

    //
    // Basic
    //
    
    void visitCompilationUnit(const ast::CompilationUnitNode& node) override;
    //void visitItem(const ast::ItemNode& node) override;
    //void visitQualifiedName(const ast::QualifiedNameNode& node) override;
    //void visitGenericArgs(const ast::GenericArgsNode& node) override;

    //
    // Expressions
    //

    //void visitExpression(const ast::ExpressionNode& node) override;
    void visitParenthesizedExpr(const ast::ParenthesizedExprNode& node) override;
    //void visitLiteralExpr(const ast::LiteralExprNode& node) override;
    void visitIntLiteralExpr(const ast::IntLiteralExprNode& node) override;
    void visitFloatLiteralExpr(const ast::FloatLiteralExprNode& node) override;
    void visitStringLiteralExpr(const ast::StringLiteralExprNode& node) override;
    void visitBoolLiteralExpr(const ast::BoolLiteralExprNode& node) override;
    void visitUnaryExpr(const ast::UnaryExprNode& node) override;
    void visitBinaryExpr(const ast::BinaryExprNode& node) override;
    void visitAssignmentExpr(const ast::AssignmentExprNode& node) override;
    void visitNameExpr(const ast::NameExprNode& node) override;
    void visitCallExpr(const ast::CallExprNode& node) override;
    void visitIndexExpr(const ast::IndexExprNode& node) override;
    void visitMemberAccessExpr(const ast::MemberAccessExprNode& node) override;
    void visitConstructExpr(const ast::ConstructExprNode& node) override;
    
    //
    // Declarations
    //

    //void visitDeclaration(const ast::DeclarationNode& node) override;
    //void visitModuleDecl(const ast::ModuleDeclNode& node) override;
    void visitFunctionDecl(const ast::FunctionDeclNode& node) override;
    void visitParameterDecl(const ast::ParameterDeclNode& node) override;
    //void visitClassDecl(const ast::ClassDeclNode& node) override;
    //void visitMethodDecl(const ast::MethodDeclNode& node) override;
    //void visitFieldDecl(const ast::FieldDeclNode& node) override;
    void visitVariableDecl(const ast::VariableDeclNode& node) override;

    //
    // Statements
    //
    
    //void visitStatement(const ast::StatementNode& node) override;
    void visitBlockStmt(const ast::BlockStmtNode& node) override;
    void visitExpressionStmt(const ast::ExpressionStmtNode& node) override;
    void visitIfStmt(const ast::IfStmtNode& node) override;
    void visitLoopStmt(const ast::LoopStmtNode& node) override;
    void visitWhileStmt(const ast::WhileStmtNode& node) override;
    void visitForStmt(const ast::ForStmtNode& node) override;
    void visitReturnStmt(const ast::ReturnStmtNode& node) override;
    //void visitUseStmt(const UseStmtNode& node) override;
    //void visitType(const TypeNode& node) override;
    //void visitBuiltinType(const BuiltinTypeNode& node) override;
    //void visitNamedType(const NamedTypeNode& node) override;

    // Functions
    void lowerFunctionDecl(const ast::FunctionDeclNode& node);
    void lowerFunctionBody(const ast::FunctionDeclNode& node);
    // Blocks
    void beginBlock(const ast::BlockStmtNode& node);
    void finishBlock();

    //
    // Expression lowering
    //

    // Shorthand for _lastValue = value; just makes it more clear since we
    // cant use actual function returns (walker is void)
    void setLoweredValue(mir::Value* value);
    mir::Value* lowerExpression(const ast::ExpressionNode& node);
    std::vector<mir::Value*> lowerExpressionList(const std::vector<ast::ExpressionNode*>& nodes);

    //
    // Conversions / casts
    //

    std::vector<mir::Value*> lowerExpressionListWithConversions(
        const std::vector<mir::Value*>& values,
        const std::vector<types::Type*>& toTypes
    );
    mir::Value* handleAnyConversion(mir::Value* value, types::Type* to);
    void validateConversion(types::Type* from, types::Type* to);
    mir::Value* lowerConversion(mir::Value* value, types::Type* to);
};

} // namespace mirgen
VEEC_NAMESPACE_END
