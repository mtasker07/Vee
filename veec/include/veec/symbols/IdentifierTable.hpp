/**
 * @file IdentifierTable.hpp
 * @brief This file contains the definition of the identifier table class.
 * 
 * The identifier table class is responsible for managing the binding of names to symbols in the
 * semantic analysis phase.
 */

#pragma once

#include <vector>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @class IdentifierTable
 * @brief Helper class for managing the binding of names to symbols.
 */
class IdentifierTable {
public:
    /**
     * @brief Creates a new IdentifierTable instance.
     */
    IdentifierTable() = default;
    ~IdentifierTable() = default;

    /**
     * @brief Checks if a given name is bound to any symbol in this table.
     * @return True if the name is bound to a symbol, false otherwise.
     */
    inline bool hasBinding(basic::StringId nameId) const {
        return _bindings.find(nameId) != _bindings.end();
    }
    /**
     * @brief Performs a lookup for a symbol by a given name.
     * @param nameId The name ID of the symbol to look up.
     * @return The symbol bound to the given name, or nullptr if the name is not bound
     * to any symbol.
     */
    inline symbols::Symbol* lookup(basic::StringId nameId) const {
        auto it = _bindings.find(nameId);
        if (it != _bindings.end()) {
            return it->second;
        }
        return nullptr;
    }

    /**
     * @brief Binds a symbol to a given name in this table.
     * @param nameId The name ID to bind the symbol to.
     * @param symbol The symbol to bind to the name.
     * @return True if the symbol was successfully bound to the name, false if the name is
     * already bound to another symbol.
     */
    inline bool bind(basic::StringId nameId, symbols::Symbol* symbol) {
        auto result = _bindings.emplace(nameId, symbol);
        return result.second;
    }

private:
    std::unordered_map<basic::StringId, symbols::Symbol*> _bindings;
};

} // namespace symbols
VEEC_NAMESPACE_END
