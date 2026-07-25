/**
 * @file SourceFile.cpp
 * @brief This file contains the implementation of the SourceFile class. 
 */

#include "veec/source/SourceFile.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace source {

void SourceFile::buildLineTable() const {
    VEE_ASSERT(_lineOffsets.empty(), "Line table should be built only once");

    _lineOffsets.clear();
    _lineOffsets.push_back(0); // First line

    for (u32 i = 0; i < _contents.size(); ++i) {
        if (_contents[i] == '\n') {
            _lineOffsets.push_back(i + 1);
        }
    }
}

} // namespace source
VEEC_NAMESPACE_END
