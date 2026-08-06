/**
 * @file Lexer.hpp
 * @brief This file contains the definition of the Lexer class,
 * which is responsible for separating source code into a list of tokens
 * that can be consumed by the parser.
 */

#pragma once

#include <string>
#include <vector>
#include <string_view>
#include <filesystem>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/Maybe.hpp"
#include "veec/basic/Token.hpp"
#include "veec/basic/TokenList.hpp"
#include "veec/source/SourceView.hpp"

VEEC_NAMESPACE_BEGIN
namespace lexing {

// lexing::Token -> basic::Token
using Token = basic::Token;
// lexing::TokenType -> basic::TokenType
using TokenType = basic::TokenType;

/**
 * @struct LexerState
 * @brief Holds the current internal state of the lexer.
 */
struct LexerState {
    u32 startLocation = 0;
    u32 currentIndex = 0;
    std::vector<Token> tokens;
};

/**
 * @class Lexer
 * @brief Responsible for separating source code into a list of tokens
 * that can be consumed by the parser.
 */
class Lexer {
public:
    /**
     * @brief Constructs a new Lexer instance with the given source view and compilation context.
     * @param source The source view to tokenize.
     * @param ctx The context for this compilation process.
     */
    Lexer(compilation::CompilationContext& ctx, source::SourceView source)
        : _ctx(ctx), _source(source) {}
        
    ~Lexer() = default;

    /**
     * @brief Tokenizes the given source code and returns a list of generated tokens.
     * @return A vector containing the generated tokens.
     */
    basic::TokenList tokenize();

private:
    compilation::CompilationContext& _ctx;
    source::SourceView _source;
    LexerState _state;

    char advance();
    char peek(size_t offset = 0) const;
    bool match(char expected);
    bool match(std::string_view expected);
    std::string_view slice(size_t len) const;
    std::string_view slice(size_t beg, size_t end) const;
    bool isAtEnd() const;

    void beginToken();
    source::SourceRange currentTokenRange() const;
    void emitToken(TokenType type);
    void emitEOFToken();

    void scanToken();
    void scanWhitespace();
    void scanNewline();
    void scanComment();
    void scanNumber();
    void scanIdentifier();

    void reportUnexpectedCharacter(char c);

    static bool isIdentifierStart(char c);
    static bool isIdentifierContinue(char c);
    static bool isWhitespace(char c);
    static bool isNewline(char c);
    static bool isAsciiLetter(char c);
    static bool isDigit(char c);

    static TokenType wordKind(std::string_view value);

    static bool tryParseNumber(const std::string& text, u64& outValue);
    static bool tryParseDecimal(const std::string& text, u64& outValue);
    static bool tryParseHexadecimal(const std::string& text, u64& outValue);
    static bool tryParseBinary(const std::string& text, u64& outValue);
};

} // namespace lexing
VEEC_NAMESPACE_END
