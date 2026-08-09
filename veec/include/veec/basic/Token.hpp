/**
 * @file Token.hpp
 * @brief This file contains the definitions for tokens used by the Vee compiler.
 * 
 * The vee compiler uses a zero-copy lexer meaning that tokens do not store
 * their own lexeme text but instead store the regions of text that they
 * correspond to.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/source/SourceRange.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @enum TokenType
 * @brief Represents the type of a token.
 */
enum class TokenType {
    Unknown,
    Error,
    Identifier,

    // -- Trivia --
    Whitespace,
    Newline,
    Comment,
    BlockComment,

    // -- Literals --
    FloatLiteral,
    IntegerLiteral,
    StringLiteral,
    CharLiteral,

    // -- Grammar --
    SemiColon,  // ;
    Colon,      // :
    Comma,      // ,
    Dot,        // .
    At,         // @
    Hash,       // #
    LParen,     // (
    RParen,     // )
    LCurly,     // {
    RCurly,     // }
    LBrack,     // [
    RBrack,     // ]
    Dollar,     // $
    Backtick,   // `

    // -- Operators --
    Question,       // ?
    Tilde,          // ~
    Ampersand,      // &
    Pipe,           // |
    Caret,          // ^
    Plus,           // +
    Minus,          // -
    Star,           // *
    Slash,          // /
    Percent,        // %
    Equal,          // =
    Bang,           // !
    Less,           // <
    Greater,        // >
    LessEqual,      // <=
    GreaterEqual,   // >=
    EqualEqual,     // ==
    BangEqual,      // !=
    PlusEqual,      // +=
    MinusEqual,     // -=
    StarEqual,      // *=
    SlashEqual,     // /=
    PlusPlus,       // ++
    MinusMinus,     // --
    StarStar,       // **
    ColonColon,     // ::
    LArrow,         // <-
    RArrow,         // ->
    
    // -- Keywords --
    Module,
    Func,
    Let,
    Var,
    If,
    Else,
    Loop,
    While,
    For,
    In,
    Return,
    True,
    False,
    Class,
    Use,
    As,
    Import,
    Public,
    Private,

    // -- Type Specifiers --
    Void,   // void
    Bool,   // bool
    I8,     // i8
    U8,     // u8
    I16,    // i16
    U16,    // u16
    I32,    // i32
    U32,    // u32
    I64,    // i64
    U64,    // u64
    F32,    // f32
    F64,    // f64

    EndOfFile,
};

/**
 * @brief Converts a token type to a human-readable string.
 * @param type The token type to convert.
 * @return A string representation of the token type.
 */
