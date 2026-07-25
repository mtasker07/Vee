/**
 * @file OperatorTable.hpp
 * @brief This file contains the definition of the operator table class.
 * 
 * The operator table class is responsible for managing operators in the semantic analysis phase.
 */

#pragma once

#include <vector>
#include <span>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/ent/OperatorSymbol.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @class OperatorTable
 * @brief Helper class for indexing operators and handling lookup.
 */
class OperatorTable {
public:
    /**
     * @brief Creates a new OperatorTable instance.
     */
    OperatorTable() = default;
    ~OperatorTable() = default;

    /**
     * @brief Adds an operator to the operator table.
     * @param op The operator to add to the operator table.
     */
    inline void addOperator(OperatorSymbol* op) {
        if (op->getOperatorKind() == OperatorSymbolKind::Unary) {
            _unaryOperators[op->getUnaryOperatorKind()].push_back(op);
        } else if (op->getOperatorKind() == OperatorSymbolKind::Binary) {
            _binaryOperators[op->getBinaryOperatorKind()].push_back(op);
        }
        else VEE_UNREACHABLE("Unknown operator kind");
    }

    /**
     * @brief Gets all the operators of a unary operator with the specified kind.
     * @param kind The kind of the unary operator to look up.
     * @return A span of operators with the specified unary operator kind.
     */
    inline std::span<OperatorSymbol* const> getUnaryOperators(UnaryOperatorKind kind) const {
        auto it = _unaryOperators.find(kind);
        if (it != _unaryOperators.end()) {
            return it->second;
        }
        return {};
    }
    /**
     * @brief Gets all the operators of a binary operator with the specified kind.
     * @param kind The kind of the binary operator to look up.
     * @return A span of operators with the specified binary operator kind.
     */
    inline std::span<OperatorSymbol* const> getBinaryOperators(BinaryOperatorKind kind) const {
        auto it = _binaryOperators.find(kind);
        if (it != _binaryOperators.end()) {
            return it->second;
        }
        return {};
    }

private:
    std::unordered_map<UnaryOperatorKind, std::vector<OperatorSymbol*>> _unaryOperators;
    std::unordered_map<BinaryOperatorKind, std::vector<OperatorSymbol*>> _binaryOperators;
};

} // namespace symbols
VEEC_NAMESPACE_END
