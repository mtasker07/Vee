/**
 * @file NullWriter.hpp
 * @brief This file contains the definition of the NullWriter class.
 * 
 * The NullWriter class is used for discarding all text output. This is typically used
 * as a way to pass "null" to an IWriter&, hence the name.
 */

#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/io/IWriter.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

/**
 * @class NullWriter
 * @brief A writer that discards all its throughput.
 */
class NullWriter : public IWriter {
public:
    /**
     * @brief Constructs a new NullWriter instance.
     */
    NullWriter() = default;

    virtual ~NullWriter() = default;

    /**
     * @brief Writes the given string view, but discards it.
     * @param str The string view to write (will be discarded).
     */
    void write(std::string_view) override {}
    /**
     * @brief This does nothing, as the NullWriter does not buffer any output.
     */
    void flush() override {}
};

} // namespace io
VEEC_NAMESPACE_END
