/**
 * @file IWriter.hpp
 * @brief This file contains the definition of the IWriter interface.
 * 
 * The IWriter interface is used for writing text output to various destinations, such as files, strings,
 * or even other output streams.
 */

#pragma once

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

/**
 * @class IWriter
 * @brief An interface for writing text output to various destinations.
 */
class IWriter {
public:
    virtual ~IWriter() = default;

    /**
     * @brief Writes the given string view to the output.
     * @param str The string view to write.
     */
    virtual void write(std::string_view str) = 0;

    /**
     * @brief Flushes the output buffer, if this writer is buffered.
     */
    virtual void flush() = 0;
};

} // namespace io
VEEC_NAMESPACE_END
