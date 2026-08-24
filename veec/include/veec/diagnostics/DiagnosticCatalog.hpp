/**
 * @file DiagnosticCatalog.hpp
 * @brief This file contains the diagnostic catalog for the VEE compiler.
 *
 * The diagnostic catalog represents a collection of diagnostic messages that can be displayed to the user,
 * such as errors or warnings.
 *
 * Descriptors should all be in the same format:
 * - MUST be inline constexpr
 * - MUST be of type DiagnosticDescriptor<ArgCount> where ArgCount is the number of arguments to format into the message template.
 * - MUST have a unique code for its specific kind. E.g. error codes can overlap with warning codes, but not with other error codes.
 * For everything else just take a look at the existing descriptors and follow the same format.
 * 
 * Its important to note that the diagnostic catalog is not intended to be used for internal diagnostics.
 *
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/diagnostics/UserDiagnostic.hpp"

VEEC_NAMESPACE_BEGIN
namespace diagnostics {

/**
 * @brief Represents a diagnostic message that can be reported to the user.
 * @tparam ArgCount The number of arguments to format into the message template.
 */
template<size_t ArgCount>
struct DiagnosticDescriptor {
    /// @brief The kind of diagnostic (error, warning, info).
    UserDiagnosticKind kind;
    /// @brief The unique code for this diagnostic.
    u16 code;
    /// @brief The message template for this diagnostic.
    std::string_view templateMsg;
};

/**
 * @brief Represents a diagnostic note that can be displayed to the user.
 * @tparam ArgCount The number of arguments to format into the message template.
 */
template<size_t ArgCount>
struct DiagnosticNoteDescriptor {
    /// @brief The unique code for this diagnostic.
    u16 code;
    /// @brief The message template for this diagnostic.
    std::string_view templateMsg;
};

// ------------------------------------------------------
//                        ERRORS
// ------------------------------------------------------

// -------------------- CLI ERRORS ----------------------

