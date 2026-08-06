/**
 * @file LexerTests.cpp
 * @brief This file contains unit tests for the lexing::Lexer class.
 */

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

#include "veec/lexing/Lexer.hpp"
#include "veec/compilation/CompilationContext.hpp"

using veec::lexing::Lexer;
using veec::lexing::Token;
using veec::lexing::TokenType;
using veec::compilation::CompilationContext;

namespace {

std::vector<Token> lex(CompilationContext& ctx, std::string_view sourceText) {
	auto fileId = ctx.sources.addVirtualFile("LexerTests.vee", sourceText);
	Lexer lexer(ctx, ctx.sources.getView(fileId));
	return lexer.tokenize().tokens;
}

void expectTypes(const std::vector<Token>& tokens, const std::vector<TokenType>& expected) {
	ASSERT_EQ(tokens.size(), expected.size());

	for (size_t i = 0; i < expected.size(); ++i) {
		SCOPED_TRACE(i);
		EXPECT_EQ(tokens[i].type(), expected[i]);
	}
}

std::string_view lexeme(const CompilationContext& ctx, const Token& token) {
	return ctx.sources.getText(token.range());
}

} // namespace

/**
 * TEST: EmptyInputProducesOnlyEndOfFile
 * 
 * Tests:
 * - Tokenizing an empty source string.
 * 
 * Expected Result:
 * - Exactly one EndOfFile token is produced.
 * 
 * Notes:
 */
TEST(LexerTests, EmptyInputProducesOnlyEndOfFile) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "");

	ASSERT_EQ(tokens.size(), 1u);
	EXPECT_EQ(tokens[0].type(), TokenType::EndOfFile);
	EXPECT_EQ(tokens[0].range().startOffset, 0u);
	EXPECT_EQ(tokens[0].range().endOffset, 0u);
}

/**
 * TEST: LexesGrammarPunctuation
 * 
 * Tests:
 * - Lexing grammar punctuation symbols.
 * 
 * Expected Result:
 * - Each symbol maps to its corresponding grammar token type.
 * 
 * Notes:
 */
TEST(LexerTests, LexesGrammarPunctuation) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, ";:,.@#(){}[]$`");

	expectTypes(tokens, {
		TokenType::SemiColon,
		TokenType::Colon,
		TokenType::Comma,
		TokenType::Dot,
		TokenType::At,
		TokenType::Hash,
		TokenType::LParen,
		TokenType::RParen,
		TokenType::LCurly,
		TokenType::RCurly,
		TokenType::LBrack,
		TokenType::RBrack,
		TokenType::Dollar,
		TokenType::Backtick,
		TokenType::EndOfFile
	});
}

/**
 * TEST: LexesOperatorsIncludingCompoundForms
 * 
 * Tests:
 * - Lexing operators with single-character and multi-character forms.
 * 
 * Expected Result:
 * - Compound operators are emitted as dedicated token types.
 * 
 * Notes:
 */
TEST(LexerTests, LexesOperatorsIncludingCompoundForms) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "? ~ & | ^ + ++ += - -- -= -> * ** *= / /= % = == ! != < <= <- > >= ::");

	expectTypes(tokens, {
		TokenType::Question,
		TokenType::Whitespace,
		TokenType::Tilde,
		TokenType::Whitespace,
		TokenType::Ampersand,
		TokenType::Whitespace,
		TokenType::Pipe,
		TokenType::Whitespace,
		TokenType::Caret,
		TokenType::Whitespace,
		TokenType::Plus,
		TokenType::Whitespace,
		TokenType::PlusPlus,
		TokenType::Whitespace,
		TokenType::PlusEqual,
		TokenType::Whitespace,
		TokenType::Minus,
		TokenType::Whitespace,
		TokenType::MinusMinus,
		TokenType::Whitespace,
		TokenType::MinusEqual,
		TokenType::Whitespace,
		TokenType::RArrow,
		TokenType::Whitespace,
		TokenType::Star,
		TokenType::Whitespace,
		TokenType::StarStar,
		TokenType::Whitespace,
		TokenType::StarEqual,
		TokenType::Whitespace,
		TokenType::Slash,
		TokenType::Whitespace,
		TokenType::SlashEqual,
		TokenType::Whitespace,
		TokenType::Percent,
		TokenType::Whitespace,
		TokenType::Equal,
		TokenType::Whitespace,
		TokenType::EqualEqual,
		TokenType::Whitespace,
		TokenType::Bang,
		TokenType::Whitespace,
		TokenType::BangEqual,
		TokenType::Whitespace,
		TokenType::Less,
		TokenType::Whitespace,
		TokenType::LessEqual,
		TokenType::Whitespace,
		TokenType::LArrow,
		TokenType::Whitespace,
		TokenType::Greater,
		TokenType::Whitespace,
		TokenType::GreaterEqual,
		TokenType::Whitespace,
		TokenType::ColonColon,
		TokenType::EndOfFile
	});
}

