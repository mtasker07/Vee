#include "veec/ast/AstWalker.hpp"

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Token.hpp"
#include "veec/ast/AstKind.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/ast/ItemNode.hpp"
#include "veec/ast/name/QualifiedNameNode.hpp"
#include "veec/ast/generic/GenericArgsNode.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"
#include "veec/ast/expr/ParenthesizedExprNode.hpp"
#include "veec/ast/expr/LiteralExprNode.hpp"
#include "veec/ast/expr/IntLiteralExprNode.hpp"
#include "veec/ast/expr/FloatLiteralExprNode.hpp"
#include "veec/ast/expr/StringLiteralExprNode.hpp"
#include "veec/ast/expr/BoolLiteralExprNode.hpp"
#include "veec/ast/expr/UnaryExprNode.hpp"
#include "veec/ast/expr/BinaryExprNode.hpp"
#include "veec/ast/expr/AssignmentExprNode.hpp"
#include "veec/ast/expr/NameExprNode.hpp"
#include "veec/ast/expr/CallExprNode.hpp"
#include "veec/ast/expr/IndexExprNode.hpp"
#include "veec/ast/expr/MemberAccessExprNode.hpp"
#include "veec/ast/expr/ConstructExprNode.hpp"
#include "veec/ast/decl/DeclarationNode.hpp"
#include "veec/ast/decl/CallableDeclNode.hpp"
#include "veec/ast/decl/ModuleDeclNode.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/ParameterDeclNode.hpp"
#include "veec/ast/decl/ClassDeclNode.hpp"
#include "veec/ast/decl/MethodDeclNode.hpp"
#include "veec/ast/decl/FieldDeclNode.hpp"
#include "veec/ast/decl/VariableDeclNode.hpp"
#include "veec/ast/stmt/StatementNode.hpp"
#include "veec/ast/stmt/BlockStmtNode.hpp"
#include "veec/ast/stmt/ExpressionStmtNode.hpp"
#include "veec/ast/stmt/IfStmtNode.hpp"
#include "veec/ast/stmt/LoopStmtNode.hpp"
#include "veec/ast/stmt/WhileStmtNode.hpp"
#include "veec/ast/stmt/ForStmtNode.hpp"
#include "veec/ast/stmt/ReturnStmtNode.hpp"
#include "veec/ast/stmt/UseStmtNode.hpp"
#include "veec/ast/type/TypeNode.hpp"
#include "veec/ast/type/BuiltinTypeNode.hpp"
#include "veec/ast/type/NamedTypeNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

