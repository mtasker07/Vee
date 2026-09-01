/**
 * @file MirTypeConverter.hpp
 * @brief This file contains the definition of the MirTypeConverter class.
 * The MirTypeConverter class is responsible for converting Vee types to MIR types.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/mir/MirFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mirgen {

/**
 * @class MirTypeConverter
 * @brief A class that converts Vee types to MIR types.
 */
class MirTypeConverter {
public:
    MirTypeConverter(compilation::CompilationContext& ctx)
        : _ctx(ctx) {}

    ~MirTypeConverter() = default;

    /**
     * @brief Converts a Vee type to a MIR type.
     * @param type The Vee type to convert.
     * @return The corresponding MIR type.
     */
    const mir::MirType* convert(const types::Type* type);

private:
    compilation::CompilationContext& _ctx;

    const mir::MirType* convertBuiltinType(const types::BuiltinType* type);
    const mir::MirPointerType* convertPointerType(const types::PointerType* type);
    const mir::MirArrayType* convertArrayType(const types::ArrayType* type);
    const mir::MirStructType* convertStructType(const types::ClassType* type);
};

} // namespace mirgen
VEEC_NAMESPACE_END
