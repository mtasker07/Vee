/**
 * @file CCompilationUnit.hpp
 * @brief This file contains the definition of the CCompilationUnit struct which represents
 * a C compilation unit.
 */

#pragma once

#include <string>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/codegen/c/construct/CConstruct.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

class CFunction;
class CStruct;
class CEnum;
class CConstant;
class CTypedef;

class CCompilationUnit : public CConstruct {
public:
    std::vector<CFunction*> functions;
    std::vector<CStruct*> structs;
    std::vector<CEnum*> enums;
    std::vector<CConstant*> constants;
    std::vector<CTypedef*> typedefs;

    virtual ~CCompilationUnit() = default;
};

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
