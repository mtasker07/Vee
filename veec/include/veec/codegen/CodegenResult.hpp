/**
 * @file CodegenResult.hpp
 * @brief This file contains the definition of the CodegenResult class which represents
 * the result of a code generation process.
 */

#pragma once

#include <string>
#include <vector>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/CodegenResultType.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {

/**
 * @class CodegenResult
 * @brief Represents the result of a code generation process.
 */
class CodegenResult {
public:
    /**
     * @brief Creates a CodegenResult with the specified type and empty data.
     */
    CodegenResult(CodegenResultType type)
        : _type(type), _data() {}

    /**
     * @brief Creates a CodegenResult with the specified type and data.
     * @param type The type of the code generation result.
     * @param data The data associated with the code generation result.
     */
    CodegenResult(CodegenResultType type, const std::string& data)
        : _type(type), _data(data) {}
    /**
     * @brief Creates a CodegenResult with the specified type and data.
     * @param type The type of the code generation result.
     * @param data The data associated with the code generation result.
     */
    CodegenResult(CodegenResultType type, std::string&& data)
        : _type(type), _data(std::move(data)) {}

    ~CodegenResult() = default;

    /**
     * @brief Returns the type of this result.
     * @return The type of this result.
     */
    CodegenResultType getType() const { return _type; }

    /**
     * @brief Returns the data associated with this result.
     * @return The data associated with this result.
     */
    const std::string& getData() const { return _data; }
    /**
     * @brief Sets the data associated with this result.
     * @param data The new data to set.
     */
    void setData(const std::string& data) { _data = data; }
    /**
     * @brief Sets the data associated with this result.
     * @param data The new data to set.
     */
    void setData(std::string&& data) { _data = std::move(data); }

    /**
     * @brief Returns whether the code generation was successful.
     * @return True if the code generation succeeded, false otherwise.
     */
    bool didSucceed() const { return _success; }
    /**
     * @brief Sets whether the code generation was successful.
     * @param success True if the code generation succeeded, false otherwise.
     */
    void setSucceeded(bool success) { _success = success; }

private:
    CodegenResultType _type;
    std::string _data;
    bool _success = false;
};

} // namespace codegen
VEEC_NAMESPACE_END
