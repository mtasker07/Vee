/**
 * @file MIRBuilder.hpp
 * @brief This file contains the definition of the MIRBuilder class.
 * 
 * The MIRBuilder class is responsible for constructing the middle intermediate representation (MIR) of the compiler.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MIRContext.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class MIRBuilder
 * @brief Responsible for constructing the middle intermediate representation (MIR) of the compiler.
 */
class MIRBuilder {
public:
    MIRBuilder(MIRContext& ctx) : _ctx(ctx) {}

    

private:
    MIRContext& _ctx;
};

} // namespace mir
VEEC_NAMESPACE_END
