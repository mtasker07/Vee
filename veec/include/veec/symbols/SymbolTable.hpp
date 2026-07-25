/**
 * @file SymbolTable.hpp
 * @brief This file contains the definition of the symbol table class.
 * 
 * The symbol table class is responsible for managing symbols in the semantic analysis phase.
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <memory>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/symbols/Symbol.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/// @brief Manages all symbols during semantic analysis.
class SymbolTable {
public:
    /**
     * @brief Creates a new SymbolTable instance.
     */
    SymbolTable() = default;
    ~SymbolTable() = default;

    /**
     * @brief Declares a symbol in this symbol table.
     * @tparam T The type of the symbol to declare, which must be derived from Symbol.
     * @tparam Args The types of the arguments to forward to the constructor of the symbol.
     * @param args The arguments to forward to the constructor of the symbol.
     * @return A pointer to the declared symbol.
     */
    template<typename T, typename... Args>
    T* declare(Args&&... args);

    /**
     * @brief Gets all symbols in this symbol table (read-only).
     * @return A reference to the vector of all symbols in this symbol table.
     * @note You shouldn't generally ever need to use this function.
     */
    const std::vector<Symbol*>& getAllSymbols() const { return _symbols; }

private:
    basic::Arena<> _symbolArena;
    std::vector<Symbol*> _symbols;
};

template<typename T, typename... Args>
T* SymbolTable::declare(Args&&... args) {
    static_assert(std::is_base_of_v<Symbol, T>, "T must be derived from Symbol");

    T* symbol = _symbolArena.create<T>(std::forward<Args>(args)...);
    _symbols.push_back(symbol);
    return symbol;
}

} // namespace symbols
VEEC_NAMESPACE_END
