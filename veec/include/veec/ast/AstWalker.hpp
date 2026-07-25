/**
 * @file AstWalker.hpp
 * @brief This file contains the definition of the base AST walker
 * classes used in the Vee compiler.
 */

#pragma once

#include <memory>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/ast/AstKind.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief Base class for all AST walkers that may need to modify the AST nodes.
 */
class AstWalker {
public:
    virtual ~AstWalker() = default;

    /**
     * @brief Walks the given AST node and all of its children.
     * @param node The AST node to walk.
     */
    void walk(AstNode& node);

protected:
    /**
     * @brief Creates a new AstWalker instance.
     * @note Since AstWalker is not technically an abstract class,
     * this is protected to prevent direct instantiation.
     */
    AstWalker() = default;

    virtual void visitCompilationUnit(CompilationUnitNode& node);
    virtual void visitItem(ItemNode& node);
    virtual void visitQualifiedName(QualifiedNameNode& node);
    virtual void visitGenericArgs(GenericArgsNode& node);
    virtual void visitExpression(ExpressionNode& node);
    virtual void visitParenthesizedExpr(ParenthesizedExprNode& node);
    virtual void visitLiteralExpr(LiteralExprNode& node);
    virtual void visitIntLiteralExpr(IntLiteralExprNode& node);
    virtual void visitFloatLiteralExpr(FloatLiteralExprNode& node);
    virtual void visitStringLiteralExpr(StringLiteralExprNode& node);
    virtual void visitBoolLiteralExpr(BoolLiteralExprNode& node);
    virtual void visitUnaryExpr(UnaryExprNode& node);
    virtual void visitBinaryExpr(BinaryExprNode& node);
    virtual void visitAssignmentExpr(AssignmentExprNode& node);
    virtual void visitNameExpr(NameExprNode& node);
    virtual void visitCallExpr(CallExprNode& node);
    virtual void visitIndexExpr(IndexExprNode& node);
    virtual void visitMemberAccessExpr(MemberAccessExprNode& node);
    virtual void visitConstructExpr(ConstructExprNode& node);
    virtual void visitDeclaration(DeclarationNode& node);
    virtual void visitModuleDecl(ModuleDeclNode& node);
    virtual void visitFunctionDecl(FunctionDeclNode& node);
    virtual void visitParameterDecl(ParameterDeclNode& node);
    virtual void visitClassDecl(ClassDeclNode& node);
    virtual void visitMethodDecl(MethodDeclNode& node);
    virtual void visitFieldDecl(FieldDeclNode& node);
    virtual void visitVariableDecl(VariableDeclNode& node);
    virtual void visitStatement(StatementNode& node);
    virtual void visitBlockStmt(BlockStmtNode& node);
    virtual void visitExpressionStmt(ExpressionStmtNode& node);
    virtual void visitIfStmt(IfStmtNode& node);
    virtual void visitLoopStmt(LoopStmtNode& node);
    virtual void visitWhileStmt(WhileStmtNode& node);
    virtual void visitForStmt(ForStmtNode& node);
    virtual void visitReturnStmt(ReturnStmtNode& node);
    virtual void visitUseStmt(UseStmtNode& node);
    virtual void visitType(TypeNode& node);
    virtual void visitBuiltinType(BuiltinTypeNode& node);
    virtual void visitNamedType(NamedTypeNode& node);
};

/**
 * @brief Base class for all AST walkers that do not need to modify the AST nodes.
 */
class ConstAstWalker {
public:
    virtual ~ConstAstWalker() = default;

    /**
     * @brief Walks the given AST node and all of its children.
     * @param node The AST node to walk.
     */
    void walk(const AstNode& node);

protected:
    /**
     * @brief Creates a new ConstAstWalker.
     * @note Since ConstAstWalker is not technically an abstract class,
     * this is protected to prevent direct instantiation.
     */
    ConstAstWalker() = default;

    virtual void visitCompilationUnit(const CompilationUnitNode& node);
    virtual void visitItem(const ItemNode& node);
    virtual void visitQualifiedName(const QualifiedNameNode& node);
    virtual void visitGenericArgs(const GenericArgsNode& node);
    virtual void visitExpression(const ExpressionNode& node);
    virtual void visitParenthesizedExpr(const ParenthesizedExprNode& node);
    virtual void visitLiteralExpr(const LiteralExprNode& node);
    virtual void visitIntLiteralExpr(const IntLiteralExprNode& node);
    virtual void visitFloatLiteralExpr(const FloatLiteralExprNode& node);
    virtual void visitStringLiteralExpr(const StringLiteralExprNode& node);
    virtual void visitBoolLiteralExpr(const BoolLiteralExprNode& node);
    virtual void visitUnaryExpr(const UnaryExprNode& node);
    virtual void visitBinaryExpr(const BinaryExprNode& node);
    virtual void visitAssignmentExpr(const AssignmentExprNode& node);
    virtual void visitNameExpr(const NameExprNode& node);
    virtual void visitCallExpr(const CallExprNode& node);
    virtual void visitIndexExpr(const IndexExprNode& node);
    virtual void visitMemberAccessExpr(const MemberAccessExprNode& node);
    virtual void visitConstructExpr(const ConstructExprNode& node);
    virtual void visitDeclaration(const DeclarationNode& node);
    virtual void visitModuleDecl(const ModuleDeclNode& node);
    virtual void visitFunctionDecl(const FunctionDeclNode& node);
    virtual void visitParameterDecl(const ParameterDeclNode& node);
    virtual void visitClassDecl(const ClassDeclNode& node);
    virtual void visitMethodDecl(const MethodDeclNode& node);
    virtual void visitFieldDecl(const FieldDeclNode& node);
    virtual void visitVariableDecl(const VariableDeclNode& node);
    virtual void visitStatement(const StatementNode& node);
    virtual void visitBlockStmt(const BlockStmtNode& node);
    virtual void visitExpressionStmt(const ExpressionStmtNode& node);
    virtual void visitIfStmt(const IfStmtNode& node);
    virtual void visitLoopStmt(const LoopStmtNode& node);
    virtual void visitWhileStmt(const WhileStmtNode& node);
    virtual void visitForStmt(const ForStmtNode& node);
    virtual void visitReturnStmt(const ReturnStmtNode& node);
    virtual void visitUseStmt(const UseStmtNode& node);
    virtual void visitType(const TypeNode& node);
    virtual void visitBuiltinType(const BuiltinTypeNode& node);
    virtual void visitNamedType(const NamedTypeNode& node);
};

} // namespace ast
VEEC_NAMESPACE_END
