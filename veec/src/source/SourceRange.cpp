/**
 * @file SourceRange.cpp
 * @brief This file contains the implementation of the SourceRange class.
 */

#include "veec/source/SourceRange.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceFile.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

std::string_view SourceRange::getText() const {
    return _file->getText(*this);
}

} // namespace source
VEEC_NAMESPACE_END
