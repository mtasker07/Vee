/**
 * @file BuiltinRegistrar.hpp
 * @brief This file contains the definition of the BuiltinRegistrar class.
 * The BuiltinRegistrar class is responsible for registering all built-in operators
 * and conversions.
 */

#pragma once

#include <unordered_map>

#include "veec/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"

VEEC_NAMESPACE_BEGIN

class CompilationContext;

/**
 * @class BuiltinRegistrar
 * @brief Responsible for registering all built-in operators and conversions.
 */
class BuiltinRegistrar {
public:
    /**
     * @brief Creates a new BuiltinRegistrar instance with the given CompilationContext.
     * @param ctx The CompilationContext to register built-in functionality into.
     */
    BuiltinRegistrar(CompilationContext& ctx)
        : _ctx(ctx) {}

    ~BuiltinRegistrar() = default;

    /**
     * @brief Registers all built-in functionality into the SemaContext.
     * This includes built-in operators and conversions.
     * This function should be called during initialization to allow
     * built-in functionality to be used during semantic analysis.
     */
    void registerAll();

private:
    CompilationContext& _ctx;

    void registerBuiltinOperators();
    void registerBuiltinConversions();
};

VEEC_NAMESPACE_END
