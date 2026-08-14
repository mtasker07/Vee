#include "veec/io/StringWriter.hpp"

#include <string>
#include <string_view>
#include <sstream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

void StringWriter::write(std::string_view str) {
    _stringStream << str;
}
void StringWriter::flush() {
    _stringStream.flush();
}

} // namespace io
VEEC_NAMESPACE_END
