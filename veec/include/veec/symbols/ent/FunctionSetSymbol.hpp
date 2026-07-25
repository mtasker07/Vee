/**
 * @file FunctionSetSymbol.hpp
 * @brief This file contains the definition of the function set symbol class.
 *
 * The function set symbol class represents a set of overloaded functions in the program.
 * It is used to group together multiple function symbols that share the same name but have
 * different parameter types.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/SymbolKind.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/FunctionType.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @brief The function set symbol represents a set of overloaded functions in the program.
 */
class FunctionSetSymbol : public Symbol {
public:
    /**
     * @brief Creates a function set symbol with the specified name ID.
     * @param nameId The name ID of the function set.
     */
    explicit FunctionSetSymbol(basic::StringId nameId)
        : Symbol(SymbolKind::FunctionSet, nameId) {}
    /**
     * @brief Creates a function set symbol with the specified name ID.
     * @param nameId The name ID of the function set.
     */
    FunctionSetSymbol(basic::StringId nameId, const std::vector<FunctionSymbol*>& overloads)
        : Symbol(SymbolKind::FunctionSet, nameId), _overloads(overloads) {}

    virtual ~FunctionSetSymbol() = default;

    /**
     * @brief Gets the overloads of this function set symbol (read-only).
     * @return The overloads of this function set symbol.
     */
    inline const std::vector<FunctionSymbol*>& getOverloads() const {
        return _overloads;
    }
    /**
     * @brief Adds a function overload to this function set symbol.
     * @param overload The function overload to add to this function set symbol.
     */
    inline void addOverload(FunctionSymbol* overload) {
        VEE_ASSERT(overload != nullptr,
            "Cannot add null overload to function set symbol");
        VEE_ASSERT(overload->getName().value() == getName().value(),
            "Cannot add overload with different name to function set symbol");
            
        _overloads.push_back(overload);
    }

    /**
     * @brief Checks if the given symbol is a function symbol.
     * @param s The symbol to check.
     * @return True if the symbol is a function symbol, false otherwise.
     */
    static bool isClassOf(const Symbol* s) {
        return s->getKind() == SymbolKind::FunctionSet;
    }

private:
    std::vector<FunctionSymbol*> _overloads;
};

} // namespace symbols
VEEC_NAMESPACE_END
