#include "veec/io/StreamWriter.hpp"

#include <iostream>
#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

void StreamWriter::write(std::string_view str) {
    _outputStream << str;
}
void StreamWriter::flush() {
    _outputStream.flush();
}

} // namespace io
VEEC_NAMESPACE_END
