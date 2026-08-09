#include "veec/lexing/Lexer.hpp"

#include <string>
#include <vector>
#include <string_view>
#include <cctype>
#include <algorithm>
#include <map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/Token.hpp"
#include "veec/basic/TokenList.hpp"
#include "veec/source/SourceView.hpp"
#include "veec/source/SourceLocation.hpp"
#include "veec/source/SourceRange.hpp"

VEEC_NAMESPACE_BEGIN
namespace lexing {

basic::TokenList Lexer::tokenize() {
    _state.currentIndex = 0;
    _state.tokens.clear();

    while (!isAtEnd()) {
        beginToken();
        scanToken();
    }

    beginToken();
    emitEOFToken();

    return basic::TokenList{
        _source.getFile(),
        std::move(_state.tokens)
    };
}

char Lexer::advance() {
    return _source.str()[_state.currentIndex++];
}
char Lexer::peek(size_t offset) const {
    size_t pos = _state.currentIndex + offset;
    return pos < _source.str().size() ? _source.str()[pos] : '\0';
}
bool Lexer::match(char expected) {
    if (isAtEnd() || _source.str()[_state.currentIndex] != expected) {
        return false;
    }
    advance();
    return true;
}
bool Lexer::match(std::string_view expected) {
    u32 len = static_cast<u32>(expected.length());
    if (_state.currentIndex + len > _source.getLength()) {
        return false;
    }
    for (u32 i = 0; i < len; ++i) {
        if (_source.str()[_state.currentIndex + i] != expected[i]) {
            return false;
        }
    }
    _state.currentIndex += len;
    return true;
}
std::string_view Lexer::slice(size_t len) const {
    size_t start = _state.currentIndex;
    size_t end = std::min(start + len, _source.getSize());

    VEE_ASSERT(len <= _source.getSize(), "Slice length exceeds source size");
    VEE_ASSERT(_state.currentIndex + len <= _source.getSize(), "Slice out of source bounds");

    return std::string_view(&_source.str()[start], end - start);
}
std::string_view Lexer::slice(size_t beg, size_t end) const { 
	VEE_ASSERT(beg <= end, "Invalid slice range: beg must be less than or equal to end");
	VEE_ASSERT(end <= _source.getSize(), "Invalid slice range: end must be less than or equal to source size");
	VEE_ASSERT(beg <= _source.getSize(), "Invalid slice range: beg must be less than or equal to source size");
  
	return std::string_view(&_source.str()[beg], end - beg);
}
bool Lexer::isAtEnd() const {
    return _state.currentIndex >= _source.getLength();
}

void Lexer::beginToken() {
    _state.startLocation = _state.currentIndex;
}
source::SourceRange Lexer::currentTokenRange() const {
    return source::SourceRange(
        _source.getFile(),
        _state.startLocation,
        _state.currentIndex
    );
}
void Lexer::emitToken(TokenType type) {
    VEE_ASSERT(type != TokenType::EndOfFile, "Use emitEOFToken for EndOfFile tokens");
    _state.tokens.emplace_back(Token{ type, currentTokenRange() });
}
void Lexer::emitEOFToken() {
    // Special case: EOF token will throw out-of-range if we try to slice
    auto range = source::SourceRange(
        _source.getFile(),
        _state.startLocation,
        _state.currentIndex
    );
	_state.tokens.emplace_back(Token{ TokenType::EndOfFile, range });
}

void Lexer::scanToken() {
    char c = peek();

    switch (c) {
        // Whitespace
        case ' ':
        case '\t':
            scanWhitespace();
            break;

        // Newline
        case '\n':
        case '\r':
            scanNewline();
            break;

        // ------------------------
        // Grammar
        // ------------------------

        case ';':
            advance();
            emitToken(TokenType::SemiColon);
            break;

        case ':':
            advance();

            if (match(':'))
                emitToken(TokenType::ColonColon);
            else
                emitToken(TokenType::Colon);

            break;

        case ',':
            advance();
            emitToken(TokenType::Comma);
            break;

        case '.':
            advance();
            emitToken(TokenType::Dot);
            break;

        case '@':
            advance();
            emitToken(TokenType::At);
            break;

        case '#':
            advance();
            emitToken(TokenType::Hash);
            break;

        case '(':
            advance();
            emitToken(TokenType::LParen);
            break;

        case ')':
            advance();
            emitToken(TokenType::RParen);
            break;

        case '{':
            advance();
            emitToken(TokenType::LCurly);
            break;

        case '}':
            advance();
            emitToken(TokenType::RCurly);
            break;

        case '[':
            advance();
            emitToken(TokenType::LBrack);
            break;

        case ']':
            advance();
            emitToken(TokenType::RBrack);
            break;

        case '$':
            advance();
            emitToken(TokenType::Dollar);
            break;

        case '`':
            advance();
            emitToken(TokenType::Backtick);
            break;

        // ------------------------
        // Operators
        // ------------------------

        case '?':
            advance();
            emitToken(TokenType::Question);
            break;

        case '~':
            advance();
            emitToken(TokenType::Tilde);
            break;

        case '&':
            advance();
            emitToken(TokenType::Ampersand);
            break;

        case '|':
            advance();
            emitToken(TokenType::Pipe);
            break;

        case '^':
            advance();
            emitToken(TokenType::Caret);
            break;

        case '+':
            advance();
            emitToken(
                match('+') ? TokenType::PlusPlus :
                match('=') ? TokenType::PlusEqual :
                            TokenType::Plus);
            break;

        case '-':
            advance();
            emitToken(
                match('-') ? TokenType::MinusMinus :
                match('=') ? TokenType::MinusEqual :
                match('>') ? TokenType::RArrow :
                            TokenType::Minus);
            break;

        case '*':
            advance();
            emitToken(
                match('*') ? TokenType::StarStar :
                match('=') ? TokenType::StarEqual :
                            TokenType::Star);
            break;

        case '/':
            if (peek(1) == '/' || peek(1) == '*') {
                scanComment();
            }
            else {
                advance();
                emitToken(match('=') ? TokenType::SlashEqual : TokenType::Slash);
            }
            break;

        case '%':
            advance();
            emitToken(TokenType::Percent);
            break;

        case '=':
            advance();
            emitToken(match('=') ? TokenType::EqualEqual : TokenType::Equal);
            break;

        case '!':
            advance();
            emitToken(match('=') ? TokenType::BangEqual : TokenType::Bang);
            break;

        case '<':
            advance();
            emitToken(
                match('=') ? TokenType::LessEqual :
                match('-') ? TokenType::LArrow :
                            TokenType::Less);
            break;

        case '>':
            advance();
            emitToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
            break;

        default:
            if (isDigit(c)) {
                scanNumber();
            } else if (isIdentifierStart(c)) {
                scanIdentifier();
            } else {
                reportUnexpectedCharacter(c);
                advance();
            }
            break;
    }
}
void Lexer::scanWhitespace() {
    while (isWhitespace(peek())) {
        advance();
    }

    emitToken(TokenType::Whitespace);
}
void Lexer::scanNewline() {
    while (isNewline(peek())) {
        advance();
    }

    emitToken(TokenType::Newline);
}
void Lexer::scanComment() {
    if (peek() == '/' && peek(1) == '/') {
        while (!isAtEnd() && !isNewline(peek())) {
            advance();
        }

        emitToken(TokenType::Comment);
    } else if (peek() == '/' && peek(1) == '*') {
        while (!isAtEnd() && !(peek() == '*' && peek(1) == '/')) {
            advance();
        }

        emitToken(TokenType::BlockComment);
    }
}
void Lexer::scanNumber() {
    while (isDigit(peek())) {
        advance();
    }

    if (peek() == '.' && isDigit(peek(1))) {
        advance(); // consume the '.'
        while (isDigit(peek())) {
            advance();
        }
        emitToken(TokenType::FloatLiteral);
    } else {
        emitToken(TokenType::IntegerLiteral);
    }
}
void Lexer::scanIdentifier() {
    while (isIdentifierContinue(peek())) {
        advance();
    }
    
    std::string_view text = slice(_state.startLocation, _state.currentIndex);
    emitToken(wordKind(text));
}

void Lexer::reportUnexpectedCharacter(char c) {
    // TODO: maybe currentTokenRange is not the best to use here?
    _ctx.diagnostics.report(diagnostics::ERROR_UNEXPECTED_CHARACTER, currentTokenRange(), c);
}

bool Lexer::isIdentifierStart(char c) {
    return isAsciiLetter(c) || c == '_';
}
bool Lexer::isIdentifierContinue(char c) {
    return isAsciiLetter(c) || isDigit(c) || c == '_';
}
bool Lexer::isWhitespace(char c) {
    return c == ' ' || c == '\t' || isNewline(c);
}
bool Lexer::isNewline(char c) {
    return c == '\n' || c == '\r';
}
bool Lexer::isAsciiLetter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

TokenType Lexer::wordKind(std::string_view value) {
    // Keywords
    if (value == "module") return TokenType::Module;
    else if (value == "func") return TokenType::Func;
	else if (value == "let") return TokenType::Let;
	else if (value == "var") return TokenType::Var;
    else if (value == "if") return TokenType::If;
    else if (value == "else") return TokenType::Else;
    else if (value == "loop") return TokenType::Loop;
    else if (value == "while") return TokenType::While;
    else if (value == "for") return TokenType::For;
	else if (value == "in") return TokenType::In;
	else if (value == "return") return TokenType::Return;
	else if (value == "true") return TokenType::True;
	else if (value == "false") return TokenType::False;
	else if (value == "class") return TokenType::Class;
    else if (value == "use") return TokenType::Use;
    else if (value == "as") return TokenType::As;
    else if (value == "import") return TokenType::Import;
    else if (value == "public") return TokenType::Public;
    else if (value == "private") return TokenType::Private;

    // Type specifiers
    else if (value == "void") return TokenType::Void;
	else if (value == "bool") return TokenType::Bool;
    else if (value == "i8") return TokenType::I8;
    else if (value == "u8") return TokenType::U8;
    else if (value == "i16") return TokenType::I16;
    else if (value == "u16") return TokenType::U16;
    else if (value == "i32") return TokenType::I32;
    else if (value == "u32") return TokenType::U32;
    else if (value == "i64") return TokenType::I64;
    else if (value == "u64") return TokenType::U64;
    else if (value == "f32") return TokenType::F32;
    else if (value == "f64") return TokenType::F64;

    return TokenType::Identifier;
}

bool Lexer::tryParseNumber(const std::string& text, u64& outValue) {
    VEE_ASSERT(!text.empty(), "Text must not be empty");

    if (text.size() > 2 && text[0] == '0') {
        if (text[1] == 'x' || text[1] == 'X') {
            return tryParseHexadecimal(text.substr(2), outValue);
        } else if (text[1] == 'b' || text[1] == 'B') {
            return tryParseBinary(text.substr(2), outValue);
        }
    }
    return tryParseDecimal(text, outValue);
}
bool Lexer::tryParseDecimal(const std::string& text, u64& outValue) {
    u64 value = 0;
	for (char c : text) {
        if (!isDigit(c)) {
            return false;
        }
		value = value * 10 + static_cast<u64>(c - '0');
    }
	outValue = value;
	return true;
}
bool Lexer::tryParseHexadecimal(const std::string& text, u64& outValue) {
    u64 value = 0;
    for (char c : text) {
        if (isDigit(c)) {
            value = value * 16 + static_cast<u64>(c - '0');
        } else if (c >= 'a' && c <= 'f') {
            value = value * 16 + static_cast<u64>(c - 'a' + 10);
        } else if (c >= 'A' && c <= 'F') {
            value = value * 16 + static_cast<u64>(c - 'A' + 10);
        } else {
            return false;
        }
	}
	outValue = value;
	return true;
}
bool Lexer::tryParseBinary(const std::string& text, u64& outValue) {
    u64 value = 0;
    for (char c : text) {
        if (c == '0') {
            value = value * 2;
        } else if (c == '1') {
            value = value * 2 + 1;
        } else {
            return false;
        }
    }
    outValue = value;
	return true;
}

} // namespace lexing
VEEC_NAMESPACE_END
