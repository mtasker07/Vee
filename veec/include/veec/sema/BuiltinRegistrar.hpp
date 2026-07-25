/**
 * @file BuiltinRegistrar.hpp
 * @brief This file contains the definition of the BuiltinRegistrar class.
 * The BuiltinRegistrar class is responsible for registering all built-in operators
 * and conversions.
 */

#pragma once

#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

class SemaContext;

/**
 * @class BuiltinRegistrar
 * @brief Responsible for registering all built-in operators and conversions.
 */
class BuiltinRegistrar {
public:
    /**
     * @brief Creates a new BuiltinRegistrar instance with the given SemaContext.
     * @param sema The SemaContext to use for registering all built-ins.
     */
    BuiltinRegistrar(SemaContext& sema)
        : _sema(sema) {}

    ~BuiltinRegistrar() = default;

    /**
     * @brief Registers all built-in functionality into the SemaContext.
     * This includes built-in operators and conversions.
     * This function should be called during initialization to allow
     * built-in functionality to be used during semantic analysis.
     */
    void registerAll();

private:
    SemaContext& _sema;

    void registerBuiltinOperators();
    void registerBuiltinConversions();
};

} // namespace sema
VEEC_NAMESPACE_END
