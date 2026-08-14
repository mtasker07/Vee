/**
 * @file StringWriter.hpp
 * @brief This file contains the definition of the StringWriter class.
 *
 * The StringWriter class is used for writing text output to a string.
 */

#pragma once

#include <string>
#include <string_view>
#include <sstream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/io/IWriter.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

/**
 * @class StringWriter
 * @brief A writer for writing text output to a string.
 */
class StringWriter : public IWriter {
public:
    /**
     * @brief Constructs a new StringWriter instance.
     */
    StringWriter()
        : _stringStream() {}

    virtual ~StringWriter() = default;

    /**
     * @brief Writes the given string view to the string.
     * @param str The string view to write.
     */
    void write(std::string_view str) override;
    /**
     * @brief Flushes the underlying string stream. Doesn't really
     * do anything for string writer.
     */
    void flush() override;

    /**
     * @brief Returns the contents of the string stream as a std::string.
     * @return The contents of the string stream.
     */
    inline std::string str() const {
        return _stringStream.str();
    }

private:
    std::ostringstream _stringStream;
};

} // namespace io
VEEC_NAMESPACE_END
