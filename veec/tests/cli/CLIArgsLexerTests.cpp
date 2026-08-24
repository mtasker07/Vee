/**
 * @file CLIArgsLexerTests.cpp
 * @brief This file contains unit tests for the cli::lexing::CLIArgsLexer class.
 * 
 * Notes:
 * - argv[0] is ALWAYS ignored. It must be included in the argv passed to tests otherwise
 * the first argument will be ignored and the test will fail.
 * 
 */

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

#include "veec/cli/lexing/CLIArgsLexer.hpp"
#include "veec/cli/CLIContext.hpp"
#include "veec/cli/CLIRawArgs.hpp"
#include "veec/cli/CLIArgsToken.hpp"

using veec::cli::lexing::CLIArgsLexer;
using veec::cli::CLIContext;
using veec::cli::CLIRawArgs;
using veec::cli::CLIArgsToken;
using veec::cli::CLIArgsTokenType;

/**
 * TEST: EmptyInputProducesNoTokens
 * 
 * Tests:
 * - Tokenizing null arguments with a 0 count.
 * 
 * Expected Result:
 * - Exactly no tokens are produced.
 * 
 * Notes:
 */
TEST(CLIArgsLexerTests, EmptyInputProducesNoTokens) {
	CLIContext ctx;

    CLIRawArgs args = CLIRawArgs(0, nullptr);

    CLIArgsLexer lexer(ctx, args);

    EXPECT_EQ(lexer.tokenize().size(), 0u);
}

/**
 * TEST: ProgramPathOnlyProducesNoTokens
 * 
 * Tests:
 * - Tokenizing args that contain the program path only.
 * 
 * Expected Result:
 * - Exactly no tokens are produced.
 * 
 * Notes:
 */
TEST(CLIArgsLexerTests, ProgramPathOnlyProducesNoTokens) {
	CLIContext ctx;

    const char* argv[] = {"program/path"};
    CLIRawArgs args = CLIRawArgs(1, argv);

    CLIArgsLexer lexer(ctx, args);

    EXPECT_EQ(lexer.tokenize().size(), 0u);
}

/**
 * TEST: LexerProducesCorrectTokensForSimpleArgs
 * 
 * Tests:
 * - Tokenizing basic command-line arguments with a program path, a long option,
 * a short option, and a positional argument.
 * - Option tokens store their name in the token's optionName field.
 * 
 * Expected Result:
 * - The correct number of tokens are produced with the right token types.
 * - Option tokens have their optionName field set correctly.
 * 
 * Notes:
 */
TEST(CLIArgsLexerTests, LexerProducesCorrectTokensForSimpleArgs) {
	CLIContext ctx;

    const char* argv[] = {"program/path", "--long-option", "-s", "positional"};
    CLIRawArgs args = CLIRawArgs(4, argv);

    CLIArgsLexer lexer(ctx, args);

    std::vector<CLIArgsToken> tokens = lexer.tokenize();

    EXPECT_EQ(tokens.size(), 3u);

    // Check types
    EXPECT_EQ(tokens[0].type, CLIArgsTokenType::LongOption);
    EXPECT_EQ(tokens[1].type, CLIArgsTokenType::ShortOption);
    EXPECT_EQ(tokens[2].type, CLIArgsTokenType::Positional);

    // Check option names
    EXPECT_EQ(tokens[0].optionName, "long-option");
    EXPECT_EQ(tokens[1].optionName, "s");
    EXPECT_TRUE(tokens[2].optionName.empty());
}

/**
 * TEST: LexerParsesShortSequences
 * 
 * Tests:
 * - Tokenizing short sequence command-line arguments.
 * 
 * Expected Result:
 * - The correct number of tokens are produced with the right token types.
 * 
 * Notes:
 */
TEST(CLIArgsLexerTests, LexerParsesShortSequences) {
	CLIContext ctx;

    const char* argv[] = {"program/path", "-abc", "-ABC", "-O2"};
    CLIRawArgs args = CLIRawArgs(4, argv);

    CLIArgsLexer lexer(ctx, args);

    std::vector<CLIArgsToken> tokens = lexer.tokenize();

    EXPECT_EQ(tokens.size(), 3u);

    // Check types
    EXPECT_EQ(tokens[0].type, CLIArgsTokenType::ShortSequence);
    EXPECT_EQ(tokens[1].type, CLIArgsTokenType::ShortSequence);
    EXPECT_EQ(tokens[2].type, CLIArgsTokenType::ShortSequence);

    // Check option names
    EXPECT_EQ(tokens[0].optionName, "abc");
    EXPECT_EQ(tokens[1].optionName, "ABC");
    EXPECT_EQ(tokens[2].optionName, "O2");
}
