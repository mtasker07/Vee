/**
 * @file APInt.hpp
 * @brief This file contains the definition of the APInt class.
 */

#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <utility>
#include <bit>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/util/HashUtils.hpp"

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
     * @brief Returns an APInt instance with the value 1 for a given bit width.
     * @param bitWidth The bit width of the APInt.
     * @return An APInt instance with the value 1.
     */
    inline static APInt one(u32 bitWidth) {
        return fromU64(bitWidth, 1);
    }
    /**
     * @brief Returns an APInt instance with the value 0 for a given bit width.
     * @param bitWidth The bit width of the APInt.
     * @return An APInt instance with the value 0.
     */
    inline static APInt zero(u32 bitWidth) {
        return fromU64(bitWidth, 0);
    }

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
    friend struct std::hash<APInt>;

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

namespace std {

// Note that this hash implementation generally should only be used for hashing APInts
// that have a bit width of 64 or less, it may not be suitable for larger values.

template<>
struct hash<veec::basic::APInt> {
    size_t operator()(const veec::basic::APInt& key) const {
        u64 h = 0x9e3779b97f4a7c15ULL;

        for (u32 limb : key._words) {
            h ^= std::rotl(static_cast<u64>(limb) * 0xbf58476d1ce4e5b9ULL, 27);
            h *= 0x94d049bb133111ebULL;
        }

        if (key.isNegative())
            h ^= 0xdeadbeefcafebabeULL;

        return static_cast<size_t>(h);
    }
};

} // namespace std
