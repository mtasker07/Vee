/**
 * @file AstFwd.hpp
 * @brief This file contains forward declarations for the AST
 * nodes used in the Vee compiler.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Token.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

// ast::Token -> basic::Token
using Token = basic::Token;
// ast::TokenType -> basic::TokenType
using TokenType = basic::TokenType;

//
// ENUMS
//

enum class AstKind : u8;
enum class UnaryOp : u8;
enum class BinaryOp : u8;
enum class AssignmentOp : u8;
enum class MemberAccessOp : u8;
enum class BuiltinTypeKind : u8;
enum class LiteralType : u8;
enum class VariableDeclKind : u8;

//
// STRUCTS
//

struct QualifiedNameSegment;

//
// NODES
//

// -- BASIC --
class AstNode;
class CompilationUnitNode;
class ItemNode;

// -- NAME --
class QualifiedNameNode;

// -- GENERIC --
class GenericArgsNode;

// -- EXPR --
class ExpressionNode;
class ParenthesizedExprNode;
class LiteralExprNode;
class IntLiteralExprNode;
class FloatLiteralExprNode;
class StringLiteralExprNode;
class BoolLiteralExprNode;
class UnaryExprNode;
class BinaryExprNode;
class AssignmentExprNode;
class NameExprNode;
class CallExprNode;
class IndexExprNode;
class MemberAccessExprNode;
class ConstructExprNode;

// -- DECL --
class DeclarationNode;
class CallableDeclNode;
class ModuleDeclNode;
class FunctionDeclNode;
class ParameterDeclNode;
class VariableDeclNode;
class ClassDeclNode;
class MethodDeclNode;
class FieldDeclNode;

// -- STMT --
class StatementNode;
class BaseLoopStmtNode;
class BlockStmtNode;
class ExpressionStmtNode;
class IfStmtNode;
class LoopStmtNode;
class WhileStmtNode;
class ForStmtNode;
class ReturnStmtNode;
class UseStmtNode;

// -- TYPE --
class TypeNode;
class BuiltinTypeNode;
class NamedTypeNode;

} // namespace ast
VEEC_NAMESPACE_END