void AstWalker::walk(AstNode& node) {
    switch (node.getNodeKind()) {
        case AstKind::CompilationUnit:
            visitCompilationUnit(static_cast<CompilationUnitNode&>(node));
            break;

        case AstKind::QualifiedName:
            visitQualifiedName(static_cast<QualifiedNameNode&>(node));
            break;
        
        case AstKind::GenericArgs:
            visitGenericArgs(static_cast<GenericArgsNode&>(node));
            break;

        case AstKind::ParenthesizedExpr:
            visitParenthesizedExpr(static_cast<ParenthesizedExprNode&>(node));
            break;

        case AstKind::IntLiteralExpr:
            visitIntLiteralExpr(static_cast<IntLiteralExprNode&>(node));
            break;

        case AstKind::FloatLiteralExpr:
            visitFloatLiteralExpr(static_cast<FloatLiteralExprNode&>(node));
            break;

        case AstKind::StringLiteralExpr:
            visitStringLiteralExpr(static_cast<StringLiteralExprNode&>(node));
            break;

        case AstKind::BoolLiteralExpr:
            visitBoolLiteralExpr(static_cast<BoolLiteralExprNode&>(node));
            break;

        case AstKind::UnaryExpr:
            visitUnaryExpr(static_cast<UnaryExprNode&>(node));
            break;

        case AstKind::BinaryExpr:
            visitBinaryExpr(static_cast<BinaryExprNode&>(node));
            break;

        case AstKind::AssignmentExpr:
            visitAssignmentExpr(static_cast<AssignmentExprNode&>(node));
            break;

        case AstKind::NameExpr:
            visitNameExpr(static_cast<NameExprNode&>(node));
            break;

        case AstKind::CallExpr:
            visitCallExpr(static_cast<CallExprNode&>(node));
            break;

        case AstKind::IndexExpr:
            visitIndexExpr(static_cast<IndexExprNode&>(node));
            break;

        case AstKind::MemberAccessExpr:
            visitMemberAccessExpr(static_cast<MemberAccessExprNode&>(node));
            break;

        case AstKind::ConstructExpr:
            visitConstructExpr(static_cast<ConstructExprNode&>(node));
            break;

        case AstKind::ModuleDecl:
            visitModuleDecl(static_cast<ModuleDeclNode&>(node));
            break;

        case AstKind::FunctionDecl:
            visitFunctionDecl(static_cast<FunctionDeclNode&>(node));
            break;

        case AstKind::ParameterDecl:
            visitParameterDecl(static_cast<ParameterDeclNode&>(node));
            break;

        case AstKind::ClassDecl:
            visitClassDecl(static_cast<ClassDeclNode&>(node));
            break;

        case AstKind::MethodDecl:
            visitMethodDecl(static_cast<MethodDeclNode&>(node));
            break;

        case AstKind::FieldDecl:
            visitFieldDecl(static_cast<FieldDeclNode&>(node));
            break;

        case AstKind::VariableDecl:
            visitVariableDecl(static_cast<VariableDeclNode&>(node));
            break;

        case AstKind::BlockStmt:
            visitBlockStmt(static_cast<BlockStmtNode&>(node));
            break;

        case AstKind::ExpressionStmt:
            visitExpressionStmt(static_cast<ExpressionStmtNode&>(node));
            break;

        case AstKind::IfStmt:
            visitIfStmt(static_cast<IfStmtNode&>(node));
            break;

        case AstKind::LoopStmt:
            visitLoopStmt(static_cast<LoopStmtNode&>(node));
            break;

        case AstKind::WhileStmt:
            visitWhileStmt(static_cast<WhileStmtNode&>(node));
            break;

        case AstKind::ForStmt:
            visitForStmt(static_cast<ForStmtNode&>(node));
            break;

        case AstKind::ReturnStmt:
            visitReturnStmt(static_cast<ReturnStmtNode&>(node));
            break;
        
        case AstKind::UseStmt:
            visitUseStmt(static_cast<UseStmtNode&>(node));
            break;

        case AstKind::BuiltinType:
            visitBuiltinType(static_cast<BuiltinTypeNode&>(node));
            break;

        case AstKind::NamedType:
            visitNamedType(static_cast<NamedTypeNode&>(node));
            break;

        default:
            VEE_UNREACHABLE("Unknown AST node kind: {}", std::to_string(int(node.getNodeKind())));
    }
}

