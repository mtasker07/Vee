/**
 * @file NameMangler.hpp
 * @brief This file contains the definition of the NameMangler class which is
 * a utility class for mangling names in the code generation process.
 * 
 * Mangling uses Vee's own custom mangling schema described below:
 * 
 * Functions:
 * _VF_<name>_[<generictype1>_<generictype2>_...]_[<paramtype1>_<paramtype2>_...]
 *
 * Primitive Types:
 * void     : v
 * bool     : b
 * string   : s
 * i8       : i8
 * i16      : i16
 * i32      : i32
 * i64      : i64
 * u8       : u8
 * u16      : u16
 * u32      : u32
 * u64      : u64
 * f32      : f32
 * f64      : f64
 * 
 * Pointer Types:
 * p<mangledpointeetype>
 * 
 * Array Types:
 * a<mangledelementtype>
 * 
 * Custom Types:
 * vt_<typeid>_[<generictype1>_<generictype2>_...]
 */

#pragma once

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace support {

class NameMangler {
public:
    NameMangler() = default;
    ~NameMangler() = default;

    /**
     * @brief Mangles the name of an MIR function according to Vee's custom mangling schema.
     * @param func The function to mangle.
     * @return The mangled name of the function.
     */
    std::string mangleFunction(const mir::Function& func);
    /**
     * @brief Mangles the name of a type according to Vee's custom mangling schema.
     * @param type The type to mangle.
     * @return The mangled name of the type.
     */
    std::string mangleType(const mir::MirType* type);
};

} // namespace support
} // namespace codegen
VEEC_NAMESPACE_END
