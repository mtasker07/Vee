/**
 * @file CPointerType.hpp
 * @brief This file contains the definition of the CPointerType struct which represents
 * a C pointer type construct.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CPointerType
 * @brief Represents a C pointer type construct (e.g. `int*`).
 */
class CPointerType : public CType {
public:
    CType* pointeeType;

    virtual ~CPointerType() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
