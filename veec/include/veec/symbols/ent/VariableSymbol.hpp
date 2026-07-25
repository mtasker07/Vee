/**
 * @file VariableSymbol.hpp
 * @brief This file contains the definition of the VariableSymbol class.
 *
 * The VariableSymbol class represents variables in the program.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/SymbolKind.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @brief Represents the storage type of a variable symbol.
 */
enum class VariableSymbolStorage : u8 {
    Local, // A local variable, typically defined within a function or block scope.
    Parameter, // A parameter variable, typically defined in a function's parameter list.
    Global, // A global variable, typically defined at the top level of a module or file.
};

/**
* @brief The variable symbol represents a variable in the program.
*/
class VariableSymbol : public Symbol {
public:
    /**
     * @brief Creates a variable symbol with the specified name ID and storage type.
     * @param nameId The name ID of the variable.
     * @param storage The storage type of the variable.
     */
    VariableSymbol(basic::StringId nameId, VariableSymbolStorage storage)
        : Symbol(SymbolKind::Variable, nameId), _storage(storage) {}

    virtual ~VariableSymbol() = default;

    /**
     * @brief Gets the storage type of this variable symbol.
     * @return The storage type of this variable symbol.
     */
	inline VariableSymbolStorage getStorage() const { return _storage; }

    /**
     * @brief Gets the type of this variable symbol.
     * @return The type of this variable symbol.
     */
    inline types::Type* getType() const { return _type; }
    /**
     * @brief Sets the type of this variable symbol.
     * @param type The type to set for this variable symbol.
     */
    inline void setType(types::Type* type) { _type = type; }

    /**
    * @brief Checks if the given symbol is a variable symbol.
    * @param s The symbol to check.
    * @return True if the symbol is a variable symbol, false otherwise.
    */
    static bool isClassOf(const Symbol* s) {
        return s->getKind() == SymbolKind::Variable;
    }

private:
	VariableSymbolStorage _storage = VariableSymbolStorage::Local;
    types::Type* _type = nullptr;
};

} // namespace symbols
VEEC_NAMESPACE_END
