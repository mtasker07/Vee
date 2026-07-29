/**
 * @file FunctionSymbol.hpp
 * @brief This file contains the definition of the function symbol class.
 *
 * The function symbol class represents functions/methods in the program.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/ScopeOwnerSymbol.hpp"
#include "veec/symbols/SymbolKind.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/types/TypeContext.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/FunctionType.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @brief Represents the kind of a function symbol, which can be either a
 * regular function or a method.
 */
enum class FunctionSymbolKind : u8 {
    Function, // Free function
    Method, // Member function (method)
};

/**
 * @brief The function symbol represents a function in the program.
 */
class FunctionSymbol : public ScopeOwnerSymbol {
public:
    /**
     * @brief Creates a function symbol with the specified name ID.
     * @param nameId The name ID of the function.
     */
    FunctionSymbol(basic::StringId nameId, FunctionSymbolKind kind = FunctionSymbolKind::Function)
        : ScopeOwnerSymbol(SymbolKind::Function, nameId), _kind(kind) {}

    virtual ~FunctionSymbol() = default;

    /**
     * @brief Gets the kind of this function symbol.
     * @return The kind of this function symbol.
     */
    inline FunctionSymbolKind getFunctionKind() const { return _kind; }

    //
    // Types
    //

    /**
     * @brief Gets the type of this function symbol.
     * @return The type of this function symbol.
     */
    inline types::FunctionType* getType() const {
        return _type;
    }
    /**
     * @brief Sets the type of this function symbol.
     * It is up to the caller to ensure that the type being set matches the stored
     * return type and parameters. Not doing so may lead to broken behaviour.
     */
    inline void setType(types::FunctionType* type) {
        VEE_ASSERT(type != nullptr, "Cannot set function symbol type to null");
        _type = type;
    }
    /**
     * @brief Gets the return type of this function symbol.
     * @return The return type of this function symbol.
     */
    inline types::Type* getReturnType() const {
        VEE_ASSERT(_type != nullptr, "Function symbol type is null, cannot get return type");
        return _type->getReturnType();
    }
    /**
     * @brief Gets the parameter types of this function symbol.
     * @return A vector of parameter types for this function symbol.
     * @note This grabs the parameter types from the function type, not the list
     * of parameter symbols.
     */
    inline std::vector<types::Type*> getParameterTypes() const {
        VEE_ASSERT(_type != nullptr, "Function symbol type is null, cannot get parameter types");
        return _type->getParameterTypes();
    }

    //
    // Parameters
    //

    /**
     * @brief Gets the parameters of this function symbol.
     * @return A list of parameter symbols for this function symbol.
     */
    inline const basic::SmallVector<VariableSymbol*>& getParameters() const {
        return _parameters;
    }
    /**
     * @brief Adds a parameter to this function symbol.
     * @param param The parameter symbol to add.
     */
    inline void addParameter(VariableSymbol* param) {
        VEE_ASSERT(param != nullptr, "Cannot add a null parameter to a function symbol");
        _parameters.push_back(param);
    }

    /**
     * @brief Checks if the given symbol is a function symbol.
     * @param s The symbol to check.
     * @return True if the symbol is a function symbol, false otherwise.
     */
    static bool isClassOf(const Symbol* s) {
        return s->getKind() == SymbolKind::Function;
    }

private:
    FunctionSymbolKind _kind;
    types::FunctionType* _type = nullptr;
    basic::SmallVector<VariableSymbol*> _parameters;
};

} // namespace symbols
VEEC_NAMESPACE_END
