/**
 * @file CodegenContext.hpp
 * @brief This file contains the definition of the CompilationContext struct,
 * which is used to hold context for the code generation process.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/support/NameMangler.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {

/**
 * @class CodegenContext
 * @brief Used to hold context for the code generation process.
 */
class CodegenContext {
public:
    support::NameMangler nameMangler;

    CodegenContext() = default;
    ~CodegenContext() = default;
};

} // namespace codegen
VEEC_NAMESPACE_END
