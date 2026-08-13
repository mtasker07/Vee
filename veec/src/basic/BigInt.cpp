#include "veec/basic/BigInt.hpp"

#include <cmath>
#include <cctype>
#include <algorithm>
#include <bit>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/util/CharUtils.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

BigInt BigInt::fromU64(u64 value, bool negative) {
    std::vector<u32> words;
    if (value != 0) {
        words.push_back(static_cast<u32>(value));
        u32 high = static_cast<u32>(value >> 32);
        if (high != 0) {
            words.push_back(high);
        }
    }
    return BigInt(negative && value != 0, std::move(words));
}
BigInt BigInt::fromString(std::string_view str, u32 base, bool negative) {
    VEE_ASSERT(base >= 2 && base <= 36, "Base must be between 2 and 36");

    // Strip leading whitespace
    while (!str.empty() && util::CharUtils::isWhitespace(str.front())) {
        str.remove_prefix(1);
    }

    // Strip leading 0s
    while (!str.empty() && str.front() == '0') {
        str.remove_prefix(1);
    }

    if (str.empty()) {
        return BigInt::fromU64(0, negative);
    }

    BigInt result;

    for (char c : str) {
        u32 digit = util::CharUtils::digitValue(c, base);

        // result = result * base + digit
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
    
    // Normalize zeros
    result.normalize();

    // Apply sign
    if (negative && !result.isZero()) {
        result._negative = true;
    }

    return result;
}
BigInt BigInt::fromDecimal(std::string_view decimalStr, bool negative) {
    return fromString(decimalStr, 10, negative);
}
BigInt BigInt::fromHex(std::string_view hexStr, bool negative) {
    return fromString(hexStr, 16, negative);
}
BigInt BigInt::fromBinary(std::string_view binaryStr, bool negative) {
    return fromString(binaryStr, 2, negative);
}

bool BigInt::isZero() const {
    return _words.empty() || (_words.size() == 1 && _words[0] == 0);
}

bool BigInt::fitsInSigned(u32 bitWidth) const {
    if (bitWidth == 0 || bitWidth > 64) {
        VEE_FATAL("Bit width must be between 1 and 64");
    }

    if (isNegative()) {
        // For negative numbers, check if it fits in the range [-2^(bitWidth-1), -1]
        BigInt minValue = BigInt::fromU64(1ULL << (bitWidth - 1), true);
        return *this >= minValue;
    } else {
        // For positive numbers, check if it fits in the range [0, 2^(bitWidth-1)-1]
        BigInt maxValue = BigInt::fromU64((1ULL << (bitWidth - 1)) - 1);
        return *this <= maxValue;
    }
}
bool BigInt::fitsInUnsigned(u32 bitWidth) const {
    if (bitWidth == 0 || bitWidth > 64) {
        VEE_FATAL("Bit width must be between 1 and 64");
    }

    BigInt maxValue = BigInt::fromU64((1ULL << bitWidth) - 1);
    return *this <= maxValue;
}

i8 BigInt::toI8() const {
	if (!fitsInSigned(8)) {
		VEE_FATAL("Value does not fit in i8");
	}
	return static_cast<i8>(_words.empty() ? 0 : _words[0]);
}
i16 BigInt::toI16() const {
	if (!fitsInSigned(16)) {
		VEE_FATAL("Value does not fit in i16");
	}
	return static_cast<i16>(_words.empty() ? 0 : _words[0]);
}
i32 BigInt::toI32() const {
	if (!fitsInSigned(32)) {
		VEE_FATAL("Value does not fit in i32");
	}
	return static_cast<i32>(_words.empty() ? 0 : _words[0]);
}
i64 BigInt::toI64() const {
	if (!fitsInSigned(64)) {
		VEE_FATAL("Value does not fit in i64");
	}
	u64 low = _words.empty() ? 0 : _words[0];
	u64 high = _words.size() > 1 ? _words[1] : 0;
	return static_cast<i64>((high << 32) | low);
}

u8 BigInt::toU8() const {
	if (!fitsInUnsigned(8)) {
		VEE_FATAL("Value does not fit in u8");
	}
	return static_cast<u8>(_words.empty() ? 0 : _words[0]);
}
u16 BigInt::toU16() const {
	if (!fitsInUnsigned(16)) {
		VEE_FATAL("Value does not fit in u16");
	}
	return static_cast<u16>(_words.empty() ? 0 : _words[0]);
}
u32 BigInt::toU32() const {
	if (!fitsInUnsigned(32)) {
		VEE_FATAL("Value does not fit in u32");
	}
	return static_cast<u32>(_words.empty() ? 0 : _words[0]);
}
u64 BigInt::toU64() const {
	if (!fitsInUnsigned(64)) {
		VEE_FATAL("Value does not fit in u64");
	}
	u64 low = _words.empty() ? 0 : _words[0];
	u64 high = _words.size() > 1 ? _words[1] : 0;
	return (high << 32) | low;
}

std::string BigInt::toString(u32 radix) const {
    if (radix < 2 || radix > 36) {
        VEE_FATAL("Radix must be between 2 and 36");
    }
    
    if (isZero()) {
        return "0";
    }
    
    std::string result;
    BigInt temp = *this;
    temp._negative = false;  // Work with absolute value
    
    while (!temp.isZero()) {
        BigInt divisor = BigInt::fromU64(radix);
        BigInt remainder = temp - (temp / divisor) * divisor;
        
        u32 remainderVal = remainder._words.empty() ? 0 : remainder._words[0];
        result = util::CharUtils::valueToDigit(static_cast<u32>(remainderVal)) + result;
        
        temp = temp / divisor;
    }
    
    if (_negative) {
        result = "-" + result;
    }
    
    return result;
}

bool operator==(const BigInt& lhs, const BigInt& rhs) {
    if (lhs._negative != rhs._negative) return false;
    if (lhs._words.size() != rhs._words.size()) return false;
    for (size_t i = 0; i < lhs._words.size(); ++i) {
        if (lhs._words[i] != rhs._words[i]) return false;
    }
    return true;
}
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs == rhs);
}
bool operator<(const BigInt& lhs, const BigInt& rhs) {
    if (lhs._negative != rhs._negative) return lhs._negative;
    int cmp = BigInt::compareAbsolute(lhs, rhs);
    return lhs._negative ? cmp > 0 : cmp < 0;
}
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    return !(rhs < lhs);
}
bool operator>(const BigInt& lhs, const BigInt& rhs) {
    return rhs < lhs;
}
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    return !(lhs < rhs);
}

