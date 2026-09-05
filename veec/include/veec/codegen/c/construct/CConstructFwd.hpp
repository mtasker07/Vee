/**
 * @file CConstructFwd.hpp
 * @brief This file contains forward declarations for C constructs.
 */

#pragma once

#include <string>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {
namespace construct {

//
// ENUMS
//

enum class CPrimitiveTypeKind : u8;

//
// STRUCTS
//

//
// NODES
//

// -- BASIC --
class CConstruct;
class CFunction;
class CStruct;
class CEnum;
class CConstant;
class CTypedef;

// -- EXPR --
class CExpr;
class CBinaryExpr;
class CUnaryExpr;
class CIdentifierExpr;
class CIntLiteralExpr;
class CFloatLiteralExpr;
class CBoolLiteralExpr;
class CStringLiteralExpr;
class CCallExpr;
class CCastExpr;
class CCompoundLiteralExpr;

// -- STMT --
class CStmt;
class CBlockStmt;
class CExprStmt;
class CAssignmentStmt;
class CCallStmt;
class CDeclStmt;
class CReturnStmt;
class CGotoStmt;
class CLabelStmt;
class CIfStmt;
class CEmptyStmt;

// -- TYPE --
class CType;
class CPrimitiveType;
class CStructType;
class CEnumType;
class CFunctionType;
class CTypedefType;
class CPointerType;

} // namespace construct
} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
