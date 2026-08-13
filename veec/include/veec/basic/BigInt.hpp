/**
 * @file BigInt.hpp
 * @brief This file contains the definition of the BigInt class.
 */

#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @brief Represents an arbitrary-precision integer.
 * Slower than APInt but can handle integers of any given size.
 * Stores value as vector of u32 words in little-endian order.
 */
class BigInt {
public:
    BigInt()
        : _negative(false) {}

    ~BigInt() = default;

    /**
     * @brief Creates a new BigInt instance from a 64-bit unsigned integer.
     * @param value 64-bit unsigned integer
     * @param negative Whether the number is negative (default false)
     * @return BigInt instance representing the value
     */
    static BigInt fromU64(u64 value, bool negative = false);
    /**
     * @brief Creates a new BigInt instance from a string representation.
     * The string must consist of valid digits for the specified
     * base ONLY. Leading 0s and whitespace are ignored.
     * Do not include a sign in the string, use the `negative`
     * parameter instead.
     * @param str String representation of the integer.
     * @param base Base for conversion (2-36, default 10).
     * @param negative Whether the number is negative (default false).
     * @return BigInt instance representing the value.
     */
    static BigInt fromString(std::string_view str, u32 base = 10, bool negative = false);
    /**
     * @brief Creates a new BigInt instance from a decimal string representation.
     * @param decimalStr Decimal string representation of the integer.
     * @param negative Whether the number is negative (default false).
     * @return BigInt instance representing the value.
     */
    static BigInt fromDecimal(std::string_view decimalStr, bool negative = false);
    /**
     * @brief Creates a new BigInt instance from a hexadecimal string representation.
     * @param hexStr Hexadecimal string representation of the integer.
     * @param negative Whether the number is negative (default false).
     * @return BigInt instance representing the value.
     */
    static BigInt fromHex(std::string_view hexStr, bool negative = false);
    /**
     * @brief Creates a new BigInt instance from a binary string representation.
     * @param binaryStr Binary string representation of the integer.
     * @param negative Whether the number is negative (default false).
     * @return BigInt instance representing the value.
     */
    static BigInt fromBinary(std::string_view binaryStr, bool negative = false);

    /**
     * @brief Check if value is zero.
     * @return true if value is 0, false otherwise.
     */
    bool isZero() const;
    /**
     * @brief Check if value is negative.
     * @return true if value < 0, false otherwise.
     */
    inline bool isNegative() const {
        return _negative;
    }
    /**
     * @brief Check if value is positive.
     * @return true if value > 0, false otherwise.
     */
    inline bool isPositive() const {
        return !_negative && !isZero();
    }

    /**
     * @brief Check if value fits in a signed integer of the given bit width.
     * @param bitWidth The bit width to check against.
     */
    bool fitsInSigned(u32 bitWidth) const;
    /**
     * @brief Check if value fits in an unsigned integer of the given bit width.
     * @param bitWidth The bit width to check against.
     */
    bool fitsInUnsigned(u32 bitWidth) const;

	/**
	 * @brief Converts this BigInt to an i8. Asserts if this value does not fit in an i8.
	 * @return An i8 with the value of this BigInt.
	 */
	i8 toI8() const;
	/**
	 * @brief Converts this BigInt to an i16. Asserts if this value does not fit in an i16.
	 * @return An i16 with the value of this BigInt.
	 */
	i16 toI16() const;
	/**
	 * @brief Converts this BigInt to an i32. Asserts if this value does not fit in an i32.
	 * @return An i32 with the value of this BigInt.
	 */
	i32 toI32() const;
	/**
	 * @brief Converts this BigInt to an i64. Asserts if this value does not fit in an i64.
	 * @return An i64 with the value of this BigInt.
	 */
	i64 toI64() const;

    /**
     * @brief Converts this BigInt to a u8. Asserts if this value does not fit in a u8.
     * @return A u8 with the value of this BigInt.
     */
    u8 toU8() const;
    /**
     * @brief Converts this BigInt to a u16. Asserts if this value does not fit in a u16.
     * @return A u16 with the value of this BigInt.
     */
    u16 toU16() const;
    /**
     * @brief Converts this BigInt to a u32. Asserts if this value does not fit in a u32.
     * @return A u32 with the value of this BigInt.
     */
    u32 toU32() const;
    /**
     * @brief Converts this BigInt to a u64. Asserts if this value does not fit in a u64.
     * @return A u64 with the value of this BigInt.
     */
    u64 toU64() const;

    /**
     * @brief Converts this BigInt to a human-readable string.
     * @param radix Base for conversion (2-36, default 10).
     * @return A string representation of the value.
     */
    std::string toString(u32 radix = 10) const;

    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator!=(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<=(const BigInt& lhs, const BigInt& rhs);
    friend bool operator>(const BigInt& lhs, const BigInt& rhs);
    friend bool operator>=(const BigInt& lhs, const BigInt& rhs);

    friend BigInt operator+(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator-(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator*(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator/(const BigInt& lhs, const BigInt& rhs);
    
    friend BigInt operator&(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator|(const BigInt& lhs, const BigInt& rhs);
    friend BigInt operator^(const BigInt& lhs, const BigInt& rhs);

private:
    std::vector<u32> _words;
    bool _negative;

    BigInt(bool negative, std::vector<u32> words)
        : _negative(negative), _words(std::move(words)) {}

    // Helper methods
    static BigInt addAbsolute(const BigInt& lhs, const BigInt& rhs);
    static BigInt subtractAbsolute(const BigInt& lhs, const BigInt& rhs);
    static BigInt multiplyAbsolute(const BigInt& lhs, const BigInt& rhs);
    static BigInt divideAbsolute(const BigInt& lhs, const BigInt& rhs);
    static int compareAbsolute(const BigInt& lhs, const BigInt& rhs);
    
    /// Push word
    inline void pushWord(u32 word) {
        _words.push_back(word);
    }
    /// Remove leading zero words
    void normalize();
};

} // namespace basic
VEEC_NAMESPACE_END