void AstWalker::visitCompilationUnit(CompilationUnitNode& node) {
    for (auto& item : node.getItems()) {
        walk(*item);
    }
}
void AstWalker::visitItem(ItemNode&) {
    // Nothing to walk
}
void AstWalker::visitQualifiedName(QualifiedNameNode& node) {
    for (auto& segment : node.getSegments()) {
        if (segment.genericArgs)
            walk(*segment.genericArgs);
    }
}
void AstWalker::visitGenericArgs(GenericArgsNode& node) {
    for (auto& arg : node.getArgs()) {
        walk(*arg);
    }
}
void AstWalker::visitExpression(ExpressionNode&) {
    // Nothing to walk
}
void AstWalker::visitParenthesizedExpr(ParenthesizedExprNode& node) {
    visitExpression(node);
    if (node.getInnerExpr())
        walk(*node.getInnerExpr());
}
void AstWalker::visitLiteralExpr(LiteralExprNode& node) {
    visitExpression(node);
}
void AstWalker::visitIntLiteralExpr(IntLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void AstWalker::visitFloatLiteralExpr(FloatLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void AstWalker::visitStringLiteralExpr(StringLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void AstWalker::visitBoolLiteralExpr(BoolLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void AstWalker::visitUnaryExpr(UnaryExprNode& node) {
    visitExpression(node);
    walk(*node.getOperand());
}
void AstWalker::visitBinaryExpr(BinaryExprNode& node) {
    visitExpression(node);
    walk(*node.getLeft());
    walk(*node.getRight());
}
void AstWalker::visitAssignmentExpr(AssignmentExprNode& node) {
    visitExpression(node);
    walk(*node.getLeft());
    walk(*node.getRight());
}
void AstWalker::visitNameExpr(NameExprNode& node) {
    visitExpression(node);
    walk(node.getQualifiedName());
}
void AstWalker::visitCallExpr(CallExprNode& node) {
    visitExpression(node);
    if (node.getCallee())
        walk(*node.getCallee());
    for (auto& arg : node.getArgs()) {
        walk(*arg);
    }
}
void AstWalker::visitIndexExpr(IndexExprNode& node) {
    visitExpression(node);
    walk(*node.getObject());
    walk(*node.getIndex());
}
void AstWalker::visitMemberAccessExpr(MemberAccessExprNode& node) {
    visitExpression(node);
    walk(*node.getObject());
}
void AstWalker::visitConstructExpr(ConstructExprNode& node) {
    visitExpression(node);
    if (node.getType())
        walk(*node.getType());
    for (auto& arg : node.getArgs()) {
        walk(*arg);
    }
}
void AstWalker::visitDeclaration(DeclarationNode& node) {
    visitItem(node);
}
void AstWalker::visitCallableDecl(CallableDeclNode& node) {
    visitDeclaration(node);
    if (node.getReturnType())
        walk(*node.getReturnType());

    if (node.getGenericParams())
        walk(*node.getGenericParams());

    for (auto& param : node.getParameters()) {
        walk(*param);
    }

    if (node.getBody())
        walk(*node.getBody());
}
void AstWalker::visitModuleDecl(ModuleDeclNode& node) {
    visitDeclaration(node);
    for (auto& item : node.getItems()) {
        walk(*item);
    }
}
void AstWalker::visitFunctionDecl(FunctionDeclNode& node) {
    visitCallableDecl(node);
}
void AstWalker::visitParameterDecl(ParameterDeclNode& node) {
    visitDeclaration(node);
    if (node.getType())
        walk(*node.getType());
}
void AstWalker::visitClassDecl(ClassDeclNode& node) {
    visitDeclaration(node);
    for (auto& item : node.getItems()) {
        walk(*item);
    }
}
void AstWalker::visitMethodDecl(MethodDeclNode& node) {
    visitCallableDecl(node);
}
void AstWalker::visitFieldDecl(FieldDeclNode& node) {
    visitDeclaration(node);
    if (node.getType())
        walk(*node.getType());
}
void AstWalker::visitVariableDecl(VariableDeclNode& node) {
    visitDeclaration(node);
    if (node.getType())
        walk(*node.getType());
    if (node.getInitializer())
        walk(*node.getInitializer());
}
void AstWalker::visitStatement(StatementNode& node) {
    visitItem(node);
}
void AstWalker::visitBlockStmt(BlockStmtNode& node) {
    visitStatement(node);
    for (auto& item : node.getItems()) {
        walk(*item);
    }
}
void AstWalker::visitExpressionStmt(ExpressionStmtNode& node) {
    visitStatement(node);
    if (node.getExpression())
        walk(*node.getExpression());
}
void AstWalker::visitIfStmt(IfStmtNode& node) {
    visitStatement(node);
    if (node.getCondition())
        walk(*node.getCondition());
    if (node.getThen())
        walk(*node.getThen());
    if (node.getElse())
        walk(*node.getElse());
}
void AstWalker::visitLoopStmt(LoopStmtNode& node) {
    visitStatement(node);
    if (node.getBody())
        walk(*node.getBody());
}
void AstWalker::visitWhileStmt(WhileStmtNode& node) {
    visitStatement(node);
    if (node.getCondition())
        walk(*node.getCondition());
    if (node.getBody())
        walk(*node.getBody());
}
void AstWalker::visitForStmt(ForStmtNode& node) {
    visitStatement(node);
    if (node.getInit())
        walk(*node.getInit());
    if (node.getCondition())
        walk(*node.getCondition());
    if (node.getIncrement())
        walk(*node.getIncrement());
    if (node.getBody())
        walk(*node.getBody());
}
void AstWalker::visitReturnStmt(ReturnStmtNode& node) {
    visitStatement(node);
    if (node.getValue())
        walk(*node.getValue());
}
void AstWalker::visitUseStmt(UseStmtNode& node) {
    visitStatement(node);
}
void AstWalker::visitType(TypeNode&) {
    // Nothing to walk
}
void AstWalker::visitBuiltinType(BuiltinTypeNode& node) {
    visitType(node);
}
void AstWalker::visitNamedType(NamedTypeNode& node) {
    visitType(node);
}

void ConstAstWalker::walk(const AstNode& node) {
    switch (node.getNodeKind()) {
        case AstKind::CompilationUnit:
            visitCompilationUnit(static_cast<const CompilationUnitNode&>(node));
            break;

        case AstKind::QualifiedName:
            visitQualifiedName(static_cast<const QualifiedNameNode&>(node));
            break;

        case AstKind::GenericArgs:
            visitGenericArgs(static_cast<const GenericArgsNode&>(node));
            break;

        case AstKind::ParenthesizedExpr:
            visitParenthesizedExpr(static_cast<const ParenthesizedExprNode&>(node));
            break;

        case AstKind::IntLiteralExpr:
            visitIntLiteralExpr(static_cast<const IntLiteralExprNode&>(node));
            break;

        case AstKind::FloatLiteralExpr:
            visitFloatLiteralExpr(static_cast<const FloatLiteralExprNode&>(node));
            break;

        case AstKind::StringLiteralExpr:
            visitStringLiteralExpr(static_cast<const StringLiteralExprNode&>(node));
            break;

        case AstKind::BoolLiteralExpr:
            visitBoolLiteralExpr(static_cast<const BoolLiteralExprNode&>(node));
            break;

        case AstKind::UnaryExpr:
            visitUnaryExpr(static_cast<const UnaryExprNode&>(node));
            break;

        case AstKind::BinaryExpr:
            visitBinaryExpr(static_cast<const BinaryExprNode&>(node));
            break;

        case AstKind::AssignmentExpr:
            visitAssignmentExpr(static_cast<const AssignmentExprNode&>(node));
            break;

        case AstKind::NameExpr:
            visitNameExpr(static_cast<const NameExprNode&>(node));
            break;

        case AstKind::CallExpr:
            visitCallExpr(static_cast<const CallExprNode&>(node));
            break;

        case AstKind::IndexExpr:
            visitIndexExpr(static_cast<const IndexExprNode&>(node));
            break;

        case AstKind::MemberAccessExpr:
            visitMemberAccessExpr(static_cast<const MemberAccessExprNode&>(node));
            break;

        case AstKind::ConstructExpr:
            visitConstructExpr(static_cast<const ConstructExprNode&>(node));
            break;

        case AstKind::ModuleDecl:
            visitModuleDecl(static_cast<const ModuleDeclNode&>(node));
            break;

        case AstKind::FunctionDecl:
            visitFunctionDecl(static_cast<const FunctionDeclNode&>(node));
            break;

        case AstKind::ParameterDecl:
            visitParameterDecl(static_cast<const ParameterDeclNode&>(node));
            break;

        case AstKind::ClassDecl:
            visitClassDecl(static_cast<const ClassDeclNode&>(node));
            break;

        case AstKind::MethodDecl:
            visitMethodDecl(static_cast<const MethodDeclNode&>(node));
            break;

        case AstKind::FieldDecl:
            visitFieldDecl(static_cast<const FieldDeclNode&>(node));
            break;

        case AstKind::VariableDecl:
            visitVariableDecl(static_cast<const VariableDeclNode&>(node));
            break;

        case AstKind::BlockStmt:
            visitBlockStmt(static_cast<const BlockStmtNode&>(node));
            break;

        case AstKind::ExpressionStmt:
            visitExpressionStmt(static_cast<const ExpressionStmtNode&>(node));
            break;

        case AstKind::IfStmt:
            visitIfStmt(static_cast<const IfStmtNode&>(node));
            break;

        case AstKind::LoopStmt:
            visitLoopStmt(static_cast<const LoopStmtNode&>(node));
            break;

        case AstKind::WhileStmt:
            visitWhileStmt(static_cast<const WhileStmtNode&>(node));
            break;

        case AstKind::ForStmt:
            visitForStmt(static_cast<const ForStmtNode&>(node));
            break;

        case AstKind::ReturnStmt:
            visitReturnStmt(static_cast<const ReturnStmtNode&>(node));
            break;

        case AstKind::UseStmt:
            visitUseStmt(static_cast<const UseStmtNode&>(node));
            break;

        case AstKind::BuiltinType:
            visitBuiltinType(static_cast<const BuiltinTypeNode&>(node));
            break;

        case AstKind::NamedType:
            visitNamedType(static_cast<const NamedTypeNode&>(node));
            break;

        default:
            VEE_UNREACHABLE("Unknown AST node kind: {}", std::to_string(int(node.getNodeKind())));
    }
}

void ConstAstWalker::visitCompilationUnit(const CompilationUnitNode& node) {
    for (const auto& item : node.getItems()) {
        walk(*item);
    }
}
void ConstAstWalker::visitItem(const ItemNode&) {
    // Nothing to walk
}
void ConstAstWalker::visitQualifiedName(const QualifiedNameNode& node) {
    for (const auto& segment : node.getSegments()) {
        if (segment.genericArgs)
            walk(*segment.genericArgs);
    }
}
void ConstAstWalker::visitGenericArgs(const GenericArgsNode& node) {
    for (const auto& arg : node.getArgs()) {
        walk(*arg);
    }
}
void ConstAstWalker::visitExpression(const ExpressionNode&) {
    // Nothing to walk
}
void ConstAstWalker::visitParenthesizedExpr(const ParenthesizedExprNode& node) {
    visitExpression(node);
    if (node.getInnerExpr())
        walk(*node.getInnerExpr());
}
void ConstAstWalker::visitLiteralExpr(const LiteralExprNode& node) {
    visitExpression(node);
}
void ConstAstWalker::visitIntLiteralExpr(const IntLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void ConstAstWalker::visitFloatLiteralExpr(const FloatLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void ConstAstWalker::visitStringLiteralExpr(const StringLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void ConstAstWalker::visitBoolLiteralExpr(const BoolLiteralExprNode& node) {
    visitLiteralExpr(node);
}
void ConstAstWalker::visitUnaryExpr(const UnaryExprNode& node) {
    visitExpression(node);
    walk(*node.getOperand());
}
void ConstAstWalker::visitBinaryExpr(const BinaryExprNode& node) {
    visitExpression(node);
    walk(*node.getLeft());
    walk(*node.getRight());
}
void ConstAstWalker::visitAssignmentExpr(const AssignmentExprNode& node) {
    walk(*node.getLeft());
    walk(*node.getRight());
}
void ConstAstWalker::visitNameExpr(const NameExprNode& node) {
    visitExpression(node);
    walk(node.getQualifiedName());
}
void ConstAstWalker::visitCallExpr(const CallExprNode& node) {
    visitExpression(node);
    walk(*node.getCallee());
    for (const auto& arg : node.getArgs()) {
        walk(*arg);
    }
}
void ConstAstWalker::visitIndexExpr(const IndexExprNode& node) {
    visitExpression(node);
    walk(*node.getObject());
    walk(*node.getIndex());
}
void ConstAstWalker::visitMemberAccessExpr(const MemberAccessExprNode& node) {
    visitExpression(node);
    walk(*node.getObject());
}
void ConstAstWalker::visitConstructExpr(const ConstructExprNode& node) {
    visitExpression(node);
    if (node.getType())
        walk(*node.getType());
    for (const auto& arg : node.getArgs()) {
        walk(*arg);
    }
}
void ConstAstWalker::visitDeclaration(const DeclarationNode& node) {
    visitItem(node);
}
void ConstAstWalker::visitModuleDecl(const ModuleDeclNode& node) {
    visitDeclaration(node);
    for (const auto& item : node.getItems()) {
        walk(*item);
    }
}
void ConstAstWalker::visitFunctionDecl(const FunctionDeclNode& node) {
    visitDeclaration(node);
    if (node.getReturnType())
        walk(*node.getReturnType());

    for (const auto& param : node.getParameters()) {
        walk(*param);
    }

    if (node.getBody())
        walk(*node.getBody());
}
void ConstAstWalker::visitParameterDecl(const ParameterDeclNode& node) {
    visitDeclaration(node);
    if (node.getType())
        walk(*node.getType());
}
void ConstAstWalker::visitClassDecl(const ClassDeclNode& node) {
    visitDeclaration(node);
    for (const auto& item : node.getItems()) {
        walk(*item);
    }
}
void ConstAstWalker::visitMethodDecl(const MethodDeclNode& node) {
    visitDeclaration(node);
    if (node.getReturnType())
        walk(*node.getReturnType());

    for (const auto& param : node.getParameters()) {
        walk(*param);
    }

    if (node.getBody())
        walk(*node.getBody());
}
void ConstAstWalker::visitFieldDecl(const FieldDeclNode& node) {
    visitDeclaration(node);
    if (node.getType())
        walk(*node.getType());
}
void ConstAstWalker::visitVariableDecl(const VariableDeclNode& node) {
    visitDeclaration(node);
    if (node.getType())
        walk(*node.getType());
    if (node.getInitializer())
        walk(*node.getInitializer());
}
void ConstAstWalker::visitStatement(const StatementNode& node) {
    visitItem(node);
}
void ConstAstWalker::visitBlockStmt(const BlockStmtNode& node) {
    visitStatement(node);
    for (const auto& item : node.getItems()) {
        walk(*item);
    }
}
void ConstAstWalker::visitExpressionStmt(const ExpressionStmtNode& node) {
    visitStatement(node);
    if (node.getExpression())
        walk(*node.getExpression());
}
void ConstAstWalker::visitIfStmt(const IfStmtNode& node) {
    visitStatement(node);
    if (node.getCondition())
        walk(*node.getCondition());
    if (node.getThen())
        walk(*node.getThen());
    if (node.getElse())
        walk(*node.getElse());
}
void ConstAstWalker::visitLoopStmt(const LoopStmtNode& node) {
    visitStatement(node);
    if (node.getBody())
        walk(*node.getBody());
}
void ConstAstWalker::visitWhileStmt(const WhileStmtNode& node) {
    visitStatement(node);
    if (node.getCondition())
        walk(*node.getCondition());
    if (node.getBody())
        walk(*node.getBody());
}
void ConstAstWalker::visitForStmt(const ForStmtNode& node) {
    visitStatement(node);
    if (node.getInit())
        walk(*node.getInit());
    if (node.getCondition())
        walk(*node.getCondition());
    if (node.getIncrement())
        walk(*node.getIncrement());
    if (node.getBody())
        walk(*node.getBody());
}
void ConstAstWalker::visitReturnStmt(const ReturnStmtNode& node) {
    visitStatement(node);
    if (node.getValue())
        walk(*node.getValue());
}
void ConstAstWalker::visitUseStmt(const UseStmtNode& node) {
    visitStatement(node);
}
void ConstAstWalker::visitType(const TypeNode&) {
    // Nothing to walk
}
void ConstAstWalker::visitBuiltinType(const BuiltinTypeNode& node) {
    visitType(node);
}
void ConstAstWalker::visitNamedType(const NamedTypeNode& node) {
    visitType(node);
}

} // namespace ast
VEEC_NAMESPACE_END
