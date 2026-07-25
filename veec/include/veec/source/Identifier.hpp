/**
 * @file Identifier.hpp
 * @brief This file contains the definition of the Identifier struct,
 * which represents an identifier in the source code.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/source/SourceRange.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

/**
 * @struct Identifier
 * @brief Represents an identifier in the source code.
 */
struct Identifier {
    basic::StringId id = 0;
    SourceRange range;
};

} // namespace source
VEEC_NAMESPACE_END