BigInt operator+(const BigInt& lhs, const BigInt& rhs) {
    // Same sign: add magnitudes
    if (lhs._negative == rhs._negative) {
        BigInt result = BigInt::addAbsolute(lhs, rhs);
        result._negative = lhs._negative;
        return result;
    }
    
    // Different signs: subtract smaller from larger
    int cmp = BigInt::compareAbsolute(lhs, rhs);
    if (cmp >= 0) {
        BigInt result = BigInt::subtractAbsolute(lhs, rhs);
        result._negative = lhs._negative;
        return result;
    } else {
        BigInt result = BigInt::subtractAbsolute(rhs, lhs);
        result._negative = rhs._negative;
        return result;
    }
}
BigInt operator-(const BigInt& lhs, const BigInt& rhs) {
    BigInt negRhs = rhs;
    negRhs._negative = !rhs._negative;
    return lhs + negRhs;
}
BigInt operator*(const BigInt& lhs, const BigInt& rhs) {
    BigInt result = BigInt::multiplyAbsolute(lhs, rhs);
    result._negative = lhs._negative != rhs._negative && !result.isZero();
    return result;
}
BigInt operator/(const BigInt& lhs, const BigInt& rhs) {
    if (rhs.isZero()) {
        VEE_FATAL("Division by zero");
    }
    
    BigInt result = BigInt::divideAbsolute(lhs, rhs);
    result._negative = lhs._negative != rhs._negative && !result.isZero();
    return result;
}

