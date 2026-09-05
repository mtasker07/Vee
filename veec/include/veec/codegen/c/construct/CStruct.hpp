/**
 * @file CStruct.hpp
 * @brief This file contains the definition of the CStruct struct which represents
 * a C struct construct.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/CConstruct.hpp"
#include "veec/codegen/c/construct/stmt/CBlockStmt.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

/**
 * @class CStructMember
 * @brief Represents a C struct member construct.
 */
class CStructMember : public CConstruct {
public:
    std::string name;
    CType* type;

    virtual ~CStructMember() = default;
};

/**
 * @class CStruct
 * @brief Represents a C struct construct.
 */
class CStruct : public CConstruct {
public:
    std::string name;
    std::vector<CStructMember*> members;
    
    virtual ~CStruct() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
