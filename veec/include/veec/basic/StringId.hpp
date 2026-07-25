/**
 * @file StringPool.hpp
 * @brief This file contains the definition of the StringPool class.
 * 
 * The StringPool is a data structure that stores unique strings and provides
 * an efficient lookup mechanism for retrieving them using a unique integer-based identifier
 * (StringId).
 * 
 * Whilst not only saving memory, this also allows for much faster string comparisons since
 * we're just comparing an integer instead of a string.
 */

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

using StringId = u32;

} // namespace basic
VEEC_NAMESPACE_END
