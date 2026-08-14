/**
 * @file FileWriter.hpp
 * @brief This file contains the definition of the FileWriter class.
 *
 * The FileWriter class is used for writing text output to a file.
 */

#pragma once

#include <fstream>
#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/fs/Path.hpp"
#include "veec/io/IWriter.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

/**
 * @class FileWriter
 * @brief A writer for writing text output to a file.
 */
class FileWriter : public IWriter {
public:
    /**
     * @brief Creates a new FileWriter instance.
     * @param filePath The path to the file to write to.
     * @param mode The file open mode. Must be an output mode.
     * @param createDirectories If true, will create any missing directories
     * in the path. Otherwise, will assert if the directories do not exist.
     */
    FileWriter(
        const fs::Path& filePath,
        std::ios_base::openmode mode = std::ios_base::out,
        bool createDirectories = true
    ) {
        open(filePath, mode, createDirectories);
    }

    virtual ~FileWriter() {
        close();
    }

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
    std::ofstream _fileStream;

    void open(
        const fs::Path& filePath,
        std::ios_base::openmode mode = std::ios_base::out,
        bool createDirectories = true
    );
    void close();
};

} // namespace io
VEEC_NAMESPACE_END
