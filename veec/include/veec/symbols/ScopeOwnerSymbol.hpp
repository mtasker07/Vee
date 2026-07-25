/**
 * @file ScopeOwnerSymbol.hpp
 * @brief This file contains the definition of the ScopeOwnerSymbol class.
 * 
 * The ScopeOwnerSymbol class represents symbols that can own a scope, such as functions.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/sema/Scope.hpp"
#include "veec/symbols/SymbolKind.hpp"
#include "veec/symbols/Symbol.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @class ScopeOwnerSymbol
 * @brief The base class for all symbols that can own a scope in the semantic analysis phase.
 */
class ScopeOwnerSymbol : public Symbol {
public:
    virtual ~ScopeOwnerSymbol() = default;

    /**
     * @brief Gets the scope owned by this symbol.
     * @return The scope owned by this symbol.
     */
    inline sema::Scope* getScope() const {
        return _scope;
    }
    /**
     * @brief Binds a scope to this symbol.
     * @param scope The scope to bind to this symbol.
     */
    inline void bindScope(sema::Scope* scope) {
        _scope = scope;
    }

    /**
     * @brief Checks if the given symbol is a scope owner symbol.
     * @param s The symbol to check.
     * @return True if the symbol is a scope owner symbol, false otherwise.
     */
    static bool isClassOf(const Symbol* s) {
        SymbolKind kind = s->getKind();
        return
			kind == SymbolKind::Module ||
            kind == SymbolKind::Function ||
            kind == SymbolKind::Class;
    }

protected:
    /**
     * @brief Constructs a new Symbol instance with the specified name and kind.
     * @param kind The kind of the symbol.
     * @param name The name of the symbol.
     */
    ScopeOwnerSymbol(SymbolKind kind, basic::StringId name)
        : Symbol(kind, name) {}

private:
    sema::Scope* _scope = nullptr;
};

} // namespace symbols
VEEC_NAMESPACE_END