inline constexpr DiagnosticDescriptor<1> ERROR_CLI_UNKNOWN_COMMAND {
    UserDiagnosticKind::Error, 5001, "unknown command: {}"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CLI_UNKNOWN_OPTION {
    UserDiagnosticKind::Error, 5002, "unknown option: {}"
};
inline constexpr DiagnosticDescriptor<3> ERROR_CLI_OPTION_VALUE_TYPE_MISMATCH {
    UserDiagnosticKind::Error, 5003, "value '{}' cannot be converted to '{}' which '{}' expects"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CLI_OPTION_DUPLICATE {
    UserDiagnosticKind::Error, 5004, "option {} specified more than once"
};
inline constexpr DiagnosticDescriptor<2> ERROR_CLI_OPTION_EXPECTS_MIN_VALUES {
    UserDiagnosticKind::Error, 5005, "option {} expects at least {} value(s), but less were provided"
};
inline constexpr DiagnosticDescriptor<2> ERROR_CLI_OPTION_EXPECTS_MAX_VALUES {
    UserDiagnosticKind::Error, 5006, "option {} expects at most {} value(s), but more were provided"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CLI_OPTION_REQUIRED {
    UserDiagnosticKind::Error, 5007, "option {} required but not specified"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CLI_FILE_DOESNT_EXIST {
    UserDiagnosticKind::Error, 5008, "file '{}' does not exist"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CLI_NOT_A_FILE {
    UserDiagnosticKind::Error, 5009, "path '{}' is not a file"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CLI_DIRECTORY_DOESNT_EXIST {
    UserDiagnosticKind::Error, 5010, "directory '{}' does not exist"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CLI_NOT_A_DIRECTORY {
    UserDiagnosticKind::Error, 5011, "path '{}' is not a directory"
};
inline constexpr DiagnosticDescriptor<2> ERROR_CLI_INVALID_VALUE {
    UserDiagnosticKind::Error, 5012, "invalid value '{}' for option {}"
};
inline constexpr DiagnosticDescriptor<3> ERROR_CLI_INVALID_VALUE_WITH_MSG {
    UserDiagnosticKind::Error, 5013, "invalid value '{}' for option {}: '{}'"
};

// ------------------ LEXING ERRORS ---------------------

inline constexpr DiagnosticDescriptor<1> ERROR_UNEXPECTED_CHARACTER {
    UserDiagnosticKind::Error, 1, "unexpected character: {}"
};

// ------------------ PARSING ERRORS --------------------

inline constexpr DiagnosticDescriptor<1> ERROR_UNEXPECTED_TOKEN {
    UserDiagnosticKind::Error, 50, "unexpected token: {}"
};
inline constexpr DiagnosticDescriptor<0> ERROR_STATEMENT_EXPECTED {
    UserDiagnosticKind::Error, 51, "expected a statement"
};
inline constexpr DiagnosticDescriptor<0> ERROR_EXPRESSION_EXPECTED {
    UserDiagnosticKind::Error, 52, "expected an expression"
};
inline constexpr DiagnosticDescriptor<0> ERROR_TYPE_EXPECTED {
    UserDiagnosticKind::Error, 53, "type expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_SEMICOLON_EXPECTED {
    UserDiagnosticKind::Error, 54, "';' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_COLON_EXPECTED {
    UserDiagnosticKind::Error, 55, "':' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_COMMA_EXPECTED {
    UserDiagnosticKind::Error, 56, "',' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_DOT_EXPECTED {
    UserDiagnosticKind::Error, 57, "'.' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_AT_EXPECTED {
    UserDiagnosticKind::Error, 58, "'@' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_HASH_EXPECTED {
    UserDiagnosticKind::Error, 59, "'#' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_LPAREN_EXPECTED {
    UserDiagnosticKind::Error, 60, "'(' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_RPAREN_EXPECTED {
    UserDiagnosticKind::Error, 61, "')' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_LCURLY_EXPECTED {
    UserDiagnosticKind::Error, 62, "'{{' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_RCURLY_EXPECTED {
    UserDiagnosticKind::Error, 63, "'}}' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_LBRACK_EXPECTED {
    UserDiagnosticKind::Error, 64, "'[' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_RBRACK_EXPECTED {
    UserDiagnosticKind::Error, 65, "']' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_DOLLAR_EXPECTED {
    UserDiagnosticKind::Error, 66, "'$' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_BACKTICK_EXPECTED {
    UserDiagnosticKind::Error, 67, "'`' expected"
};
// --- OPERATORS EXPECTED ---
inline constexpr DiagnosticDescriptor<0> ERROR_EQUAL_EXPECTED {
    UserDiagnosticKind::Error, 70, "'=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_PLUS_EXPECTED {
    UserDiagnosticKind::Error, 71, "'+' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_MINUS_EXPECTED {
    UserDiagnosticKind::Error, 72, "'-' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_STAR_EXPECTED {
    UserDiagnosticKind::Error, 73, "'*' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_SLASH_EXPECTED {
    UserDiagnosticKind::Error, 74, "'/' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_PERCENT_EXPECTED {
    UserDiagnosticKind::Error, 75, "'%' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_AMPERSAND_EXPECTED {
    UserDiagnosticKind::Error, 76, "'&' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_PIPE_EXPECTED {
    UserDiagnosticKind::Error, 77, "'|' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_CARET_EXPECTED {
    UserDiagnosticKind::Error, 78, "'^' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_TILDE_EXPECTED {
    UserDiagnosticKind::Error, 79, "'~' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_BANG_EXPECTED {
    UserDiagnosticKind::Error, 80, "'!' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_LESS_EXPECTED {
    UserDiagnosticKind::Error, 81, "'<' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_GREATER_EXPECTED {
    UserDiagnosticKind::Error, 82, "'>' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_LESSEQUAL_EXPECTED {
    UserDiagnosticKind::Error, 83, "'<=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_GREATEREQUAL_EXPECTED {
    UserDiagnosticKind::Error, 84, "'>=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_EQUAL_EQUAL_EXPECTED {
    UserDiagnosticKind::Error, 85, "'==' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_BANGEQUAL_EXPECTED {
    UserDiagnosticKind::Error, 86, "'!=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_PLUS_EQUAL_EXPECTED {
    UserDiagnosticKind::Error, 87, "'+=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_MINUS_EQUAL_EXPECTED {
    UserDiagnosticKind::Error, 88, "'-=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_STAR_EQUAL_EXPECTED {
    UserDiagnosticKind::Error, 89, "'*=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_SLASH_EQUAL_EXPECTED {
    UserDiagnosticKind::Error, 90, "'/=' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_PLUSPLUS_EXPECTED {
    UserDiagnosticKind::Error, 91, "'++' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_MINUSMINUS_EXPECTED {
    UserDiagnosticKind::Error, 92, "'--' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_STARSTAR_EXPECTED {
    UserDiagnosticKind::Error, 93, "'**' expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_COLONCOLON_EXPECTED {
    UserDiagnosticKind::Error, 94, "'::' expected"
};
inline constexpr DiagnosticDescriptor<2> ERROR_KEYWORD_EXPECTED {
    UserDiagnosticKind::Error, 95, "expected '{}' keyword, got '{}'"
};
inline constexpr DiagnosticDescriptor<0> ERROR_IDENTIFIER_EXPECTED {
    UserDiagnosticKind::Error, 96, "identifier expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_MEMBER_NAME_EXPECTED {
    UserDiagnosticKind::Error, 97, "member name expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_MODULE_NAME_EXPECTED {
    UserDiagnosticKind::Error, 98, "module name expected"
};
inline constexpr DiagnosticDescriptor<0> ERROR_IMPORT_NAME_EXPECTED {
    UserDiagnosticKind::Error, 99, "import name expected"
};

// -------------------- SEMA ERRORS ---------------------

inline constexpr DiagnosticDescriptor<1> ERROR_UNKNOWN_IDENTIFIER {
    UserDiagnosticKind::Error, 500, "unknown identifier: {}"
};
inline constexpr DiagnosticDescriptor<2> ERROR_SYMBOL_CANNOT_BE_QUALIFIED {
    UserDiagnosticKind::Error, 501, "the {} '{}' cannot be qualified"
};
inline constexpr DiagnosticDescriptor<3> ERROR_NAME_ALREADY_DECLARED_IN_SCOPE {
    UserDiagnosticKind::Error, 502, "cannot declare {} '{}' because a {} with the same name already exists in this scope"
};
inline constexpr DiagnosticDescriptor<3> ERROR_NO_MEMBER_IN_SYMBOL {
    UserDiagnosticKind::Error, 503, "the {} '{}' has no member named '{}'"
};
inline constexpr DiagnosticDescriptor<2> ERROR_INT_LITERAL_OUT_OF_RANGE {
    UserDiagnosticKind::Error, 504, "integer literal '{}' is out of range for {}-bit integer"
};
inline constexpr DiagnosticDescriptor<2> ERROR_UINT_LITERAL_OUT_OF_RANGE {
    UserDiagnosticKind::Error, 505, "integer literal '{}' is out of range for {}-bit unsigned integer"
};
inline constexpr DiagnosticDescriptor<2> ERROR_UNARY_OPERATOR_NOT_FOUND {
    UserDiagnosticKind::Error, 506, "unary operator {} not found for '{}'"
};
inline constexpr DiagnosticDescriptor<3> ERROR_BINARY_OPERATOR_AMBIGUOUS {
    UserDiagnosticKind::Error, 507, "binary operator {} is ambiguous for '{}' and '{}'"
};
inline constexpr DiagnosticDescriptor<3> ERROR_BINARY_OPERATOR_NOT_FOUND {
    UserDiagnosticKind::Error, 508, "binary operator {} not found for '{}' and '{}'"
};
inline constexpr DiagnosticDescriptor<2> ERROR_UNARY_OPERATOR_AMBIGUOUS {
    UserDiagnosticKind::Error, 509, "unary operator {} is ambiguous for '{}'"
};
inline constexpr DiagnosticDescriptor<1> ERROR_CALL_NON_FUNCTION {
    UserDiagnosticKind::Error, 510, "cannot call non-function '{}'"
};
inline constexpr DiagnosticDescriptor<1> ERROR_FUNCTION_OVERLOAD_NOT_FOUND {
    UserDiagnosticKind::Error, 511, "function overload not found for '{}'"
};
inline constexpr DiagnosticDescriptor<1> ERROR_FUNCTION_OVERLOAD_AMBIGUOUS {
    UserDiagnosticKind::Error, 512, "ambiguous function overload for '{}'"
};
inline constexpr DiagnosticDescriptor<2> ERROR_NO_IMPLICIT_CONVERSION_AVAILABLE {
    UserDiagnosticKind::Error, 513, "no implicit conversion available from '{}' to '{}'"
};
inline constexpr DiagnosticDescriptor<2> ERROR_NO_CONVERSION_AVAILABLE {
    UserDiagnosticKind::Error, 514, "no conversion available from '{}' to '{}'"
};
inline constexpr DiagnosticDescriptor<1> ERROR_VAR_DECL_NO_TYPE_OR_INIT {
    UserDiagnosticKind::Error, 515, "cannot deduce type for variable '{}' because it has no type or initializer"
};
inline constexpr DiagnosticDescriptor<2> ERROR_VAR_ASSIGNMENT_TYPE_MISMATCH {
    UserDiagnosticKind::Error, 516, "value of type '{}' cannot be assigned to variable of type '{}'"
};

// ------------------------------------------------------
//                       WARNINGS
// ------------------------------------------------------

// -------------------- SEMA WARNINGS -------------------

inline constexpr DiagnosticDescriptor<2> WARNING_NARROWING_CONVERSION {
    UserDiagnosticKind::Warning, 1000, "narrowing conversion from '{}' to '{}' may result in a loss of data"
};

// ------------------------------------------------------
//                        INFOS
// ------------------------------------------------------


// ------------------------------------------------------
//                        NOTES
// ------------------------------------------------------

inline constexpr DiagnosticNoteDescriptor<2> NOTE_PREVIOUS_DECLARATION_HERE {
    10000, "previous declaration is here"
};

}
VEEC_NAMESPACE_END
