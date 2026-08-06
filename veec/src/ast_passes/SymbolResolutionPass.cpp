#include "veec/sema_passes/SymbolResolutionPass.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/ast/name/QualifiedNameNode.hpp"
#include "veec/ast/expr/NameExprNode.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/ParameterDeclNode.hpp"
#include "veec/ast/stmt/BlockStmtNode.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/Scope.hpp"
#include "veec/sema/ScopeGuard.hpp"
#include "veec/sema/ScopeManager.hpp"
#include "veec/symbols/SymbolTable.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/ScopeOwnerSymbol.hpp"
#include "veec/symbols/IdentifierTable.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/ClassSymbol.hpp"
#include "veec/symbols/ent/FieldSymbol.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"


VEEC_NAMESPACE_BEGIN
namespace sema_passes {

void SymbolResolutionPass::visitCompilationUnit(ast::CompilationUnitNode& node) {
    // Get scope for unit
    sema::ScopeGuard guard(&_currentScope, _sema.scopes.getNodeScope(&node));
    VEE_ASSERT(_currentScope, "Compilation unit node has no associated scope");

    // Walk node normally
    ast::AstWalker::visitCompilationUnit(node);
}
void SymbolResolutionPass::visitBlockStmt(ast::BlockStmtNode& node) {
    // Get scope for block
    sema::Scope* blockScope = _sema.scopes.getNodeScope(&node);
    sema::ScopeGuard guard(&_currentScope, blockScope);

    // Resolve block symbols
    ast::AstWalker::visitBlockStmt(node);
}
void SymbolResolutionPass::visitFunctionDecl(ast::FunctionDeclNode& node) {
    // Get scope for function
    sema::Scope* funcScope = _sema.scopes.getNodeScope(&node);
    sema::ScopeGuard guard(&_currentScope, funcScope);

    // Resolve function symbols
    ast::AstWalker::visitFunctionDecl(node);
}

void SymbolResolutionPass::visitNameExpr(ast::NameExprNode& node) {
	resolveQualifiedName(node.getQualifiedName());
}

void SymbolResolutionPass::resolveQualifiedName(ast::QualifiedNameNode& node) {
    using SymKind = symbols::SymbolKind;

    // Identify start scope for resolution
    source::Identifier firstSegment = node.getSegments().front().identifier;
    sema::Scope* s = _currentScope;
    while (s != nullptr) {
        if (lookupNameInScope(firstSegment.id, s) != nullptr) {
            break;
        }
        s = s->getParent();
    }

    // Unknown first segment
    if (s == nullptr) {
        _ctx.diagnostics.report(
            diagnostics::ERROR_UNKNOWN_IDENTIFIER,
            node.getRange(),
            _ctx.strings.get(firstSegment.id)
        );
        return;
    }

    // Validate each segment from initial scope
	const auto& segments = node.getSegments();
    symbols::Symbol* lastResolvedSymbol = nullptr;
    for (size_t i = 0; i < segments.size(); ++i) {
        basic::StringId id = segments[i].identifier.id;
        symbols::Symbol* resolved = lookupNameInScope(id, s);
        if (resolved == nullptr) {
            // Unknown symbol (not first segment)
            _ctx.diagnostics.report(
                diagnostics::ERROR_NO_MEMBER_IN_SYMBOL,
                node.getRange(),
                symbols::Symbol::kindString(lastResolvedSymbol->getKind()),
                _ctx.strings.get(segments[i - 1].identifier.id),
                // ^^ Always safe since first segment is guaranteed to be valid
                _ctx.strings.get(id)
            );
            return;
        }
        lastResolvedSymbol = resolved;

		if (i == segments.size() - 1) {
            // No more segments, break early to avoid checking for scope owner
            break;
        }

        symbols::ScopeOwnerSymbol* sos = resolved->as<symbols::ScopeOwnerSymbol>();
        if (!sos) {
            std::string_view kindStr =
                symbols::Symbol::kindString(resolved->getKind());

            _ctx.diagnostics.report(
                diagnostics::ERROR_SYMBOL_CANNOT_BE_QUALIFIED,
                node.getRange(),
                kindStr,
                _ctx.strings.get(id)
            );
            return;
        }

        s = sos->getScope();
    }
    VEE_ASSERT(lastResolvedSymbol != nullptr, "Failed to resolve qualified name. Node:\n{}", node.toString(_ctx));
    node.resolvedSymbol = lastResolvedSymbol;
}
symbols::Symbol* SymbolResolutionPass::lookupNameInScope(basic::StringId nameId, sema::Scope* scope) {
    VEE_ASSERT(scope != nullptr, "Scope is null while looking up name: {}", _ctx.strings.get(nameId));

    return scope->getIdentifierTable().lookup(nameId);
}

} // namespace sema_passes
VEEC_NAMESPACE_END
