/**
 * @file CCodegenContext.hpp
 * @brief This file contains the definition of the CCodegenContext class which is
 * used to maintain context during C code generation.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {

/**
 * @class CCodegenContext
 * @brief Maintains context during C code generation.
 */
class CCodegenContext {
public:
    basic::Arena<> constructArena;

    CCodegenContext() = default;
    ~CCodegenContext() = default;
};

} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
