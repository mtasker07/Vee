#include "veec/sema_passes/SymbolCollectionPass.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/ast/decl/ModuleDeclNode.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/ParameterDeclNode.hpp"
#include "veec/ast/decl/ClassDeclNode.hpp"
#include "veec/ast/decl/MethodDeclNode.hpp"
#include "veec/ast/decl/FieldDeclNode.hpp"
#include "veec/ast/decl/VariableDeclNode.hpp"
#include "veec/ast/stmt/BlockStmtNode.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/Scope.hpp"
#include "veec/sema/ScopeManager.hpp"
#include "veec/symbols/SymbolTable.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/ScopeOwnerSymbol.hpp"
#include "veec/symbols/IdentifierTable.hpp"
#include "veec/symbols/OperatorTable.hpp"
#include "veec/symbols/ent/ModuleSymbol.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/FunctionSetSymbol.hpp"
#include "veec/symbols/ent/ClassSymbol.hpp"
#include "veec/symbols/ent/FieldSymbol.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN

namespace diagnostics {
    class UserDiagnostic;
}

namespace sema_passes {

void SymbolCollectionPass::visitCompilationUnit(ast::CompilationUnitNode& node) {
    // Create scope for unit
    sema::Scope* moduleScope = _sema.scopes.createScope(nullptr);
    _sema.scopes.setNodeScope(&node, moduleScope);
    pushScope(moduleScope);

    // Walk node normally
    ast::AstWalker::visitCompilationUnit(node);

    popScope();
}
void SymbolCollectionPass::visitModuleDecl(ast::ModuleDeclNode& node) {
    symbols::ModuleSymbol* oldCurrentModule = _currentModule;

    basic::StringId modName = node.getName().id;

    // Declare symbol
    symbols::ModuleSymbol* modSym = _sema.symbols.declare<symbols::ModuleSymbol>(modName);
    VEE_ASSERT(modSym != nullptr, "Failed to declare module symbol: {}", _ctx.strings.get(modName));

    // Resolve node to symbol
    node.symbol = modSym;

    // Bind module name -> module
    if (existsBindingNamed(modName)) {
        reportNameAlreadyDeclaredInScope(node, "module", modName);
    }
    else {
        ensureBind(modName, modSym);
    }

    _currentModule = modSym;

    // Create scope for module
    sema::Scope* moduleScope = _sema.scopes.createScope(currentScope());
    _sema.scopes.setNodeScope(&node, moduleScope);
    modSym->bindScope(moduleScope);
    pushScope(moduleScope);

    // Walk node normally
    ast::AstWalker::visitModuleDecl(node);

    popScope();
    _currentModule = oldCurrentModule;
}
void SymbolCollectionPass::visitFunctionDecl(ast::FunctionDeclNode& node) {
    symbols::FunctionSymbol* oldCurrentFunctionOrMethod = _currentFunctionOrMethod;
    
    basic::StringId funcName = node.getName().id;

    // Declare symbol
    auto funcSym = _sema.symbols.declare<symbols::FunctionSymbol>(funcName);
    VEE_ASSERT(funcSym != nullptr, "Failed to declare function symbol: {}", _ctx.strings.get(funcName));

    // Resolve node to symbol
    node.symbol = funcSym;

    // Bind function name -> function set symbol
    if (symbols::Symbol* existingSym = bindingNamed(funcName)) {
        // Existing set -> add overload
        if (existingSym->is<symbols::FunctionSetSymbol>()) {
            symbols::FunctionSetSymbol* funcSetSym = existingSym->as<symbols::FunctionSetSymbol>();
            funcSetSym->addOverload(funcSym);
        }
        // Existing non-function -> report
        else {
            reportNameAlreadyDeclaredInScope(node, "function", funcName);
        }
    }
    else {
        // New set
        auto funcSetSym = _sema.symbols.declare<symbols::FunctionSetSymbol>(funcName);
        VEE_ASSERT(funcSetSym != nullptr, "Failed to declare function set symbol: {}", _ctx.strings.get(funcName));

        // Add initial overload
        funcSetSym->addOverload(funcSym);

        // Bind by name
        ensureBind(funcName, funcSetSym);
    }

    _currentFunctionOrMethod = funcSym;

    // Create scope for function
    sema::Scope* funcScope = _sema.scopes.createScope(currentScope());
    _sema.scopes.setNodeScope(&node, funcScope);
    funcSym->bindScope(funcScope);
    pushScope(funcScope);

    ast::AstWalker::visitFunctionDecl(node);

    popScope();
    _currentFunctionOrMethod = oldCurrentFunctionOrMethod;
}
void SymbolCollectionPass::visitParameterDecl(ast::ParameterDeclNode& node) {
    basic::StringId paramName = node.getName().id;

    // Declare symbol
    auto paramSym = _sema.symbols.declare<symbols::VariableSymbol>(paramName, symbols::VariableSymbolStorage::Parameter);
    VEE_ASSERT(paramSym != nullptr, "Failed to declare parameter symbol: {}", _ctx.strings.get(paramName));

    // Resolve node to symbol
    node.symbol = paramSym;

    // Bind param name -> param symbol
    if (existsBindingNamed(paramName)) {
        reportNameAlreadyDeclaredInScope(node, "parameter", paramName);
    }
    else {
        ensureBind(paramName, paramSym);
    }

    ast::AstWalker::visitParameterDecl(node);
}
void SymbolCollectionPass::visitClassDecl(ast::ClassDeclNode& node) {
    symbols::ClassSymbol* oldCurrentClass = _currentClass;

    basic::StringId className = node.getName().id;

    // Declare symbol
    auto classSym = _sema.symbols.declare<symbols::ClassSymbol>(className);
    VEE_ASSERT(classSym != nullptr, "Failed to declare class symbol: {}", _ctx.strings.get(className));

    // Resolve node to symbol
    node.symbol = classSym;
    
    // Bind class name -> class symbol
    if (existsBindingNamed(className)) {
        reportNameAlreadyDeclaredInScope(node, "class", className);
    }
    else {
        ensureBind(className, classSym);
    }

    _currentClass = classSym;

    // Create scope for class
    sema::Scope* classScope = _sema.scopes.createScope(currentScope());
    _sema.scopes.setNodeScope(&node, classScope);
    classSym->bindScope(classScope);
    pushScope(classScope);

    // Walk node normally
    ast::AstWalker::visitClassDecl(node);

    popScope();
    _currentClass = oldCurrentClass;
}
void SymbolCollectionPass::visitMethodDecl(ast::MethodDeclNode& node) {
    VEE_ASSERT(_currentClass != nullptr, "Method declaration outside of class context");
    symbols::FunctionSymbol* oldCurrentFunctionOrMethod = _currentFunctionOrMethod;

    basic::StringId methodName = node.getName().id;

    // Declare symbol
    auto methodSym = _sema.symbols.declare<symbols::FunctionSymbol>(methodName);
    VEE_ASSERT(methodSym != nullptr, "Failed to declare method symbol: {}", _ctx.strings.get(methodName));

    // Resolve node to symbol
    node.symbol = methodSym;
    
    // Bind function name -> function set symbol
    if (symbols::Symbol* existingSym = bindingNamed(methodName)) {
        // Existing set -> add overload
        if (existingSym->is<symbols::FunctionSetSymbol>()) {
            symbols::FunctionSetSymbol* funcSetSym = existingSym->as<symbols::FunctionSetSymbol>();
            funcSetSym->addOverload(methodSym);
        }
        // Existing non-method -> report
        else {
            reportNameAlreadyDeclaredInScope(node, "method", methodName);
        }
    }
    else {
        // New set
        auto funcSetSym = _sema.symbols.declare<symbols::FunctionSetSymbol>(methodName);
        VEE_ASSERT(funcSetSym != nullptr, "Failed to declare function set symbol: {}", _ctx.strings.get(methodName));

        // Add initial overload
        funcSetSym->addOverload(methodSym);

        // Bind by name
        ensureBind(methodName, funcSetSym);
    }

    _currentFunctionOrMethod = methodSym;

    // Create scope for method
    sema::Scope* methodScope = _sema.scopes.createScope(currentScope());
    _sema.scopes.setNodeScope(&node, methodScope);
    methodSym->bindScope(methodScope);
    pushScope(methodScope);

    ast::AstWalker::visitMethodDecl(node);

    popScope();
    _currentFunctionOrMethod = oldCurrentFunctionOrMethod;
}
void SymbolCollectionPass::visitFieldDecl(ast::FieldDeclNode& node) {
    VEE_ASSERT(_currentClass != nullptr, "Field declaration outside of class context");

    basic::StringId fieldName = node.getName().id;

    // Declare symbol
    auto fieldSym = _sema.symbols.declare<symbols::FieldSymbol>(fieldName);
    VEE_ASSERT(fieldSym != nullptr, "Failed to declare field symbol: {}", _ctx.strings.get(fieldName));
    
    // Resolve node to symbol
    node.symbol = fieldSym;

    // Bind field name -> field symbol
    if (existsBindingNamed(fieldName)) {
        reportNameAlreadyDeclaredInScope(node, "field", fieldName);
    }
    else {
        ensureBind(fieldName, fieldSym);
    }

    ast::AstWalker::visitFieldDecl(node);
}
void SymbolCollectionPass::visitVariableDecl(ast::VariableDeclNode& node) {
    basic::StringId varName = node.getName().id;

    symbols::VariableSymbolStorage storage = inModule() ?
		symbols::VariableSymbolStorage::Global :
		symbols::VariableSymbolStorage::Local;

    // Declare symbol
    auto varSym = _sema.symbols.declare<symbols::VariableSymbol>(varName, storage);
    VEE_ASSERT(varSym != nullptr, "Failed to declare variable symbol: {}", _ctx.strings.get(varName));
    
    // Resolve node to symbol
    node.symbol = varSym;

    // Bind variable name -> variable symbol
    if (existsBindingNamed(varName)) {
        reportNameAlreadyDeclaredInScope(node, "variable", varName);
    }
    else {
        ensureBind(varName, varSym);
    }

    ast::AstWalker::visitVariableDecl(node);
}
void SymbolCollectionPass::visitBlockStmt(ast::BlockStmtNode& node) {
    // Create scope for block
    sema::Scope* blockScope = _sema.scopes.createScope(currentScope());
    pushScope(blockScope);
    _sema.scopes.setNodeScope(&node, blockScope);

    // Collect block symbols
    ast::AstWalker::visitBlockStmt(node);

    popScope();
}

symbols::Symbol* SymbolCollectionPass::bindingNamed(basic::StringId nameId) const {
    VEE_ASSERT(currentScope() != nullptr, "No current scope available");
    const symbols::IdentifierTable& idTable = currentScope()->getIdentifierTable();
    return idTable.lookup(nameId);
}
bool SymbolCollectionPass::existsBindingNamed(basic::StringId nameId) const {
    VEE_ASSERT(currentScope() != nullptr, "No current scope available");
    const symbols::IdentifierTable& idTable = currentScope()->getIdentifierTable();
    return idTable.hasBinding(nameId);
}

void SymbolCollectionPass::ensureBind(basic::StringId nameId, symbols::Symbol* symbol) {
    VEE_ASSERT(currentScope() != nullptr, "No current scope available");
    symbols::IdentifierTable& idTable = currentScope()->getIdentifierTable();
    VEE_ASSERT(idTable.bind(nameId, symbol),
        "Failed to bind name to symbol: {}", _ctx.strings.get(nameId));
}

void SymbolCollectionPass::reportNameAlreadyDeclaredInScope(const ast::AstNode& node, std::string_view symbolKind, basic::StringId nameId) {
    symbols::Symbol* existingSym = currentScope()->getIdentifierTable().lookup(nameId);
    _ctx.diagnostics.report(
        diagnostics::ERROR_NAME_ALREADY_DECLARED_IN_SCOPE,
        node.getRange(),
        symbolKind,
        _ctx.strings.get(nameId),
        symbols::Symbol::kindString(existingSym->getKind())
    );
}

} // namespace sema_passes
VEEC_NAMESPACE_END
