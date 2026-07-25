/**
 * @file Scope.hpp
 * @brief This file contains the definition of the Scope class.
 * The Scope class represents a scope in the semantic analysis phase,
 * managing symbols and their visibility.
 * 
 * Imports work like aliases. So if you import a symbol such as `use foo::bar`,
 * the imports of the importee scope will be `bar` -> `foo::bar`. This means
 * you can lookup without the full name.
 * This works similarly for importing specific types/functions from modules.
 * In the case of an example like `use foo::bar::{baz, qux}`, the imports of
 * the importee scope will be `baz` -> `foo::bar::baz` and `qux` -> `foo::bar::qux`.
 */

#pragma once

#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Maybe.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/IdentifierTable.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

/**
 * @class Scope
 * @brief Represents a scope in the semantic analysis phase, managing symbols and their visibility.
 */
class Scope {
public:
    /**
     * @brief Creates a new Scope instance with an optional parent scope and owner symbol.
     * @param parent The parent scope, or nullptr if this is a top-level scope.
     * @param owner The symbol that owns this scope, or nullptr if this scope is not owned by any symbol.
     */
    Scope(Scope* parent = nullptr, symbols::Symbol* owner = nullptr)
        : _parent(parent), _owner(owner) {}

    /**
     * @brief Checks whether this scope is a top-level scope (i.e., has no parent).
     * @return True if this scope is a top-level scope, false otherwise.
     */
    inline bool isTopLevel() const { return _parent == nullptr; }
    /**
     * @brief Gets the parent scope of this scope.
     * @return A pointer to the parent scope, or nullptr if this is a top-level scope.
     */
    inline Scope* getParent() const { return _parent; }

    /**
     * @brief Gets the symbol that owns this scope.
     * @return A pointer to the owner symbol, or nullptr if this scope is not owned by any symbol.
     */
    inline symbols::Symbol* getOwner() const { return _owner; }

    /**
     * @brief Gets the identifier table of this scope (read-only).
     * @return A reference to the identifier table of this scope.
     */
    inline const symbols::IdentifierTable& getIdentifierTable() const { return _identifiers; }
    /**
     * @brief Gets the identifier table of this scope.
     * @return A reference to the identifier table of this scope.
     */
    inline symbols::IdentifierTable& getIdentifierTable() { return _identifiers; }

private:
    friend class ScopeManager;

    Scope* _parent = nullptr;
    symbols::Symbol* _owner = nullptr;
    symbols::IdentifierTable _identifiers;
};

} // namespace sema
VEEC_NAMESPACE_END
