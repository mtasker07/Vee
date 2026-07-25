#include "veec/ast_passes/TypeInferencePass.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
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
#include "veec/ast/type/TypeNode.hpp"
#include "veec/ast/type/BuiltinTypeNode.hpp"
#include "veec/ast/type/NamedTypeNode.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/FunctionSetSymbol.hpp"
#include "veec/symbols/ent/ClassSymbol.hpp"
#include "veec/symbols/ent/FieldSymbol.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/TypeTable.hpp"
#include "veec/types/ErrorType.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/types/PointerType.hpp"
#include "veec/types/ArrayType.hpp"
#include "veec/types/ClassType.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast_passes {

void TypeInferencePass::visitType(ast::TypeNode& node) {
    ast::AstWalker::visitType(node);
}
void TypeInferencePass::visitBuiltinType(ast::BuiltinTypeNode& node) {
    ast::AstWalker::visitBuiltinType(node);

    // Convert to builtin type (from ast builtin type)
    types::BuiltinTypeKind kind;
    switch (node.getBuiltinTypeKind()) {
        case ast::BuiltinTypeKind::Void: kind = types::BuiltinTypeKind::Void; break;
        case ast::BuiltinTypeKind::Bool: kind = types::BuiltinTypeKind::Bool; break;
        case ast::BuiltinTypeKind::String: kind = types::BuiltinTypeKind::String; break;
        case ast::BuiltinTypeKind::I8: kind = types::BuiltinTypeKind::I8; break;
        case ast::BuiltinTypeKind::I16: kind = types::BuiltinTypeKind::I16; break;
        case ast::BuiltinTypeKind::I32: kind = types::BuiltinTypeKind::I32; break;
        case ast::BuiltinTypeKind::I64: kind = types::BuiltinTypeKind::I64; break;
        case ast::BuiltinTypeKind::U8: kind = types::BuiltinTypeKind::U8; break;
        case ast::BuiltinTypeKind::U16: kind = types::BuiltinTypeKind::U16; break;
        case ast::BuiltinTypeKind::U32: kind = types::BuiltinTypeKind::U32; break;
        case ast::BuiltinTypeKind::U64: kind = types::BuiltinTypeKind::U64; break;
        case ast::BuiltinTypeKind::F32: kind = types::BuiltinTypeKind::F32; break;
        case ast::BuiltinTypeKind::F64: kind = types::BuiltinTypeKind::F64; break;
        default:
            VEE_UNREACHABLE("Unknown builtin type kind");
    }

    types::Type* type = _sema.types.getBuiltin(kind);
    _sema.types.setNodeType(&node, type);
}
void TypeInferencePass::visitNamedType(ast::NamedTypeNode& node) {
    ast::AstWalker::visitNamedType(node);

    types::ErrorType* errorType = _sema.types.getError();

    symbols::Symbol* resolvedSymbol = node.getResolvedSymbol();
    if (!resolvedSymbol || !resolvedSymbol->is<symbols::ClassSymbol>()) {
        _sema.types.setNodeType(&node, errorType);
        return;
    }

    symbols::ClassSymbol* classSymbol = resolvedSymbol->as<symbols::ClassSymbol>();

    types::Type* classType = _sema.types.getClass(classSymbol);
    _sema.types.setNodeType(&node, classType);
}

} // namespace ast_passes
VEEC_NAMESPACE_END
