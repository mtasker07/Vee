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
#include "veec/symbols/ScopeOwnerSymbol.hpp"
#include "veec/symbols/SymbolKind.hpp"
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

    /**
     * @brief Gets the return type of this function symbol.
     * @return The return type of this function symbol.
     */
    inline types::FunctionType* getType() const { return _funcType; }
    /**
     * @brief Sets the function type of this function symbol.
     * @param funcType The function type to set for this function symbol.
     */
    inline void setType(types::FunctionType* funcType) { _funcType = funcType; }

    /**
     * @brief Gets the return type of this function symbol.
     * @return The return type of this function symbol.
     */
    inline types::Type* getReturnType() const {
        VEE_ASSERT(_funcType != nullptr, "Function type not set for function symbol");
        return _funcType->getReturnType();
    }

    /**
     * @brief Gets the parameter types of this function symbol.
     * @return A vector of parameter types for this function symbol.
     */
    inline const std::vector<types::Type*>& getParameterTypes() const {
        VEE_ASSERT(_funcType != nullptr, "Function type not set for function symbol");
        return _funcType->getParameterTypes();
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
    types::FunctionType* _funcType = nullptr;
};

} // namespace symbols
VEEC_NAMESPACE_END
