#include "veec/util/CharUtils.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace util {

bool CharUtils::isDigit(char ch) {
    return ch >= '0' && ch <= '9';
}
bool CharUtils::isHexDigit(char ch) {
    return (ch >= '0' && ch <= '9') ||
           (ch >= 'a' && ch <= 'f') ||
           (ch >= 'A' && ch <= 'F');
}
bool CharUtils::isBinaryDigit(char ch) {
    return ch == '0' || ch == '1';
}
bool CharUtils::isSign(char ch) {
    return ch == '+' || ch == '-';
}
bool CharUtils::isWhitespace(char ch) {
    return
        ch == ' ' ||
        ch == '\t' ||
        ch == '\n' ||
        ch == '\r' ||
        ch == '\f' ||
        ch == '\v';
}
bool CharUtils::isUppercase(char ch) {
    return ch >= 'A' && ch <= 'Z';
}
bool CharUtils::isLowercase(char ch) {
    return ch >= 'a' && ch <= 'z';
}
bool CharUtils::isAlpha(char ch) {
    return isUppercase(ch) || isLowercase(ch);
}
bool CharUtils::isAlphanumeric(char ch) {
    return isAlpha(ch) || isDigit(ch);
}

u32 CharUtils::digitValue(char ch, u32 base) {
    VEE_ASSERT(base >= 2 && base <= 36,
        "Base must be between 2 and 36");

    u32 v;

    if (ch >= '0' && ch <= '9') v = ch - '0';
    else if (ch >= 'a' && ch <= 'z') v = ch - 'a' + 10;
    else if (ch >= 'A' && ch <= 'Z') v = ch - 'A' + 10;
    else VEE_FATAL("Invalid digit");

    if (v >= base) {
        VEE_FATAL("Digit out of range for base");
    }

    return v;
}
char CharUtils::valueToDigit(u32 val) {
    if (val < 10) {
        return char('0' + val);
    }
    if (val < 36) {
        return char('a' + (val - 10));
    }
    VEE_FATAL("Invalid digit value");
}

} // namespace basic
VEEC_NAMESPACE_END
