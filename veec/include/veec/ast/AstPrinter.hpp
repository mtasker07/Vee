/**
 * @file AstPrinter.hpp
 * @brief Contains the definition of the AstPrinter class, which is
 * responsible for converting AST nodes into human-readable string
 * representations for debugging and visualization purposes.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/io/IWriter.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/ast/AstFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @class AstPrinter
 * @brief Responsible for converting AST nodes into human-readable
 * strings for debugging and visualization purposes.
 */
class AstPrinter {
public:
    /**
     * @brief Creates a new AstPrinter instance with the given SourceManager.
     * @param sm The SourceManager to use for retrieving source text.
     * @param sp The StringPool to use for retrieving string text.
     */
    AstPrinter(const compilation::CompilationContext& ctx)
        : _sm(ctx.sources), _sp(ctx.strings) {}

    ~AstPrinter() = default;

    /**
     * @brief Converts the given AST node into a human-readable text representation.
     * @param node The AST node to convert.
     * @param writer The writer to which the text representation will be written.
     */
    void printNode(const AstNode& node, io::IWriter& writer) const;

private:
    const source::SourceManager& _sm;
    const basic::StringPool& _sp;
};

} // namespace ast
VEEC_NAMESPACE_END
