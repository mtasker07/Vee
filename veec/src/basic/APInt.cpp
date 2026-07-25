#include "veec/basic/APInt.hpp"

#include <algorithm>
#include <cctype>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/util/CharUtils.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

namespace {

size_t trimmedSize(const std::vector<u32>& words) {
    size_t size = words.size();
    while (size > 0 && words[size - 1] == 0) {
        --size;
    }
    return size;
}

} // namespace

APInt APInt::fromU64(u32 bitWidth, u64 value, bool negative) {
    std::vector<u32> words;
    u32 count = limbCount(bitWidth);

    if (count > 0) {
        words.resize(count, 0);
        words[0] = static_cast<u32>(value);

        if (count > 1) {
            words[1] = static_cast<u32>(value >> 32);
        }
    }

    APInt result(bitWidth, std::move(words));
    result.applyWidthMask();

    if (negative) {
        result = twosComplement(result);
        result.applyWidthMask();
    }

    return result;
}
APInt APInt::fromString(u32 bitWidth, std::string_view str, u32 base, bool negative) {
    VEE_ASSERT(base >= 2 && base <= 36, "Base must be between 2 and 36");

    while (!str.empty() && util::CharUtils::isWhitespace(str.front())) {
        str.remove_prefix(1);
    }

    while (!str.empty() && str.front() == '0') {
        str.remove_prefix(1);
    }

    if (str.empty()) {
        return APInt::fromU64(bitWidth, 0, negative);
    }

    APInt result(bitWidth, {});

    for (char c : str) {
        u32 digit = util::CharUtils::digitValue(c, base);
        u64 carry = digit;

        for (size_t i = 0; i < result._words.size(); ++i) {
            u64 cur = static_cast<u64>(result._words[i]) * base + carry;
            result._words[i] = static_cast<u32>(cur);
            carry = cur >> 32;
        }

        if (carry) {
            result._words.push_back(static_cast<u32>(carry));
        }
    }

    result.applyWidthMask();

    if (negative) {
        result = twosComplement(result);
        result.applyWidthMask();
    }

    return result;
}
APInt APInt::fromDecimal(u32 bitWidth, std::string_view decimalStr, bool negative) {
    return fromString(bitWidth, decimalStr, 10, negative);
}
APInt APInt::fromHex(u32 bitWidth, std::string_view hexStr, bool negative) {
    return fromString(bitWidth, hexStr, 16, negative);
}
APInt APInt::fromBinary(u32 bitWidth, std::string_view binaryStr, bool negative) {
    return fromString(bitWidth, binaryStr, 2, negative);
}

APInt APInt::max(u32 bitWidth) {
    if (bitWidth == 0) {
        return APInt(0, {});
    }

    u32 count = limbCount(bitWidth);
    std::vector<u32> words(count, 0xFFFFFFFFu);

    if (bitWidth % 32 != 0) {
        words[count - 1] = (1u << (bitWidth % 32)) - 1;
    }

    return APInt(bitWidth, std::move(words));
}

bool APInt::isZero() const {
    return trimmedSize(_words) == 0;
}
bool APInt::isNegative() const {
    return hasSignBit();
}

std::string APInt::toString(u32 radix) const {
    if (radix < 2 || radix > 36) {
        VEE_FATAL("Radix must be between 2 and 36");
    }

    if (isZero()) {
        return "0";
    }

    APInt temp = *this;

    std::string result;
    APInt divisor = APInt::fromU64(temp._bitWidth, static_cast<u64>(radix));

    while (!temp.isZero()) {
        APInt quotient = divideUnsigned(temp, divisor);
        APInt product = multiplyUnsigned(quotient, divisor);
        APInt remainder = subtractUnsigned(temp, product);

        u32 remainderVal = remainder._words.empty() ? 0 : remainder._words[0];
        result = util::CharUtils::valueToDigit(remainderVal) + result;
        temp = quotient;
    }

    return result;
}

