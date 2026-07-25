/**
 * @file ClassSymbol.hpp
 * @brief This file contains the definition of the class symbol class.
 *
 * The class symbol class represents classes in the program.
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
 * @brief The class symbol represents a class in the program.
 */
class ClassSymbol : public ScopeOwnerSymbol {
public:
    /**
     * @brief Creates a class symbol with the specified name ID.
     * @param nameId The name ID of the class.
     */
    ClassSymbol(basic::StringId nameId)
    : ScopeOwnerSymbol(SymbolKind::Class, nameId) {}

    virtual ~ClassSymbol() = default;

    /**
     * @brief Gets the type of this class symbol (read-only).
     * @return The type of this class symbol.
     */
    inline const types::ClassType* getType() const { return _type; }
    /**
     * @brief Gets the type of this class symbol.
     * @return The type of this class symbol.
     */
    inline types::ClassType* getType() { return _type; }
    /**
     * @brief Sets the type of this class symbol.
     * @param type The type to set for this class symbol.
     */
    inline void setType(types::ClassType* type) { _type = type; }

    /**
     * @brief Gets the visibility of this class symbol.
     * @return The visibility of this class symbol.
     */
    inline Visibility getVisibility() const { return _visibility; }
    /**
     * @brief Sets the visibility of this class symbol.
     * @param visibility The visibility to set for this class symbol.
     */
    inline void setVisibility(Visibility visibility) { _visibility = visibility; }

    /**
     * @brief Checks if the given symbol is a class symbol.
     * @param s The symbol to check.
     * @return True if the symbol is a class symbol, false otherwise.
     */
    static bool isClassOf(const Symbol* s) {
        return s->getKind() == SymbolKind::Class;
    }

private:
    types::ClassType* _type = nullptr;
    Visibility _visibility = Visibility::Public;
};

} // namespace symbols
VEEC_NAMESPACE_END
