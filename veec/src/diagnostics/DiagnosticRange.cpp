#include "veec/diagnostics/DiagnosticRange.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/source/SourceRange.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

DiagnosticRange DiagnosticRange::fromSourceRange(const source::SourceRange& range) {
    return DiagnosticRange(range.getFile(), range.getBegin(), range.getEnd());
}

} // namespace diagnostics
VEEC_NAMESPACE_END
