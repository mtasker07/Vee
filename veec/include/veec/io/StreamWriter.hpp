/**
 * @file StreamWriter.hpp
 * @brief This file contains the definition of the StreamWriter class.
 * 
 * The StreamWriter class is used for writing text output to a stream.
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
 * @class StreamWriter
 * @brief A writer for writing text output to a stream.
 */
class StreamWriter : public IWriter {
public:
    /**
     * @brief Constructs a new StreamWriter instance.
     * @param outputStream The output stream to write to.
     */
    explicit StreamWriter(std::ostream& outputStream)
        : _outputStream(outputStream) {}

    virtual ~StreamWriter() = default;

    /**
     * @brief Writes the given string view to the stream.
     * @param str The string view to write.
     */
    void write(std::string_view str) override;
    /**
     * @brief Flushes the stream's output buffer.
     */
    void flush() override;
    
private:
    std::ostream& _outputStream;
};

} // namespace io
VEEC_NAMESPACE_END
