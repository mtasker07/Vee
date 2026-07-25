/**
 * @file UseStmtNode.hpp
 * @brief This file contains the definition of the UseStmtNode AST node.
 * The UseStmt node represents a use statement in the AST.
 * 
 * A use statement is used to import names from other modules into the current
 * scope. It consists of a qualified name for the module, and an optional list of
 * names to import from that module.
 * 
 * For example:
 * 
 * use std::io::{Read, Write};  // Imports only Read and Write from std::io
 * use std::io::Read;           // Imports only Read from std::io
 * use std::io;                 // Imports all names from std::io
 * 
 * Using statements are symbol aware, therefore use statements can be used to import
 * specific symbols or a module itself. It all depends on whether the use statement path
 * resolves to a module or a symbol. If a compact import is used '{...}', then the use
 * statement is implicitly importing symbols and not modules.
 * 
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/basic/Token.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/stmt/StatementNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

/**
 * @brief This node represents a use statement in the AST.
 */
class UseStmtNode : public StatementNode {
public:
    UseStmtNode(AstKey, std::vector<source::Identifier>&& modulePath, std::vector<source::Identifier>&& imports)
        : StatementNode(AstKey{}, AstKind::UseStmt), _modulePath(std::move(modulePath)), _imports(std::move(imports)) {}

    virtual ~UseStmtNode() = default;

    /**
     * @brief Gets the module path in this use statement (read-only).
     * @return The module path in this use statement.
     */
    inline const std::vector<source::Identifier>& getModulePath() const {
        return _modulePath;
    }
    /**
     * @brief Gets the module path in this use statement.
     * @return The module path in this use statement.
     */
	inline std::vector<source::Identifier>& getModulePath() {
		return _modulePath;
	}

    /**
     * @brief Gets the import names in this use statement (read-only).
     * @return The import names in this use statement.
     */
    inline const std::vector<source::Identifier>& getImports() const {
        return _imports;
    }
    /**
     * @brief Gets the import names in this use statement.
     * @return The import names in this use statement.
     */
    inline std::vector<source::Identifier>& getImports() {
        return _imports;
    }

    /**
     * @brief Checks if the given AST node is a UseStmtNode.
     * @param node The AST node to check.
     * @return True if the given AST node is a UseStmtNode, false otherwise.
     */
    static bool isClassOf(const AstNode* node) {
        return node && node->getNodeKind() == AstKind::UseStmt;
    }

private:
    std::vector<source::Identifier> _modulePath;
    std::vector<source::Identifier> _imports;
};

} // namespace ast
VEEC_NAMESPACE_END
