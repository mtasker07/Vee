/**
 * @file CharUtils.hpp
 * @brief This file contains character utility functions.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace util {

/**
 * @namespace CharUtils
 * @brief Provides utility functions for character operations.
 */
namespace CharUtils {
    /**
     * @brief Check if a character is a digit (0-9).
     * @param ch The character to check.
     * @return true if the character is a digit, false otherwise.
     */
    bool isDigit(char ch);
    /**
     * @brief Check if a character is a hexadecimal digit (0-9, a-f, A-F).
     * @param ch The character to check.
     * @return true if the character is a hexadecimal digit, false otherwise.
     */
    bool isHexDigit(char ch);
    /**
     * @brief Check if a character is a binary digit (0 or 1).
     * @param ch The character to check.
     * @return true if the character is a binary digit, false otherwise.
     */
    bool isBinaryDigit(char ch);
    /**
     * @brief Check if a character is a sign character ('+' or '-').
     * @param ch The character to check.
     * @return true if the character is a sign character, false otherwise.
     */
    bool isSign(char ch);
    /**
     * @brief Check if a character is whitespace (space, tab, newline, etc.).
     * @param ch The character to check.
     * @return true if the character is whitespace, false otherwise.
     */
    bool isWhitespace(char ch);
    /**
     * @brief Check if a character is an uppercase letter (A-Z).
     * @param ch The character to check.
     * @return true if the character is an uppercase letter, false otherwise.
     */
    bool isUppercase(char ch);
    /**
     * @brief Check if a character is a lowercase letter (a-z).
     * @param ch The character to check.
     * @return true if the character is a lowercase letter, false otherwise.
     */
    bool isLowercase(char ch);
    /**
     * @brief Check if a character is an alphabetic letter (A-Z or a-z).
     * @param ch The character to check.
     * @return true if the character is an alphabetic letter, false otherwise.
     */
    bool isAlpha(char ch);
    /**
     * @brief Check if a character is alphanumeric (A-Z, a-z, or 0-9).
     * @param ch The character to check.
     * @return true if the character is alphanumeric, false otherwise.
     */
    bool isAlphanumeric(char ch);

    /**
     * @brief Converts a digit character to integer value (0-35)
     * @param ch The character to convert.
     * @param base The numeric base (2-36) for validation.
     * @return The numeric value of the digit character.
     */
    u32 digitValue(char ch, u32 base = 10);
    /**
     * @brief Converts an integer value (0-35) to a digit character.
     * @param val The integer value to convert.
     * @return The corresponding digit character.
     */
    char valueToDigit(u32 val);
} // namespace CharUtils

} // namespace util
VEEC_NAMESPACE_END
