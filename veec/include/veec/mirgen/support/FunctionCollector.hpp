/**
 * @file FunctionCollector.hpp
 * @brief This file contains the definition of the FunctionCollector class.
 * The FunctionCollector class is responsible for collecting function declarations from the AST.
 * 
 * The idea of this class is to only ever have a single visit function, for functions, since the
 * default implementations of all other visits should recursively walk the AST. Therefore, if any
 * functions are missing, it indicates a bug in the AST walker. It also makes logic a lot simpler.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"

VEEC_NAMESPACE_BEGIN
namespace mirgen {
namespace support {

/**
 * @class FunctionCollector
 * @brief A class that collects function declarations from the AST.
 */
class FunctionCollector : private ast::ConstAstWalker {
public:
    FunctionCollector() = default;
    ~FunctionCollector() = default;

    /**
     * @brief Collects all function declarations from the given AST node.
     * @param node The AST node to collect function declarations from.
     * @return A list of the collected function declaration nodes.
     */
    std::vector<const ast::FunctionDeclNode*> collectFunctions(const ast::AstNode& node);

private:
    std::vector<const ast::FunctionDeclNode*> _functions;

    void visitFunctionDecl(const ast::FunctionDeclNode& node) override;
};

} // namespace support
} // namespace mirgen
VEEC_NAMESPACE_END
