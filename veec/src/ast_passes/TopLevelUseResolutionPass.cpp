#include "veec/sema_passes/TopLevelUseResolutionPass.hpp"

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
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/ClassSymbol.hpp"
#include "veec/symbols/ent/FieldSymbol.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema_passes {

void TopLevelUseResolutionPass::visitCompilationUnit(ast::CompilationUnitNode&) {
}

} // namespace sema_passes
VEEC_NAMESPACE_END
