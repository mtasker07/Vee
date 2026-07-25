/**
 * @file SemaContext.hpp
 * @brief This file contains the definition of the SemaContext class,
 * which is used to hold context for semantic analysis stages during the
 * compilation process.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/symbols/SymbolTable.hpp"
#include "veec/symbols/OperatorTable.hpp"
#include "veec/types/TypeTable.hpp"
#include "veec/sema/ScopeManager.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

/**
 * @class SemaContext
 * @brief Used to hold context for semantic analysis stages during the compilation process.
 */
class SemaContext {
public:
    ScopeManager scopes;
    symbols::SymbolTable symbols;
    symbols::OperatorTable operators;
    types::TypeTable types;

    /**
     * @brief Creates a new SemaContext instance.
     */
    SemaContext() = default;
    ~SemaContext() = default;
};

} // namespace sema
VEEC_NAMESPACE_END