APInt operator+(const APInt& lhs, const APInt& rhs) {
    return APInt::addUnsigned(lhs, rhs);
}
APInt operator-(const APInt& lhs, const APInt& rhs) {
    return APInt::subtractUnsigned(lhs, rhs);
}
APInt operator*(const APInt& lhs, const APInt& rhs) {
    return APInt::multiplyUnsigned(lhs, rhs);
}
APInt operator/(const APInt& lhs, const APInt& rhs) {
    if (rhs.isZero()) {
        VEE_FATAL("Division by zero");
    }

    // Signed division over raw twos complement limbs
    bool negative = lhs.isNegative() != rhs.isNegative();
    APInt lhsAbs = lhs.isNegative() ? APInt::twosComplement(lhs) : lhs;
    APInt rhsAbs = rhs.isNegative() ? APInt::twosComplement(rhs) : rhs;

    APInt quotient = APInt::divideUnsigned(lhsAbs, rhsAbs);

    if (negative) {
        quotient = APInt::twosComplement(quotient);
    }

    quotient.applyWidthMask();
    return quotient;
}

APInt operator&(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    size_t maxSize = std::max(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    result.reserve(maxSize);

    for (size_t i = 0; i < maxSize; ++i) {
        u32 lword = i < lhs._words.size() ? lhs._words[i] : 0;
        u32 rword = i < rhs._words.size() ? rhs._words[i] : 0;
        result.push_back(lword & rword);
    }

    APInt value(lhs._bitWidth, std::move(result));
    value.applyWidthMask();
    return value;
}
APInt operator|(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    size_t maxSize = std::max(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    result.reserve(maxSize);

    for (size_t i = 0; i < maxSize; ++i) {
        u32 lword = i < lhs._words.size() ? lhs._words[i] : 0;
        u32 rword = i < rhs._words.size() ? rhs._words[i] : 0;
        result.push_back(lword | rword);
    }

    APInt value(lhs._bitWidth, std::move(result));
    value.applyWidthMask();
    return value;
}
APInt operator^(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    size_t maxSize = std::max(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    result.reserve(maxSize);

    for (size_t i = 0; i < maxSize; ++i) {
        u32 lword = i < lhs._words.size() ? lhs._words[i] : 0;
        u32 rword = i < rhs._words.size() ? rhs._words[i] : 0;
        result.push_back(lword ^ rword);
    }

    APInt value(lhs._bitWidth, std::move(result));
    value.applyWidthMask();
    return value;
}

u32 APInt::limbCount(u32 bitWidth) {
    return bitWidth == 0 ? 0 : (bitWidth + 31) / 32;
}
u32 APInt::topMask() const {
    if (_bitWidth == 0) {
        return 0;
    }

    u32 rem = _bitWidth % 32;
    if (rem == 0) {
        return 0xFFFFFFFFu;
    }

    return (1u << rem) - 1;
}

void APInt::applyWidthMask() {
    u32 count = limbCount(_bitWidth);

    if (count == 0) {
        _words.clear();
        return;
    }

    _words.resize(count, 0);
    _words.back() &= topMask();
}
bool APInt::hasSignBit() const {
    if (_bitWidth == 0 || _words.empty()) {
        return false;
    }

    u32 rem = _bitWidth % 32;
    u32 signMask = rem == 0 ? 0x80000000u : (1u << (rem - 1));
    return (_words.back() & signMask) != 0;
}

APInt APInt::twosComplement(const APInt& value) {
    APInt result = value;

    u32 count = limbCount(result._bitWidth);
    if (count == 0) {
        return result;
    }

    // Invert then add 1 inside width
    result._words.resize(count, 0);
    for (u32& word : result._words) {
        word = ~word;
    }

    u64 carry = 1;
    for (size_t i = 0; i < result._words.size(); ++i) {
        u64 sum = static_cast<u64>(result._words[i]) + carry;
        result._words[i] = static_cast<u32>(sum);
        carry = sum >> 32;
    }

    result.applyWidthMask();
    return result;
}

APInt APInt::addUnsigned(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    size_t maxSize = std::max(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    result.reserve(maxSize + 1);

    u64 carry = 0;
    for (size_t i = 0; i < maxSize; ++i) {
        u64 lword = i < lhs._words.size() ? lhs._words[i] : 0;
        u64 rword = i < rhs._words.size() ? rhs._words[i] : 0;

        u64 sum = lword + rword + carry;
        result.push_back(static_cast<u32>(sum));
        carry = sum >> 32;
    }

    if (carry) {
        result.push_back(static_cast<u32>(carry));
    }

    APInt sum(lhs._bitWidth, std::move(result));
    sum.applyWidthMask();
    return sum;
}
APInt APInt::subtractUnsigned(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    size_t maxSize = std::max(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    result.reserve(maxSize);

    u64 borrow = 0;
    for (size_t i = 0; i < maxSize; ++i) {
        u64 lword = i < lhs._words.size() ? lhs._words[i] : 0;
        u64 rword = i < rhs._words.size() ? rhs._words[i] : 0;

        u64 subtrahend = rword + borrow;
        if (lword < subtrahend) {
            result.push_back(static_cast<u32>((1ULL << 32) + lword - subtrahend));
            borrow = 1;
        } else {
            result.push_back(static_cast<u32>(lword - subtrahend));
            borrow = 0;
        }
    }

    APInt diff(lhs._bitWidth, std::move(result));
    diff.applyWidthMask();
    return diff;
}
APInt APInt::multiplyUnsigned(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    if (lhs.isZero() || rhs.isZero()) {
        return APInt::fromU64(lhs._bitWidth, 0);
    }

    std::vector<u32> result(lhs._words.size() + rhs._words.size(), 0);

    for (size_t i = 0; i < lhs._words.size(); ++i) {
        u64 carry = 0;
        for (size_t j = 0; j < rhs._words.size(); ++j) {
            u64 product = static_cast<u64>(lhs._words[i]) * rhs._words[j];
            u64 sum = static_cast<u64>(result[i + j]) + product + carry;

            result[i + j] = static_cast<u32>(sum);
            carry = sum >> 32;
        }

        // Drop carry past top limb; fixed width wraps
        if (i + rhs._words.size() < result.size()) {
            result[i + rhs._words.size()] += static_cast<u32>(carry);
        }
    }

    APInt product(lhs._bitWidth, std::move(result));
    product.applyWidthMask();
    return product;
}
APInt APInt::divideUnsigned(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    if (rhs.isZero()) {
        VEE_FATAL("Division by zero");
    }

    if (compareUnsigned(lhs, rhs) < 0) {
        return APInt::fromU64(lhs._bitWidth, 0);
    }

    if (lhs._bitWidth == 0) {
        return APInt::fromU64(0, 0);
    }

    u32 extBitWidth = lhs._bitWidth + 1;
    APInt divisor(extBitWidth, rhs._words);
    divisor.applyWidthMask();

    APInt remainder(extBitWidth, std::vector<u32>(limbCount(extBitWidth), 0));
    APInt quotient(lhs._bitWidth, std::vector<u32>(limbCount(lhs._bitWidth), 0));

    for (u32 bit = lhs._bitWidth; bit > 0; --bit) {
        u32 carry = 0;
        for (size_t i = 0; i < remainder._words.size(); ++i) {
            u32 nextCarry = remainder._words[i] >> 31;
            remainder._words[i] = (remainder._words[i] << 1) | carry;
            carry = nextCarry;
        }
        remainder.applyWidthMask();

        u32 sourceBit = bit - 1;
        u32 sourceWord = sourceBit / 32;
        u32 sourceShift = sourceBit % 32;
        if (sourceWord < lhs._words.size() && ((lhs._words[sourceWord] >> sourceShift) & 1u) != 0) {
            remainder._words[0] |= 1u;
        }

        if (compareUnsigned(remainder, divisor) >= 0) {
            remainder = subtractUnsigned(remainder, divisor);

            u32 quotientWord = sourceBit / 32;
            u32 quotientShift = sourceBit % 32;
            quotient._words[quotientWord] |= (1u << quotientShift);
        }
    }

    quotient.applyWidthMask();
    return quotient;
}
int APInt::compareUnsigned(const APInt& lhs, const APInt& rhs) {
    VEE_ASSERT(lhs._bitWidth == rhs._bitWidth, "Bit widths must match");

    size_t lhsSize = trimmedSize(lhs._words);
    size_t rhsSize = trimmedSize(rhs._words);

    if (lhsSize != rhsSize) {
        return lhsSize < rhsSize ? -1 : 1;
    }

    for (size_t i = lhsSize; i > 0; --i) {
        size_t index = i - 1;
        if (lhs._words[index] != rhs._words[index]) {
            return lhs._words[index] < rhs._words[index] ? -1 : 1;
        }
    }

    return 0;
}

} // namespace basic
VEEC_NAMESPACE_END
