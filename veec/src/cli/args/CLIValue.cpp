#include "veec/cli/args/CLIValue.hpp"

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/util/CharUtils.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {
namespace args {

bool CLIValue::isInteger() const {
    if (_value.empty()) {
        return false;
    }

    size_t cursor = 0;
    if (util::CharUtils::isSign(_value[cursor++]) && _value.size() == 1) {
        return false; // Sign with no value
    }

    for (; cursor < _value.size(); ++cursor) {
        if (!util::CharUtils::isDigit(_value[cursor])) {
            return false;
        }
    }

    return true;
}
bool CLIValue::isFloat() const {
    if (_value.empty()) {
        return false;
    }

    if (isInteger()) {
        return true;
        // ^^ Integers are floats
    }

    size_t cursor = 0;
    if (util::CharUtils::isSign(_value[cursor++]) && _value.size() == 1) {
        return false; // Sign with no value
    }

    bool hasDecimalPoint = false;
    for (; cursor < _value.size(); ++cursor) {
        if (util::CharUtils::isDigit(_value[cursor])) {
            continue;
        }
        if (_value[cursor] == '.' && !hasDecimalPoint) {
            hasDecimalPoint = true;
            continue;
        }
        return false;
    }

    return true;
}

} // namespace args
} // namespace cli
VEEC_NAMESPACE_END
