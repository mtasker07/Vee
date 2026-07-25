/**
 * @file SymbolFwd.hpp
 * @brief This file contains the forward declarations of all symbol classes.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

//
// -- ENUMS --
//

enum class FunctionSymbolKind : u8;
enum class VariableSymbolStorage : u8;
enum class OperatorImplementation : u8;
enum class OperatorSymbolKind : u8;
enum class UnaryOperatorKind : u8;
enum class BinaryOperatorKind : u8;

//
// -- BASES --
//

class Symbol;
class ScopeOwnerSymbol;

//
// -- ENTITIES --
//

class ModuleSymbol;
class FunctionSymbol;
class FunctionSetSymbol;
class VariableSymbol;
class ClassSymbol;
class FieldSymbol;
class OperatorSymbol;

} // namespace symbols
VEEC_NAMESPACE_END
