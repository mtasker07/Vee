/**
 * @file CLIArgsParserTests.cpp
 * @brief This file contains unit tests for the cli::parsing::CLIArgsParser class.
 * 
 * Notes:
 * - argv[0] is ALWAYS ignored. It must be included in the argv passed to tests otherwise
 * the first argument will be ignored and the test will fail.
 * 
 */

#include <gtest/gtest.h>

#include <string_view>
#include <vector>

#include "veec/cli/parsing/CLIArgsParser.hpp"
#include "veec/cli/lexing/CLIArgsLexer.hpp"
#include "veec/cli/CLIArgs.hpp"
#include "veec/cli/CLIArgsToken.hpp"
#include "veec/cli/parsing/CLIArgsParseResult.hpp"
#include "veec/compilation/CompilationContext.hpp"

using veec::cli::parsing::CLIArgsParser;
using veec::cli::lexing::CLIArgsLexer;
using veec::cli::CLIArgs;
using veec::cli::CLIArgsToken;
using veec::cli::CLIArgsTokenType;
using veec::cli::parsing::CLIArgsParseResult;
using veec::compilation::CompilationContext;

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
TEST(CLIArgsParserTests, EmptyInputProducesNoTokens) {
	CompilationContext ctx;

    CLIArgs args;
    args.parse(0, nullptr);

    CLIArgsLexer lexer(ctx, args);
    std::vector<CLIArgsToken> tokens = lexer.tokenize();
    CLIArgsParser parser(ctx, tokens);

    CLIArgsParseResult result = parser.parse();
    EXPECT_EQ(result.getPositionals().size(), 0u);
    EXPECT_EQ(result.getOptions().size(), 0u);
}
