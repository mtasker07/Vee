/**
 * @file CPrimitiveType.hpp
 * @brief This file contains the definition of the CPrimitiveType struct which represents
 * a C primitive type construct.
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
 * @enum CPrimitiveTypeKind
 * @brief Enumerates all possible C primitive types.
 */
enum class CPrimitiveTypeKind : u8 {
    Void,
    Bool,
    Char,
    UnsignedChar,
    Short,
    UnsignedShort,
    Int,
    UnsignedInt,
    Long,
    UnsignedLong,
    LongLong,
    UnsignedLongLong,
    Float,
    Double,
    LongDouble,
};

/**
 * @class CPrimitiveType
 * @brief Represents a C primitive type construct.
 */
class CPrimitiveType : public CType {
public:
    CPrimitiveTypeKind kind;

    CPrimitiveType(CPrimitiveTypeKind kind)
        : kind(kind) {}
    
    virtual ~CPrimitiveType() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