BigInt operator&(const BigInt& lhs, const BigInt& rhs) {
    // Bitwise AND only on positive numbers
    if (lhs.isNegative() || rhs.isNegative()) {
        VEE_FATAL("Bitwise AND not supported for negative numbers");
    }
    
    size_t minSize = std::min(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    
    for (size_t i = 0; i < minSize; ++i) {
        result.push_back(lhs._words[i] & rhs._words[i]);
    }
    
    return BigInt(false, std::move(result));
}
BigInt operator|(const BigInt& lhs, const BigInt& rhs) {
    // Bitwise OR only on positive numbers
    if (lhs.isNegative() || rhs.isNegative()) {
        VEE_FATAL("Bitwise OR not supported for negative numbers");
    }
    
    size_t maxSize = std::max(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    result.reserve(maxSize);
    
    for (size_t i = 0; i < maxSize; ++i) {
        u32 lword = i < lhs._words.size() ? lhs._words[i] : 0;
        u32 rword = i < rhs._words.size() ? rhs._words[i] : 0;
        result.push_back(lword | rword);
    }
    
    return BigInt(false, std::move(result));
}
BigInt operator^(const BigInt& lhs, const BigInt& rhs) {
    // Bitwise XOR only on positive numbers
    if (lhs.isNegative() || rhs.isNegative()) {
        VEE_FATAL("Bitwise XOR not supported for negative numbers");
    }
    
    size_t maxSize = std::max(lhs._words.size(), rhs._words.size());
    std::vector<u32> result;
    result.reserve(maxSize);
    
    for (size_t i = 0; i < maxSize; ++i) {
        u32 lword = i < lhs._words.size() ? lhs._words[i] : 0;
        u32 rword = i < rhs._words.size() ? rhs._words[i] : 0;
        result.push_back(lword ^ rword);
    }
    
    return BigInt(false, std::move(result));
}

BigInt BigInt::addAbsolute(const BigInt& lhs, const BigInt& rhs) {
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
    
    return BigInt(false, std::move(result));
}
BigInt BigInt::subtractAbsolute(const BigInt& lhs, const BigInt& rhs) {
    // Assumes |lhs| >= |rhs|
    std::vector<u32> result;
    result.reserve(lhs._words.size());
    
    i64 borrow = 0;
    for (size_t i = 0; i < lhs._words.size(); ++i) {
        u64 lword = lhs._words[i];
        u64 rword = i < rhs._words.size() ? rhs._words[i] : 0;
        
        i64 diff = static_cast<i64>(lword) - static_cast<i64>(rword) - borrow;
        if (diff < 0) {
            result.push_back(static_cast<u32>(diff + (1ULL << 32)));
            borrow = 1;
        } else {
            result.push_back(static_cast<u32>(diff));
            borrow = 0;
        }
    }
    
    BigInt res(false, std::move(result));
    res.normalize();
    return res;
}
BigInt BigInt::multiplyAbsolute(const BigInt& lhs, const BigInt& rhs) {
    if (lhs.isZero() || rhs.isZero()) {
        return BigInt::fromU64(0);
    }
    
    std::vector<u32> result(lhs._words.size() + rhs._words.size(), 0);
    
    for (size_t i = 0; i < lhs._words.size(); ++i) {
        u64 carry = 0;
        for (size_t j = 0; j < rhs._words.size(); ++j) {
            // Multiply and add: result[i+j] += lhs[i] * rhs[j] + carry
            u64 product = static_cast<u64>(lhs._words[i]) * rhs._words[j];
            u64 sum = static_cast<u64>(result[i + j]) + product + carry;

            result[i + j] = static_cast<u32>(sum);
            carry = sum >> 32;
        }
        if (carry) {
            result[i + rhs._words.size()] += static_cast<u32>(carry);
        }
    }
    
    BigInt res(false, std::move(result));
    res.normalize();
    return res;
}
BigInt BigInt::divideAbsolute(const BigInt& lhs, const BigInt& rhs) {
    if (rhs.isZero()) {
        VEE_FATAL("Division by zero");
    }
    
    if (compareAbsolute(lhs, rhs) < 0) {
        return BigInt::fromU64(0);
    }
    
    // Long division algorithm
    std::vector<u32> quotient;
    BigInt remainder = BigInt::fromU64(0);
    
    // Process words from most significant to least
    for (auto it = lhs._words.rbegin(); it != lhs._words.rend(); ++it) {
        remainder._words.insert(remainder._words.begin(), *it);
        remainder.normalize();
        
        // Binary search for quotient digit
        u32 qdigit = 0;
        u32 low = 0, high = UINT32_MAX;
        
        while (low <= high) {
            u32 mid = low + (high - low) / 2;
            BigInt midBigInt = BigInt::fromU64(mid);
            BigInt product = multiplyAbsolute(rhs, midBigInt);
            
            int cmp = compareAbsolute(product, remainder);
            if (cmp <= 0) {
                qdigit = mid;
                low = mid + 1;
            } else {
                if (mid == 0) break;
                high = mid - 1;
            }
        }
        
        if (qdigit > 0) {
            BigInt product = multiplyAbsolute(rhs, BigInt::fromU64(qdigit));
            remainder = subtractAbsolute(remainder, product);
        }
        
        quotient.insert(quotient.begin(), qdigit);
    }
    
    while (!quotient.empty() && quotient.back() == 0) {
        quotient.pop_back();
    }
    
    return BigInt(false, std::move(quotient));
}
int BigInt::compareAbsolute(const BigInt& lhs, const BigInt& rhs) {
    if (lhs._words.size() != rhs._words.size()) {
        return lhs._words.size() < rhs._words.size() ? -1 : 1;
    }
    
    for (int i = static_cast<int>(lhs._words.size()) - 1; i >= 0; --i) {
        if (lhs._words[i] != rhs._words[i]) {
            return lhs._words[i] < rhs._words[i] ? -1 : 1;
        }
    }
    
    return 0;
}

void BigInt::normalize() {
    while (!_words.empty() && _words.back() == 0) {
        _words.pop_back();
    }

    if (_words.empty()) {
        _negative = false;
    }
}

} // namespace basic
VEEC_NAMESPACE_END
