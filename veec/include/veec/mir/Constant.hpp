/**
 * @file Constant.hpp
 * @brief This file contains the definition of the Constant class.
 * 
 * The Constant class represents a constant value in the MIR.
 */

#pragma once

#include <vector>
#include <string_view>
#include <variant>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/basic/APFloat.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirKind.hpp"
#include "veec/mir/MirNode.hpp"
#include "veec/mir/Value.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/util/HashUtils.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

namespace support {
    class ConstantTable;
}

/**
 * @brief Represents the kind of a constant value, e.g. integer.
 */
enum class ConstantKind : u8 {
    Int, // An integer constant
    Float, // A floating-point constant
    String, // A string constant
    Bool // A boolean constant
};

// Forward declare for conversion methods
class ConstantInt;
class ConstantFloat;
class ConstantString;
class ConstantBool;

/**
 * @class Constant
 * @brief Represents a constant value in the MIR. This can be an integer, float, string, or
 * boolean.
 */
class Constant : public Value {
public:
    ~Constant() = default;

    /**
     * @brief Gets the kind of this constant.
     * @return The kind of this constant.
     */
    inline ConstantKind getKind() const { return _kind; }

    /**
     * @brief Checks if this constant is an integer.
     * @return True if this constant is an integer, false otherwise.
     */
    inline bool isInteger() const {
        return _kind == ConstantKind::Int;
    }
    /**
     * @brief Checks if this constant is a floating-point number.
     * @return True if this constant is a float, false otherwise.
     */
    inline bool isFloat() const {
        return _kind == ConstantKind::Float;
    }
    /**
     * @brief Checks if this constant is a string.
     * @return True if this constant is a string, false otherwise.
     */
    inline bool isString() const {
        return _kind == ConstantKind::String;
    }
    /**
     * @brief Checks if this constant is a boolean.
     * @return True if this constant is a boolean, false otherwise.
     */
    inline bool isBool() const {
        return _kind == ConstantKind::Bool;
    }

    /**
     * @brief Converts this constant to a ConstantInt if it is an integer (read-only).
     * @return This constant as a ConstantInt, or nullptr if it is not an integer.
     */
    const ConstantInt* asInteger() const;
    /**
     * @brief Converts this constant to a ConstantInt if it is an integer.
     * @return This constant as a ConstantInt, or nullptr if it is not an integer.
     */
    ConstantInt* asInteger();
    /**
     * @brief Converts this constant to a ConstantFloat if it is a float (read-only).
     * @return This constant as a ConstantFloat, or nullptr if it is not a float
     */
    const ConstantFloat* asFloat() const;
    /**
     * @brief Converts this constant to a ConstantFloat if it is a float.
     * @return This constant as a ConstantFloat, or nullptr if it is not a float
     */
    ConstantFloat* asFloat();
    /**
     * @brief Converts this constant to a ConstantString if it is a string (read-only).
     * @return This constant as a ConstantString, or nullptr if it is not a string
     */
    const ConstantString* asString() const;
    /**
     * @brief Converts this constant to a ConstantString if it is a string.
     * @return This constant as a ConstantString, or nullptr if it is not a string
     */
    ConstantString* asString();
    /**
     * @brief Converts this constant to a ConstantBool if it is a boolean (read-only).
     * @return This constant as a ConstantBool, or nullptr if it is not a boolean
     */
    const ConstantBool* asBool() const;
    /**
     * @brief Converts this constant to a ConstantBool if it is a boolean.
     * @return This constant as a ConstantBool, or nullptr if it is not a boolean
     */
    ConstantBool* asBool();

protected:
    Constant(
        MirKey key,
        ConstantKind kind
    )
        : Value(key, MirKind::Constant),
        _kind(kind) {}

private:
    friend class MirFactory;
    friend class support::ConstantTable;

    ConstantKind _kind;
};

//
// CONSTANT INT
//

/**
 * @class ConstantInt
 * @brief Represents a constant integer value in the MIR.
 */
class ConstantInt : public Constant {
public:
    using KeyType = util::HashUtils::CompositeKey<types::Type*, basic::APInt>;
    using Hasher = util::HashUtils::CompositeHasher;

    ConstantInt(
        MirKey key,
        basic::APInt value
    )
        : Constant(key, ConstantKind::Int),
        _value(value) {}

    ~ConstantInt() = default;

    /**
     * @brief Gets the integer value of this constant.
     * @return The integer value of this constant.
     */
    inline const basic::APInt& getValue() const { return _value; }

private:
    friend class MirFactory;

    basic::APInt _value;
};

//
// CONSTANT FLOAT
//

/**
 * @class ConstantFloat
 * @brief Represents a constant float value in the MIR.
 */
class ConstantFloat : public Constant {
public:
    using KeyType = util::HashUtils::CompositeKey<types::Type*, double>;
    using Hasher = util::HashUtils::CompositeHasher;

    ConstantFloat(
        MirKey key,
        double value
    )
        : Constant(key, ConstantKind::Float),
        _value(value) {}

    ~ConstantFloat() = default;

    /**
     * @brief Gets the float value of this constant.
     * @return The float value of this constant.
     */
    inline double getValue() const { return _value; }

private:
    friend class MirFactory;

    double _value; // TODO: Use APFloat
};

//
// CONSTANT STRING
//

/**
 * @class ConstantString
 * @brief Represents a constant string value in the MIR.
 */
class ConstantString : public Constant {
public:
    using KeyType = util::HashUtils::CompositeKey<types::Type*, std::string_view>;
    using Hasher = util::HashUtils::CompositeHasher;

    ConstantString(
        MirKey key,
        std::string_view value
    )
        : Constant(key, ConstantKind::String),
        _value(value) {}

    ~ConstantString() = default;

    /**
     * @brief Gets the string value of this constant.
     * @return The string value of this constant.
     */
    inline std::string_view getValue() const { return _value; }

private:
    friend class MirFactory;

    std::string_view _value;
};

//
// CONSTANT BOOL
//

/**
 * @class ConstantBool
 * @brief Represents a constant boolean value in the MIR.
 */
class ConstantBool : public Constant {
public:
    // Dont bother with hashers for bools, we only have two values

    ConstantBool(
        MirKey key,
        bool value
    )
        : Constant(key, ConstantKind::Bool),
        _value(value) {}

    ~ConstantBool() = default;

    /**
     * @brief Gets the boolean value of this constant.
     * @return The boolean value of this constant.
     */
    inline bool getValue() const { return _value; }

private:
    friend class MirFactory;

    bool _value;
};

} // namespace mir
VEEC_NAMESPACE_END
