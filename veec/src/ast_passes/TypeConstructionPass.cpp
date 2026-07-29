#include "veec/ast_passes/TypeConstructionPass.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/ParameterDeclNode.hpp"
#include "veec/ast/decl/ClassDeclNode.hpp"
#include "veec/ast/decl/MethodDeclNode.hpp"
#include "veec/ast/decl/FieldDeclNode.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/Scope.hpp"
#include "veec/sema/ScopeManager.hpp"
#include "veec/symbols/SymbolTable.hpp"
#include "veec/symbols/ent/ClassSymbol.hpp"
#include "veec/symbols/ent/FieldSymbol.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/ClassType.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast_passes {

void TypeConstructionPass::visitClassDecl(ast::ClassDeclNode& node) {
    VEE_ASSERT(node.symbol != nullptr, "ClassDeclNode assigned no symbol!");

    symbols::ClassSymbol* oldCurrentClass = _currentClass;
    _currentClass = node.symbol;

    // Create semantic type
    types::ClassType* classType = _ctx.types.table.getClass(node.symbol);
    _currentClass->setType(classType);

    // Walk members
    ast::AstWalker::visitClassDecl(node);

    _currentClass = oldCurrentClass;
}
void TypeConstructionPass::visitMethodDecl(ast::MethodDeclNode& node) {
    VEE_ASSERT(node.symbol != nullptr, "MethodDeclNode assigned no symbol!");
    VEE_ASSERT(_currentClass != nullptr, "Method declaration outside of class context");
    VEE_ASSERT(_currentClass->getType() != nullptr, "Class symbol has no associated type");

    symbols::FunctionSymbol* methodSym = node.symbol;
    _currentClass->getType()->addMethod(methodSym);
}
void TypeConstructionPass::visitFieldDecl(ast::FieldDeclNode& node) {
    VEE_ASSERT(node.symbol != nullptr, "FieldDeclNode assigned no symbol!");
    VEE_ASSERT(_currentClass != nullptr, "Field declaration outside of class context");
    VEE_ASSERT(_currentClass->getType() != nullptr, "Class symbol has no associated type");

    symbols::FieldSymbol* fieldSym = node.symbol;
    _currentClass->getType()->addField(fieldSym);
}

} // namespace ast_passes
VEEC_NAMESPACE_END