inline std::string_view tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::Unknown: return "Unknown";
        case TokenType::Error: return "Error";
        case TokenType::Identifier: return "Identifier";
        case TokenType::Whitespace: return "Whitespace";
        case TokenType::Newline: return "Newline";
        case TokenType::Comment: return "Comment";
        case TokenType::BlockComment: return "BlockComment";
        case TokenType::FloatLiteral: return "FloatLiteral";
        case TokenType::IntegerLiteral: return "IntegerLiteral";
        case TokenType::StringLiteral: return "StringLiteral";
        case TokenType::CharLiteral: return "CharLiteral";
        case TokenType::SemiColon: return "SemiColon";
        case TokenType::Colon: return "Colon";
        case TokenType::Comma: return "Comma";
        case TokenType::Dot: return "Dot";
        case TokenType::At: return "At";
        case TokenType::Hash: return "Hash";
        case TokenType::LParen: return "LParen";
        case TokenType::RParen: return "RParen";
        case TokenType::LCurly: return "LCurly";
        case TokenType::RCurly: return "RCurly";
        case TokenType::LBrack: return "LBrack";
        case TokenType::RBrack: return "RBrack";
        case TokenType::Dollar: return "Dollar";
        case TokenType::Backtick: return "Backtick";
        case TokenType::Question: return "Question";
        case TokenType::Tilde: return "Tilde";
        case TokenType::Ampersand: return "Ampersand";
        case TokenType::Pipe: return "Pipe";
        case TokenType::Caret: return "Caret";
        case TokenType::Plus: return "Plus";
        case TokenType::Minus: return "Minus";
        case TokenType::Star: return "Star";
        case TokenType::Slash: return "Slash";
        case TokenType::Percent: return "Percent";
        case TokenType::Equal: return "Equal";
        case TokenType::Bang: return "Bang";
        case TokenType::Less: return "Less";
        case TokenType::Greater: return "Greater";
        case TokenType::LessEqual: return "LessEqual";
        case TokenType::GreaterEqual: return "GreaterEqual";
        case TokenType::EqualEqual: return "EqualEqual";
        case TokenType::BangEqual: return "BangEqual";
        case TokenType::PlusEqual: return "PlusEqual";
        case TokenType::MinusEqual: return "MinusEqual";
        case TokenType::StarEqual: return "StarEqual";
        case TokenType::SlashEqual: return "SlashEqual";
        case TokenType::PlusPlus: return "PlusPlus";
        case TokenType::MinusMinus: return "MinusMinus";
        case TokenType::StarStar: return "StarStar";
        case TokenType::ColonColon: return "ColonColon";
        case TokenType::LArrow: return "LArrow";
        case TokenType::RArrow: return "RArrow";
        case TokenType::Module: return "Module";
        case TokenType::Func: return "Func";
        case TokenType::Let: return "Let";
        case TokenType::Var: return "Var";
        case TokenType::If: return "If";
        case TokenType::Else: return "Else";
        case TokenType::Loop: return "Loop";
        case TokenType::While: return "While";
        case TokenType::For: return "For";
        case TokenType::In: return "In";
        case TokenType::Return: return "Return";
        case TokenType::True: return "True";
        case TokenType::False: return "False";
        case TokenType::Class: return "Class";
        case TokenType::Use: return "Use";
        case TokenType::As: return "As";
        case TokenType::Import: return "Import";
        case TokenType::Public: return "Public";
        case TokenType::Private: return "Private";
        case TokenType::Void: return "Void";
        case TokenType::Bool: return "Bool";
        case TokenType::I8: return "I8";
        case TokenType::U8: return "U8";
        case TokenType::I16: return "I16";
        case TokenType::U16: return "U16";
        case TokenType::I32: return "I32";
        case TokenType::U32: return "U32";
        case TokenType::I64: return "I64";
        case TokenType::U64: return "U64";
        case TokenType::F32: return "F32";
        case TokenType::F64: return "F64";
        case TokenType::EndOfFile: return "EndOfFile";
        default:
            VEE_UNREACHABLE("Invalid token type");
    }
}

/**
 * @class Token
 * @brief Represents a single token in the source code.
 */
class Token {
public:
    /**
     * @brief Constructs a new Token instance with the given type and source range.
     * A token must always have a type and a range of source code that it corresponds to.
     * @param type The type of the token.
     * @param range The source range of the token in the source code.
     */
    Token(TokenType type, source::SourceRange range)
        : _type(type), _range(range) {}

    // ACCESSORS

    /**
     * @brief Gets the type of this token.
     * @return The type of this token.
     */
    TokenType type() const { return _type; }
    /**
     * @brief Gets the source range of this token.
     * @return The source range of this token.
     */
    source::SourceRange range() const { return _range; }
    /**
     * @brief Gets the text of this token.
     * @return A string_view of the text corresponding to this token.
     */
    std::string_view text() const {
        return _range.getText();
    }

    // BASIC UTILITIES

    /**
     * @brief Checks if this token is a given type.
     * @param type The token type to check against.
     * @return True if the token is of the given type, false otherwise.
     */
    inline bool is(TokenType type) const {
        return _type == type;
    }
    /**
     * @brief Checks if this token is any of the given types.
     * @tparam Types A variadic list of token types to check against.
     * @param types The token types to check against.
     * @return True if the token is any of the given types, false otherwise.
     */
    template<typename... Types>
    inline bool isAnyOf(Types... types) const {
        return ((is(types)) || ...);
    }

    // BROAD TYPE CHECKS

