/**
 * @file Symbol.hpp
 * @brief This file contains the definition of the symbol class.
 * 
 * The symbol class is the base class for all symbols in the semantic analysis phase.
 * Symbols represent various entities in the program, such as functions.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Maybe.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/SymbolKind.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @class Symbol
 * @brief The base class for all symbols in the semantic analysis phase.
 * Symbols represent various entities in the program, such as functions.
 */
class Symbol {
public:
    virtual ~Symbol() = default;

    /**
     * @brief Converts a SymbolKind to its string representation.
     * @param kind The symbol kind.
     * @return A string representation of the symbol kind.
     */
    static std::string_view kindString(SymbolKind kind);

    /**
     * @brief Gets the kind of this symbol.
     * @return The kind of this symbol.
     */
    inline SymbolKind getKind() const { return _kind; }
    /**
     * @brief Gets the name? of this symbol, if any.
     * @return The name? of this symbol.
     */
    inline basic::Maybe<basic::StringId> getName() const { return _name; }
    /**
     * @brief Gets the name! of this symbol, if any.
     * @return The name! of this symbol.
     * @note Asserts if the symbol has no name, check first using getName().
     */
    inline basic::StringId getNameValue() const {
        VEE_ASSERT(_name.hasValue(), "Cannot get definitive name of symbol when it has no name");
        return _name.value();
    }

    /**
     * @brief Gets the static kind of this symbol.
     * @return The static kind of this symbol.
     */
    template<typename T>
    inline bool is() const {
        static_assert(std::is_base_of_v<Symbol, T>, "T must be derived from Symbol");
        return T::isClassOf(this);
    }

    /**
     * @brief Casts this symbol to the specified type.
     * @tparam T The type to cast to, which must be derived from Symbol.
     * @return A pointer to this symbol cast to the specified type, or nullptr if this symbol
     * is not of the specified type.
     */
    template<typename T>
    inline T* as() {
        static_assert(std::is_base_of_v<Symbol, T>, "T must be derived from Symbol");

        if (is<T>())
            return static_cast<T*>(this);
        return nullptr;
    }
    /**
     * @brief Casts this symbol to the specified type (read-only).
     * @tparam T The type to cast to, which must be derived from Symbol.
     * @return A pointer to this symbol cast to the specified type, or nullptr if this symbol
     * is not of the specified type.
     */
    template<typename T>
    inline const T* as() const {
        static_assert(std::is_base_of_v<Symbol, T>, "T must be derived from Symbol");

        if (is<T>())
            return static_cast<const T*>(this);
        return nullptr;
    }

protected:
    /**
     * @brief Constructs a new unnamed Symbol instance with the specified kind.
     * @param kind The kind of the symbol.
     * @param name The name of the symbol.
     */
    Symbol(SymbolKind kind)
        : _kind(kind), _name() {}
    /**
     * @brief Constructs a new Symbol instance with the specified name and kind.
     * @param kind The kind of the symbol.
     * @param name The name of the symbol.
     */
    Symbol(SymbolKind kind, basic::StringId name)
        : _kind(kind), _name(name) {}

private:
    SymbolKind _kind;
    basic::Maybe<basic::StringId> _name;
};

} // namespace symbols
VEEC_NAMESPACE_END
