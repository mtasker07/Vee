/**
 * @file CStructType.hpp
 * @brief This file contains the definition of the CStructType struct which represents
 * a C struct type construct.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {
    
class CStruct;

/**
 * @enum CStructTypeKind
 * @brief The type of struct this is.
 */
enum class CStructTypeKind {
    /// @brief Inline struct definition (e.g. struct { ... })
    Inline,
    /// @brief Reference to a defined struct (e.g. struct MyStruct)
    ReferenceToDefined
};

/**
 * @class CStructType
 * @brief Represents a C struct type construct.
 */
class CStructType : public CType {
public:
    CStructTypeKind kind;
    CStruct* referenceToDefined;
    std::vector<CType*> inlineMembers;
    
    virtual ~CStructType() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
