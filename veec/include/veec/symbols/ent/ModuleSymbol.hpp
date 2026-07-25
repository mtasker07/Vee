/**
 * @file ModuleSymbol.hpp
 * @brief This file contains the definition of the module symbol class.
 *
 * The module symbol class represents modules in the program. Modules are like
 * namespaces in that they are generic named containers for symbols.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/symbols/ScopeOwnerSymbol.hpp"
#include "veec/symbols/SymbolKind.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/Visibility.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @brief The module symbol represents a module in the program.
 */
class ModuleSymbol : public ScopeOwnerSymbol {
public:
    /**
     * @brief Creates a module symbol with the specified name ID.
     * @param nameId The name ID of the module.
     */
    ModuleSymbol(basic::StringId nameId)
    : ScopeOwnerSymbol(SymbolKind::Module, nameId) {}

    virtual ~ModuleSymbol() = default;

    /**
     * @brief Gets the symbols of this module symbol (read-only).
     * @return The symbols of this module symbol.
     */
    inline const std::vector<Symbol*>& getSymbols() const { return _symbols; }
    /**
     * @brief Adds a symbol to this module symbol.
     * @param symbol The symbol to add to this module symbol.
     */
    inline void addSymbol(Symbol* symbol) { _symbols.push_back(symbol); }

    /**
     * @brief Gets the visibility of this module symbol.
     * @return The visibility of this module symbol.
     */
    inline Visibility getVisibility() const { return _visibility; }
    /**
     * @brief Sets the visibility of this module symbol.
     * @param visibility The visibility to set for this module symbol.
     */
    inline void setVisibility(Visibility visibility) { _visibility = visibility; }

    /**
     * @brief Checks if the given symbol is a module symbol.
     * @param s The symbol to check.
     * @return True if the symbol is a module symbol, false otherwise.
     */
    static bool isClassOf(const Symbol* s) {
        return s->getKind() == SymbolKind::Module;
    }

private:
    std::vector<Symbol*> _symbols;
    Visibility _visibility = Visibility::Public;
};

} // namespace symbols
VEEC_NAMESPACE_END
