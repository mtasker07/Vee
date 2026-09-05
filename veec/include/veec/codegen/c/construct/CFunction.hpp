/**
 * @file CFunction.hpp
 * @brief This file contains the definition of the CFunction struct which represents
 * a C function construct.
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
 * @class CFunctionParam
 * @brief Represents a parameter of a C function.
 */
class CFunctionParam : public CConstruct {
public:
    std::string name;
    CType* type;

    virtual ~CFunctionParam() = default;
};

/**
 * @class CFunction
 * @brief Represents a C function construct.
 */
class CFunction : public CConstruct {
public:
    CType* returnType;
    std::string name;
    std::vector<CFunctionParam*> params;
    CBlockStmt* body;
    
    virtual ~CFunction() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
