/**
 * @file CLIValue.hpp
 * @brief This file contains the definition of the CLIValue class and related types.
 * 
 * The CLIValue class is a wrapper class around a generic command-line value.
 * 
 * By default, CLI values do not have a type and are stored as plain strings. However,
 * type checking can be done by using the `canBe` method, which checks if the value can be
 * interpreted as a specific type. This keeps the class simple and allows flexibility. For example,
 * an option that takes a float can accept an integer value without requiring any explicit
 * conversion semantics that overcomplicate the system.
 */

#pragma once

#include <string_view>
#include <variant>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/BigInt.hpp"

VEEC_NAMESPACE_BEGIN
namespace cli {

/**
 * @enum CLIValueType
 * @brief Represents the type of a command-line value.
 */
enum class CLIValueType : u8 {
    None, // Special sentinel type

    Integer, // A whole number
    Float, // A decimal number
    String, // Text
};

/**
 * @brief Converts a CLIValueType to a string representation.
 * @param type The CLIValueType to convert.
 * @return A string representation of the CLIValueType.
 */
inline std::string_view toString(CLIValueType type) {
    switch (type) {
        case CLIValueType::None:
            return "none";
        case CLIValueType::Integer:
            return "integer";
        case CLIValueType::Float:
            return "float";
        case CLIValueType::String:
            return "string";
        default:
            VEE_UNREACHABLE("Invalid CLIValueType");
    }
}

/**
 * @class CLIValue
 * @brief Represents a generic command-line value.
 */
class CLIValue {
public:
    CLIValue() = default;
    CLIValue(std::string_view value)
        : _value(value) {}

    inline bool canBe(CLIValueType type) const {
        VEE_ASSERT(type != CLIValueType::None, "Cannot check for None type");

        switch (type) {
            case CLIValueType::Integer:
                return isInteger();
            case CLIValueType::Float:
                return isFloat();
            case CLIValueType::String:
                return true;
                // ^^ Any value can be treated as a string

            default:
                VEE_UNREACHABLE("Invalid CLIValueType");
        }
    }

    inline basic::BigInt getIntegerValue() const {
        VEE_ASSERT(canBe(CLIValueType::Integer), "Value cannot be converted to integer");
        return basic::BigInt::fromString(_value, 10);
    }
    inline double getFloatValue() const {
        VEE_ASSERT(canBe(CLIValueType::Float), "Value cannot be converted to float");
        return std::stod(std::string(_value));
    }
    inline std::string_view getStringValue() const {
        // Anything can be a string, so no need to check
        return _value;
    }

private:
    std::string_view _value;

    // Is _value a valid integer?
    bool isInteger() const;
    // Is _value a valid float?
    // Note: returns true for integers aswell, no need to double check
    bool isFloat() const;
};

} // namespace cli
VEEC_NAMESPACE_END
