/**
 * @file Result.hpp
 * @brief This file contains the definition of the Result class.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @brief Represents the result of an operation that can either succeed
 * with a value of type T or fail with an error of type E.
 */
template<typename T, typename E>
class Result {
public:
    /**
     * @brief Constructs a successful Result with the given value.
     * @param value The value of the successful result.
     */
    Result(T value) : _value(value), _hasError(false) {}
    /**
     * @brief Constructs a failed Result with the given error.
     * @param error The error of the failed result.
     */
    Result(E error) : _error(error), _hasError(true) {}

    /**
     * @brief Checks if the Result represents a failure.
     * @return True if the Result is a failure, false otherwise.
     */
    bool hasError() const { return _hasError; }
    /**
     * @brief Checks if the Result represents a success.
     * @return True if the Result is a success, false otherwise.
     */
    T getValue() const { return _value; }
    /**
     * @brief Gets the error of the Result.
     * @return The error of the Result.
     */
    E getError() const { return _error; }

private:
    T _value;
    E _error;
    bool _hasError;
};

} // namespace basic
VEEC_NAMESPACE_END
