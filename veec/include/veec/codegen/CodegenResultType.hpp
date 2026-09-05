/**
 * @file CodegenResultType.hpp
 * @brief This file contains the definition of the CodegenResultType enum which represents
 * the type of a code generation result.
 * 
 * Different backends are allowed to produce different types of output, and this enum
 * differentiates those types. For example, the C backend would produce LanguageCode because
 * it emits text in the form of another programming language. In the case of something like
 * a bytecode backend, it would produce BinaryIR.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {

/**
 * @brief Represents the type of a code generation result.
 */
enum class CodegenResultType : u32 {
    /**
     * @brief Language code (text).
     */
    LanguageCode,
    /**
     * @brief Textual intermediate representation.
     */
    TextualIR,
    /**
     * @brief Textual assembly code.
     */
    TextualAssembly,
    /**
     * @brief Binary encoded intermediate representation.
     */
    BinaryIR,
    /**
     * @brief Binary encoded assembly code.
     */
    BinaryAssembly,
};

} // namespace codegen
VEEC_NAMESPACE_END
