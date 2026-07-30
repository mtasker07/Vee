/**
 * @file APInt.hpp
 * @brief This file contains the definition of the APInt class.
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

class APInt {
public:
    ~APInt() = default;

    /**
     * @brief Creates a new APInt instance from a 64-bit unsigned integer.
     * @param bitWidth The bit width of the APInt
     * @param value 64-bit unsigned integer
     * @param negative Whether the number is negative (default false)
     * @return APInt instance representing the value
     */
    static APInt fromU64(u32 bitWidth, u64 value, bool negative = false);
    /**
     * @brief Creates a new APInt instance from a string representation.
     * The string must consist of valid digits for the specified
     * base ONLY. Leading 0s and whitespace are ignored.
     * Do not include a sign in the string, use the `negative`
     * parameter instead.
     * @param bitWidth The bit width of the APInt
     * @param str String representation of the integer
     * @param base Base for conversion (2-36, default 10)
     * @param negative Whether the number is negative (default false)
     * @return APInt instance representing the value
     */
    static APInt fromString(u32 bitWidth, std::string_view str, u32 base = 10, bool negative = false);
    /**
     * @brief Creates a new APInt instance from a decimal string representation.
     * @param bitWidth The bit width of the APInt.
     * @param decimalStr Decimal string representation of the integer.
     * @param negative Whether the number is negative (default false).
     * @return APInt instance representing the value.
     */
    static APInt fromDecimal(u32 bitWidth, std::string_view decimalStr, bool negative = false);
    /**
     * @brief Creates a new APInt instance from a hexadecimal string representation.
     * @param bitWidth The bit width of the APInt.
     * @param hexStr Hexadecimal string representation of the integer.
     * @param negative Whether the number is negative (default false).
     * @return APInt instance representing the value.
     */
    static APInt fromHex(u32 bitWidth, std::string_view hexStr, bool negative = false);
    /**
     * @brief Creates a new APInt instance from a binary string representation.
     * @param bitWidth The bit width of the APInt.
     * @param binaryStr Binary string representation of the integer.
     * @param negative Whether the number is negative (default false).
     * @return APInt instance representing the value.
     */
    static APInt fromBinary(u32 bitWidth, std::string_view binaryStr, bool negative = false);

    /**
     * @brief Returns the maximum value for a given bit width.
     * @param bitWidth The bit width of the APInt.
     * @return APInt instance representing the maximum value.
     */
    static APInt max(u32 bitWidth);

    /**
     * @brief Gets the bit width of this APInt.
     * @return The bit width of this APInt.
     */
    inline u32 getBitWidth() const {
        return _bitWidth;
    }

    /**
     * @brief Checks if this APInt is zero.
     * @return true if this APInt is zero, false otherwise.
     */
    bool isZero() const;
    /**
     * @brief Checks if this APInt is negative.
     * @return true if this APInt < 0, false otherwise.
     */
    bool isNegative() const;
    /**
     * @brief Checks if this APInt is positive.
     * @return true if this APInt > 0, false otherwise.
     */
    inline bool isPositive() const {
        return !isNegative() && !isZero();
    }

    /**
     * @brief Converts this APInt to a human-readable string.
     * @param radix Base for conversion (2-36, default 10).
     * @return A string representation of the value.
     */
    std::string toString(u32 radix = 10) const;

    friend bool operator==(const APInt& lhs, const APInt& rhs);
    friend bool operator!=(const APInt& lhs, const APInt& rhs);
    friend bool operator<(const APInt& lhs, const APInt& rhs);
    friend bool operator<=(const APInt& lhs, const APInt& rhs);
    friend bool operator>(const APInt& lhs, const APInt& rhs);
    friend bool operator>=(const APInt& lhs, const APInt& rhs);

    friend APInt operator+(const APInt& lhs, const APInt& rhs);
    friend APInt operator-(const APInt& lhs, const APInt& rhs);
    friend APInt operator*(const APInt& lhs, const APInt& rhs);
    friend APInt operator/(const APInt& lhs, const APInt& rhs);

    friend APInt operator&(const APInt& lhs, const APInt& rhs);
    friend APInt operator|(const APInt& lhs, const APInt& rhs);
    friend APInt operator^(const APInt& lhs, const APInt& rhs);

private:
    u32 _bitWidth;
    std::vector<u32> _words;

    /// Create from raw width and limbs.
    APInt(u32 bitWidth, std::vector<u32> words)
        : _bitWidth(bitWidth), _words(std::move(words)) {}

    static APInt addUnsigned(const APInt& lhs, const APInt& rhs);
    static APInt subtractUnsigned(const APInt& lhs, const APInt& rhs);
    static APInt multiplyUnsigned(const APInt& lhs, const APInt& rhs);
    static APInt divideUnsigned(const APInt& lhs, const APInt& rhs);
    static int compareUnsigned(const APInt& lhs, const APInt& rhs);
    static APInt twosComplement(const APInt& value);

    // Helper methods
    /// Return limb count for bit width.
    static u32 limbCount(u32 bitWidth);
    /// Return mask for top limb.
    u32 topMask() const;
    /// Resize and mask to configured width.
    void applyWidthMask();
    /// Check sign bit in top limb.
    bool hasSignBit() const;
    /// Append raw limb.
    inline void pushWord(u32 word) {
        _words.push_back(word);
    }
};

} // namespace basic
VEEC_NAMESPACE_END
