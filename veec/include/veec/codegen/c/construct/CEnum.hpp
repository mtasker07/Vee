/**
 * @file CEnum.hpp
 * @brief This file contains the definition of the CEnum struct which represents
 * a C enum construct.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/CConstruct.hpp"
#include "veec/codegen/c/construct/stmt/CBlockStmt.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CEnumMember
 * @brief Represents a C enum member construct.
 */
class CEnumMember : public CConstruct {
public:
    std::string name;
    CExpr* value;

    virtual ~CEnumMember() = default;
};

/**
 * @class CEnum
 * @brief Represents a C enum construct.
 */
class CEnum : public CConstruct {
public:
    std::string name;
    CType* underlyingType;
    std::vector<CEnumMember*> members;
    
    virtual ~CEnum() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