/**
 * TEST: LexesKeywordsAndTypeSpecifiers
 * 
 * Tests:
 * - Lexing all currently recognized keyword and type-specifier words.
 * 
 * Expected Result:
 * - Words map to dedicated keyword/type token kinds instead of Identifier.
 * 
 * Notes:
 */
TEST(LexerTests, LexesKeywordsAndTypeSpecifiers) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "func if else while for loop void i8 u8 i16 u16 i32 u32 i64 u64 f32 f64");

	expectTypes(tokens, {
		TokenType::Func, TokenType::Whitespace,
		TokenType::If, TokenType::Whitespace,
		TokenType::Else, TokenType::Whitespace,
		TokenType::While, TokenType::Whitespace,
		TokenType::For, TokenType::Whitespace,
		TokenType::Loop, TokenType::Whitespace,
		TokenType::Void, TokenType::Whitespace,
		TokenType::I8, TokenType::Whitespace,
		TokenType::U8, TokenType::Whitespace,
		TokenType::I16, TokenType::Whitespace,
		TokenType::U16, TokenType::Whitespace,
		TokenType::I32, TokenType::Whitespace,
		TokenType::U32, TokenType::Whitespace,
		TokenType::I64, TokenType::Whitespace,
		TokenType::U64, TokenType::Whitespace,
		TokenType::F32, TokenType::Whitespace,
		TokenType::F64,
		TokenType::EndOfFile
	});
}

/**
 * TEST: LexesIdentifiers
 * 
 * Tests:
 * - Identifier rules for leading underscore and alphanumeric continuation.
 * 
 * Expected Result:
 * - Valid identifier spellings are emitted as Identifier tokens.
 * 
 * Notes:
 */
TEST(LexerTests, LexesIdentifiers) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "name _value value2 _9");

	expectTypes(tokens, {
		TokenType::Identifier,
		TokenType::Whitespace,
		TokenType::Identifier,
		TokenType::Whitespace,
		TokenType::Identifier,
		TokenType::Whitespace,
		TokenType::Identifier,
		TokenType::EndOfFile
	});
}

/**
 * TEST: LexesIntegerAndFloatLiterals
 * 
 * Tests:
 * - Integer literal lexing.
 * - Float literal lexing when decimal point is followed by a digit.
 * 
 * Expected Result:
 * - Integer and float token kinds are produced correctly.
 * 
 * Notes:
 */
TEST(LexerTests, LexesIntegerAndFloatLiterals) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "0 42 12.5 99.");

	expectTypes(tokens, {
		TokenType::IntegerLiteral,
		TokenType::Whitespace,
		TokenType::IntegerLiteral,
		TokenType::Whitespace,
		TokenType::FloatLiteral,
		TokenType::Whitespace,
		TokenType::IntegerLiteral,
		TokenType::Dot,
		TokenType::EndOfFile
	});
}

/**
 * TEST: LexesWhitespaceAndNewlineTrivia
 * 
 * Tests:
 * - Coalescing of consecutive spaces/tabs.
 * - Coalescing of consecutive newlines.
 * 
 * Expected Result:
 * - Trivia is grouped into Whitespace or Newline tokens.
 * 
 * Notes:
 */
