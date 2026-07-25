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

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @enum SymbolKind
 * @brief Represents the kind of a symbol.
 */
enum class SymbolKind {
    Module,
    Function,
    FunctionSet,
    Variable,
    Class,
    Field,
    Operator,
};

} // namespace symbols
VEEC_NAMESPACE_END
