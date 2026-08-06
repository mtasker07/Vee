/**
 * @file TokenList.hpp
 * @brief This file contains the definition of the TokenList struct,
 * which holds a list of tokens generated from a source file.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Token.hpp"
#include "veec/source/SourceFileId.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @brief Represents a list of tokens to be parsed, along with the source file they originate from.
 */
struct TokenList {
    /**
     * @brief The ID of the source file from which these tokens were generated.
     */
    source::SourceFileId fileId;
    /**
     * @brief The list of tokens generated from this file.
     */
    std::vector<Token> tokens;
};

} // namespace basic
VEEC_NAMESPACE_END