    /**
     * @brief Checks if the token is an identifier token.
     * @return True if the token is an identfier token, false otherwise.
     */
    inline bool isIdentifier() const {
        return _type == TokenType::Identifier;
    }
    /**
     * @brief Checks if the token is a trivia token (whitespace, newline, comment).
     * @return True if the token is a trivia token, false otherwise.
     */
    inline bool isTrivia() const {
        switch (_type) {
            case TokenType::Whitespace:
            case TokenType::Newline:
            case TokenType::Comment:
            case TokenType::BlockComment:
                break;
            default:
                return false;
        }
        return true;
    }
    /**
     * @brief Checks if the token is a literal token.
     * @return True if the token is a literal token, false otherwise.
     */
    inline bool isLiteral() const {
        switch (_type) {
            case TokenType::FloatLiteral:
            case TokenType::IntegerLiteral:
            case TokenType::StringLiteral:
            case TokenType::CharLiteral:
                break;
            default:
                return false;
        }
        return true;
    }
    /**
     * @brief Checks if the token is a grammar token.
     * @return True if the token is a grammar token, false otherwise.
     */
    inline bool isGrammar() const {
        switch (_type) {
            case TokenType::SemiColon:
            case TokenType::Colon:
            case TokenType::Comma:
            case TokenType::Dot:
            case TokenType::At:
            case TokenType::Hash:
            case TokenType::LParen:
            case TokenType::RParen:
            case TokenType::LCurly:
            case TokenType::RCurly:
            case TokenType::LBrack:
            case TokenType::RBrack:
            case TokenType::Dollar:
            case TokenType::Backtick:
                break;
            default:
                return false;
        }
        return true;
    }
    /**
     * @brief Checks if the token is an operator token.
     * @return True if the token is an operator token, false otherwise.
     */
    inline bool isOperator() const {
        switch (_type) {
            case TokenType::Question:
            case TokenType::Tilde:
            case TokenType::Ampersand:
            case TokenType::Pipe:
            case TokenType::Caret:
            case TokenType::Plus:
            case TokenType::Minus:
            case TokenType::Star:
            case TokenType::Slash:
            case TokenType::Percent:
            case TokenType::Equal:
            case TokenType::Bang:
            case TokenType::Less:
            case TokenType::Greater:
            case TokenType::LessEqual:
            case TokenType::GreaterEqual:
            case TokenType::EqualEqual:
            case TokenType::BangEqual:
            case TokenType::PlusEqual:
            case TokenType::MinusEqual:
            case TokenType::StarEqual:
            case TokenType::SlashEqual:
            case TokenType::PlusPlus:
            case TokenType::MinusMinus:
            case TokenType::StarStar:
            case TokenType::ColonColon:
            case TokenType::LArrow:
            case TokenType::RArrow:
                break;
            default:
                return false;
        }
        return true;
    }
    /**
     * @brief Checks if the token is a keyword token.
     * @return True if the token is a keyword token, false otherwise.
     * @note Type specifier keywords are not considered in this check,
     * use isTypeSpecifier() to check for type specifier keywords.
     */
    inline bool isKeyword() const {
        switch (_type) {
            case TokenType::Module:
            case TokenType::Func:
            case TokenType::If:
            case TokenType::Else:
            case TokenType::Loop:
            case TokenType::While:
            case TokenType::For:
            case TokenType::In:
            case TokenType::Let:
            case TokenType::Var:
            case TokenType::Return:
            case TokenType::True:
            case TokenType::False:
            case TokenType::Class:
            case TokenType::Use:
            case TokenType::As:
            case TokenType::Import:
            case TokenType::Public:
            case TokenType::Private:
                break;
            default:
                return false;
        }
        return true;
    }
    /**
     * @brief Checks if the token is a type specifier token.
     * @return True if the token is a type specifier token, false otherwise.
     */
    inline bool isTypeSpecifier() const {
        switch (_type) {
            case TokenType::Void:
            case TokenType::Bool:
            case TokenType::I8:
            case TokenType::U8:
            case TokenType::I16:
            case TokenType::U16:
            case TokenType::I32:
            case TokenType::U32:
            case TokenType::I64:
            case TokenType::U64:
            case TokenType::F32:
            case TokenType::F64:
                break;
            default:
                return false;
        }
        return true;
    }
    /**
     * @brief Checks if the token is an end of file token.
     * @return True if the token is an end of file token, false otherwise.
     */
    inline bool isEndOfFile() const {
        return _type == TokenType::EndOfFile;
    }

private:
    TokenType _type = TokenType::Unknown;
    source::SourceRange _range;
};

} // namespace basic
VEEC_NAMESPACE_END
