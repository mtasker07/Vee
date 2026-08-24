/**
 * @file StdStreams.hpp
 * @brief This file contains the definition of the standard stream writers.
 * 
 * This includes writers for standard output, standard error, standard log, and a null writer
 * that discards all text output.
 */

#pragma once

#include <iostream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/io/StreamWriter.hpp"
#include "veec/io/NullWriter.hpp"

VEEC_NAMESPACE_BEGIN
namespace io {

/**
 * @brief Gets a reference to the standard output stream writer.
 * @return A reference to the standard output stream writer.
 */
inline StreamWriter& getStdOut() {
    static StreamWriter stdOutWriter(std::cout);
    return stdOutWriter;
}
/**
 * @brief Gets a reference to the standard error stream writer.
 * @return A reference to the standard error stream writer.
 */
inline StreamWriter& getStdErr() {
    static StreamWriter stdErrWriter(std::cerr);
    return stdErrWriter;
}
/**
 * @brief Gets a reference to the standard log stream writer.
 * @return A reference to the standard log stream writer.
 */
inline StreamWriter& getStdLog() {
    static StreamWriter stdLogWriter(std::clog);
    return stdLogWriter;
}
/**
 * @brief Gets a reference to the discarder (null) writer that discards all text output.
 * @return A reference to the discarder writer.
 */
inline NullWriter& getDiscarder() {
    static NullWriter nullWriter;
    return nullWriter;
}

} // namespace io
VEEC_NAMESPACE_END
