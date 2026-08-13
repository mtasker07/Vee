/**
 * @file Terminal.hpp
 * @brief This file contains the definition of the Terminal class.
 * 
 */

#pragma once

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

enum class TerminalCapability {

};

/**
 * @class Terminal
 * @brief Wraps the terminal.
 */
class Terminal {
public:
    virtual ~Terminal() = default;

private:
    Terminal() = default;
};

} // namespace io
VEEC_NAMESPACE_END
