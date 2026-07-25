/**
 * @file FieldSymbol.hpp
 * @brief This file contains the definition of the field symbol class.
 *
 * The field symbol class represents fields within user-defined types in the program.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/SymbolKind.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/Visibility.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @brief The field symbol represents a field within a user-defined type in the program.
 */
class FieldSymbol : public Symbol {
public:
    /**
     * @brief Creates a field symbol with the specified name ID.
     * @param nameId The name ID of the field.
     */
    FieldSymbol(basic::StringId nameId)
        : Symbol(SymbolKind::Field, nameId) {}

    virtual ~FieldSymbol() = default;

    /**
     * @brief Gets the type of this field symbol.
     * @return The type of this field symbol.
     */
    inline types::Type* getType() const { return _type; }
    /**
     * @brief Sets the type of this field symbol.
     * @param type The type to set for this field symbol.
     */
    inline void setType(types::Type* type) { _type = type; }
    
    /**
     * @brief Gets the visibility of this field symbol.
     * @return The visibility of this field symbol.
     */
    inline Visibility getVisibility() const { return _visibility; }
    /**
     * @brief Sets the visibility of this field symbol.
     * @param visibility The visibility to set for this field symbol.
     */
    inline void setVisibility(Visibility visibility) { _visibility = visibility; }

    /**
     * @brief Checks if the given symbol is a field symbol.
     * @param s The symbol to check.
     * @return True if the symbol is a field symbol, false otherwise.
     */
    static bool isClassOf(const Symbol* s) {
        return s->getKind() == SymbolKind::Field;
    }

private:
    types::Type* _type = nullptr;
    Visibility _visibility = Visibility::Private;
};

} // namespace symbols
VEEC_NAMESPACE_END
