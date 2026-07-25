/**
 * @file Symbol.hpp
 * @brief This file contains the definition of the symbol class.
 * 
 * The symbol class is the base class for all symbols in the semantic analysis phase.
 * Symbols represent various entities in the program, such as functions.
 */

#pragma once

#include <string_view>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/symbols/Symbol.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @class SymbolHandle
 * @brief Represents a handle to a symbol, which may or may not be resolved.
 * A symbol handle can be resolved to a specific symbol during semantic analysis.
 * @tparam T The type of symbol this handle refers to, which must be derived from Symbol.
 * @note If `T` is not specified, it defaults to `Symbol` (which can be any symbol type).
 */
template<typename T = Symbol>
class SymbolHandle {
    static_assert(std::is_base_of_v<Symbol, T>,
        "T must be derived from Symbol");

public:
    /**
     * @brief Constructs a new, unresolved SymbolHandle instance.
     */
    SymbolHandle() = default;
    /**
     * @brief Constructs a new SymbolHandle instance that refers to the specified symbol.
     * @param symbol The symbol this handle should refer to.
     */
    SymbolHandle(const T& symbol)
        : _symbolId(symbol.getId()) {}

    ~SymbolHandle() = default;
   
    /**
     * @brief Checks if this symbol handle is resolved to a specific symbol.
     * @return True if this symbol handle is resolved, false otherwise.
     */
    inline bool isResolved() const {
        return Symbol::isValidId(_symbolId);
    }
    /**
     * @brief Resolves this symbol handle to the specified symbol.
     * @param symbol The symbol to resolve this handle to.
     * @note This should ONLY be called if this handle is unresolved. Calling it on an
     * already resolved handle will result in a fatal error.
     */
    inline void resolve(T& symbol) {
        if (isResolved()) {
            VEE_FATAL("Symbol handle already resolved!");
        }
        _symbolId = symbol.getId();
    }
    /**
     * @brief Resolves this symbol handle to the specified symbol ID.
     * @param id The ID of the symbol to resolve this handle to.
     * @note This should ONLY be called if this handle is unresolved. Calling it on an
     * already resolved handle will result in a fatal error.
     */
    inline void resolve(SymbolId id) {
        if (isResolved()) {
            VEE_FATAL("Symbol handle already resolved!");
        }
        _symbolId = id;
    }

    /**
     * @brief Gets the symbol ID this handle refers to.
     * @return The symbol ID this handle refers to.
     * @note This should ONLY be called if this handle is resolved. Calling it on an
     * unresolved handle will result in a fatal error.
     */
    inline SymbolId get() const {
        VEE_ASSERT(isResolved(), "Symbol handle is not resolved yet!");
        return _symbolId;
    }

private:
    SymbolId _symbolId = 0;
};

} // namespace symbols
VEEC_NAMESPACE_END