TEST(LexerTests, LexesWhitespaceAndNewlineTrivia) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, " \t\t\n\n");

	expectTypes(tokens, {
		TokenType::Whitespace,
		TokenType::EndOfFile
	});

	EXPECT_EQ(lexeme(ctx, tokens[0]), " \t\t\n\n");
}

/**
 * TEST: LexesSingleLineComment
 * 
 * Tests:
 * - Lexing a C++-style single-line comment.
 * 
 * Expected Result:
 * - Comment token is emitted up to but not including newline.
 * 
 * Notes:
 */
TEST(LexerTests, LexesSingleLineComment) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "// comment\nnext");

	expectTypes(tokens, {
		TokenType::Comment,
		TokenType::Newline,
		TokenType::Identifier,
		TokenType::EndOfFile
	});

	EXPECT_EQ(lexeme(ctx, tokens[0]), "// comment");
}

/**
 * TEST: LexesBlockCommentUsingCurrentImplementationBehavior
 * 
 * Tests:
 * - Lexing a block comment sequence.
 * 
 * Expected Result:
 * - BlockComment token is emitted.
 * - Closing delimiter is lexed as separate Star and Slash tokens.
 * 
 * Notes:
 * - This test documents current behavior of scanComment.
 */
TEST(LexerTests, LexesBlockCommentUsingCurrentImplementationBehavior) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "/*x*/");

	expectTypes(tokens, {
		TokenType::BlockComment,
		TokenType::Star,
		TokenType::Slash,
		TokenType::EndOfFile
	});

	EXPECT_EQ(lexeme(ctx, tokens[0]), "/*x");
	EXPECT_EQ(lexeme(ctx, tokens[1]), "*");
	EXPECT_EQ(lexeme(ctx, tokens[2]), "/");
}

/**
 * TEST: ReportsUnexpectedCharacterDiagnostic
 * 
 * Tests:
 * - Reporting diagnostic when lexer encounters unsupported character.
 * 
 * Expected Result:
 * - One error diagnostic with code 1 is emitted.
 * - Lexer still emits EndOfFile token.
 * 
 * Notes:
 */
TEST(LexerTests, ReportsUnexpectedCharacterDiagnostic) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "\"");

	const auto& diagnostics = ctx.diagnostics.getDiagnostics();
	ASSERT_EQ(diagnostics.size(), 1u);
	EXPECT_EQ(diagnostics[0].getCode(), 1u);

	ASSERT_EQ(tokens.size(), 1u);
	EXPECT_EQ(tokens[0].type(), TokenType::EndOfFile);
}

/**
 * TEST: TokenRangesMatchSourceSlices
 * 
 * Tests:
 * - Source ranges of produced tokens.
 * 
 * Expected Result:
 * - Token ranges map back to expected source lexemes.
 * 
 * Notes:
 */
TEST(LexerTests, TokenRangesMatchSourceSlices) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "func add");

	ASSERT_GE(tokens.size(), 3u);
	EXPECT_EQ(lexeme(ctx, tokens[0]), "func");
	EXPECT_EQ(lexeme(ctx, tokens[1]), " ");
	EXPECT_EQ(lexeme(ctx, tokens[2]), "add");

	EXPECT_EQ(tokens[0].range().startOffset, 0u);
	EXPECT_EQ(tokens[0].range().endOffset, 4u);
	EXPECT_EQ(tokens[2].range().startOffset, 5u);
	EXPECT_EQ(tokens[2].range().endOffset, 8u);
}

/**
 * TEST: DistinguishesSlashOperatorFromCommentStart
 * 
 * Tests:
 * - Slash operator and slash-equals operator.
 * - Single-line comment detection.
 * 
 * Expected Result:
 * - `/` and `/=` lex as operators.
 * - `//...` lexes as Comment token.
 * 
 * Notes:
 */
TEST(LexerTests, DistinguishesSlashOperatorFromCommentStart) {
	CompilationContext ctx;
	std::vector<Token> tokens = lex(ctx, "/ /= //x");

	expectTypes(tokens, {
		TokenType::Slash,
		TokenType::Whitespace,
		TokenType::SlashEqual,
		TokenType::Whitespace,
		TokenType::Comment,
		TokenType::EndOfFile
	});
}
