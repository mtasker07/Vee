/**
 * @file SymbolCollectionPass.hpp
 * @brief This file contains the definition of the SymbolCollectionPass class,
 * which is responsible for collecting all symbols in the program and
 * adding them to the symbol table.
 * 
 * The SymbolCollectionPass is also responsible for creating the initial
 * scope hierarchy for the program and binding them to the correct ast nodes,
 * which is used by other passes. No other semantic passes should create any scopes,
 * they should only access them using the ScopeManager.
 */

#pragma once

#include <stack>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/SemaContext.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema_passes {

/**
 * @class SymbolCollectionPass
 * @brief A semantic analysis pass that collects all symbols in the program.
 */
class SymbolCollectionPass : public sema::Pass {
public:
    /**
     * @brief Creates a new SymbolCollectionPass instance with the given context.
     * @param ctx The CompilationContext to use for this pass.
     * @param sema The SemaContext to use for this pass.
     */
    SymbolCollectionPass(compilation::CompilationContext& ctx, sema::SemaContext& sema)
        : sema::Pass(ctx, sema) {}
        
    virtual ~SymbolCollectionPass() = default;

    /**
     * @brief Runs the symbol collection pass over a given module node.
     * @param node The CompilationUnitNode AST node to run this pass on.
     */
    virtual void run(ast::CompilationUnitNode& node) override {
        walk(node);
    }

protected:
    virtual void visitCompilationUnit(ast::CompilationUnitNode& node) override;
    virtual void visitModuleDecl(ast::ModuleDeclNode& node) override;
    virtual void visitFunctionDecl(ast::FunctionDeclNode& node) override;
    virtual void visitParameterDecl(ast::ParameterDeclNode& node) override;
    virtual void visitClassDecl(ast::ClassDeclNode& node) override;
    virtual void visitMethodDecl(ast::MethodDeclNode& node) override;
    virtual void visitFieldDecl(ast::FieldDeclNode& node) override;
	virtual void visitVariableDecl(ast::VariableDeclNode& node) override;
    virtual void visitBlockStmt(ast::BlockStmtNode& node) override;

private:
    symbols::ModuleSymbol* _currentModule = nullptr;
    symbols::FunctionSymbol* _currentFunctionOrMethod = nullptr;
    symbols::ClassSymbol* _currentClass = nullptr;
    std::stack<sema::Scope*> _scopeStack;

    // Helpers
    bool existsBindingNamed(basic::StringId nameId) const;
    symbols::Symbol* bindingNamed(basic::StringId nameId) const;

    void ensureBind(basic::StringId nameId, symbols::Symbol* symbol);

    // Report helpers
    void reportNameAlreadyDeclaredInScope(const ast::AstNode& node, std::string_view symbolKind, basic::StringId nameId);

    // True if in module but not function/class, etc
    inline bool inModule() const {
		return _currentModule && !_currentFunctionOrMethod && !_currentClass;
    }

    inline sema::Scope* currentScope() const {
        if (_scopeStack.empty()) {
            return nullptr;
        }
        return _scopeStack.top();
    }
    inline void pushScope(sema::Scope* scope) {
        _scopeStack.push(scope);
    }
    inline sema::Scope* popScope() {
        if (_scopeStack.empty()) {
            return nullptr;
        }
        sema::Scope* top = _scopeStack.top();
        _scopeStack.pop();
        return top;
    }
};

} // namespace sema_passes
VEEC_NAMESPACE_END
