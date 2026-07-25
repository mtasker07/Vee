/**
 * @file AstKind.hpp
 * @brief Contains the AstKind enum which is used to differentiate between different kinds of AST nodes.
 * 
 * This is used for quick type identification (without dynamic_cast overhead).
 * Additionally, it makes walking significantly easier, since we don't have to
 * use double-dispatch, which requires a ton of boilerplate code and is generally
 * more of a pain to work with.
 * 
 * The name convention for the enum values is to use the name of the AST node class
 * without the "Node" suffix. For example, the AstKind for the CompilationUnitNode
 * class is AstKind::CompilationUnit.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief Used to differentiate between different kinds of AST nodes.
 */
enum class AstKind : u8 {
    CompilationUnit,

    // -- NAME --
    QualifiedName,

    // -- GENERIC --
    GenericArgs,

    // -- EXPR --
    FirstExpression,
    ParenthesizedExpr = FirstExpression,
    IntLiteralExpr,
    FloatLiteralExpr,
    StringLiteralExpr,
    BoolLiteralExpr,
    UnaryExpr,
    BinaryExpr,
    AssignmentExpr,
    NameExpr,
    CallExpr,
    IndexExpr,
    MemberAccessExpr,
    ConstructExpr,
    LastExpression = ConstructExpr,
    
    // -- DECL --
    FirstDeclaration,
    ModuleDecl = FirstDeclaration,
    FunctionDecl,
    ParameterDecl,
    ClassDecl,
    MethodDecl,
    FieldDecl,
    VariableDecl,
    LastDeclaration = VariableDecl,

    // -- STMT --
    FirstStatement,
    BlockStmt = FirstStatement,
    ExpressionStmt,
    IfStmt,
    LoopStmt,
    WhileStmt,
    ForStmt,
    ReturnStmt,
    UseStmt,
    LastStatement = UseStmt,

    // -- TYPE --
    FirstType,
    BuiltinType = FirstType,
    NamedType,
    LastType = NamedType,
};

} // namespace ast
VEEC_NAMESPACE_END
