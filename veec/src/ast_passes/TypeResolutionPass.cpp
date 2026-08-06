#include "veec/sema_passes/TypeResolutionPass.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/ParameterDeclNode.hpp"
#include "veec/ast/decl/ClassDeclNode.hpp"
#include "veec/ast/decl/MethodDeclNode.hpp"
#include "veec/ast/decl/FieldDeclNode.hpp"
#include "veec/ast/decl/VariableDeclNode.hpp"
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
namespace sema_passes {

void TypeResolutionPass::visitFunctionDecl(ast::FunctionDeclNode& node) {
    ast::AstWalker::visitFunctionDecl(node);
    // ^^ Assigns parameter types and return type

    symbols::FunctionSymbol* functionSymbol = node.symbol;
    VEE_ASSERT(functionSymbol != nullptr, "FunctionDeclNode has no associated FunctionSymbol");

    // Set the return type and parameters
    types::Type* returnType = _ctx.types.table.getNodeType(node.getReturnType());

    std::vector<types::Type*> parameterTypes;
    for (ast::ParameterDeclNode* paramNode : node.getParameters()) {
        // Add to symbol
        functionSymbol->addParameter(paramNode->symbol);
        parameterTypes.push_back(_ctx.types.table.getNodeType(paramNode->getType()));
    }

    // Assign to symbol
    types::FunctionType* funcType = _ctx.types.table.getFunction(returnType, parameterTypes);
    functionSymbol->setType(funcType);
}
void TypeResolutionPass::visitParameterDecl(ast::ParameterDeclNode& node) {
    ast::AstWalker::visitParameterDecl(node);

    symbols::VariableSymbol* paramSymbol = node.symbol;
    VEE_ASSERT(paramSymbol != nullptr, "ParameterDeclNode has no associated VariableSymbol");

    // Grab parameter type
    types::Type* paramType = _ctx.types.table.getNodeType(node.getType());

    // Assign to symbol
    paramSymbol->setType(paramType);
}
void TypeResolutionPass::visitClassDecl(ast::ClassDeclNode& node) {
    ast::AstWalker::visitClassDecl(node);
    
    // Class symbol info set from TypeConstructionPass
    VEE_ASSERT(node.symbol != nullptr, "ClassDeclNode has no associated ClassSymbol");
    VEE_ASSERT(node.symbol->getType() != nullptr, "ClassSymbol has no associated ClassType");
}
void TypeResolutionPass::visitMethodDecl(ast::MethodDeclNode& node) {
    ast::AstWalker::visitMethodDecl(node);
    // ^^ Assigns parameter types and return type

    symbols::FunctionSymbol* methodSymbol = node.symbol;
    VEE_ASSERT(methodSymbol != nullptr, "MethodDeclNode has no associated FunctionSymbol");

    // Grab the return type and parameter types
    types::Type* returnType = _ctx.types.table.getNodeType(node.getReturnType());
    std::vector<types::Type*> parameterTypes;
    for (ast::ParameterDeclNode* paramNode : node.getParameters()) {
        parameterTypes.push_back(_ctx.types.table.getNodeType(paramNode->getType()));
    }

    // Assign to symbol
    types::FunctionType* funcType = _ctx.types.table.getFunction(returnType, parameterTypes);
    methodSymbol->setType(funcType);
}
void TypeResolutionPass::visitFieldDecl(ast::FieldDeclNode& node) {
    ast::AstWalker::visitFieldDecl(node);

    symbols::FieldSymbol* fieldSymbol = node.symbol;
    VEE_ASSERT(fieldSymbol != nullptr, "FieldDeclNode has no associated FieldSymbol");

    // Grab field type
    types::Type* fieldType = _ctx.types.table.getNodeType(node.getType());

    // Assign to symbol
    fieldSymbol->setType(fieldType);
}
void TypeResolutionPass::visitVariableDecl(ast::VariableDeclNode& node) {
    ast::AstWalker::visitVariableDecl(node);

    symbols::VariableSymbol* varSymbol = node.symbol;
    VEE_ASSERT(varSymbol != nullptr, "VariableDeclNode has no associated VariableSymbol");

    // Grab variable type
    types::Type* varType = _ctx.types.table.getNodeType(node.getType());

    // Assign to symbol
    varSymbol->setType(varType);
}

void TypeResolutionPass::visitType(ast::TypeNode& node) {
    ast::AstWalker::visitType(node);
}
void TypeResolutionPass::visitBuiltinType(ast::BuiltinTypeNode& node) {
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
    
    types::Type* type = _ctx.types.table.getBuiltin(kind);
    _ctx.types.table.setNodeType(&node, type);
}
void TypeResolutionPass::visitNamedType(ast::NamedTypeNode& node) {
    ast::AstWalker::visitNamedType(node);

    types::ErrorType* errorType = _ctx.types.table.getError();

    symbols::Symbol* resolvedSymbol = node.getResolvedSymbol();
    if (!resolvedSymbol || !resolvedSymbol->is<symbols::ClassSymbol>()) {
        _ctx.types.table.setNodeType(&node, errorType);
        return;
    }

    symbols::ClassSymbol* classSymbol = resolvedSymbol->as<symbols::ClassSymbol>();

    types::Type* classType = _ctx.types.table.getClass(classSymbol);
    _ctx.types.table.setNodeType(&node, classType);
}

} // namespace sema_passes
VEEC_NAMESPACE_END
