/**
 * @file OperatorSymbol.hpp
 * @brief This file contains the definition of the operator symbol class.
 *
 * The operator symbol class represents operators in the program.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/ScopeOwnerSymbol.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/SymbolKind.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/Visibility.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace symbols {

/**
 * @enum OperatorImplementation
 * @brief Represents the implementation type of an operator, such as builtin
 * or user-defined.
 */
enum class OperatorImplementation : u8 {
    Builtin,
    UserDefined,
};

/**
 * @enum OperatorSymbolKind
 * @brief Represents the kind of an operator, such as unary or binary.
 */
enum class OperatorSymbolKind : u8 {
    Unary,
    Binary,
};

/**
 * @enum UnaryOperatorKind
 * @brief Represents the kind of a unary operator, such as negation (-).
 */
enum class UnaryOperatorKind : u8 {
    Plus,
    Minus,
    Increment, // Pre increment
    Decrement, // Pre decrement
    LogicalNot,
    BitwiseNot,
    Dereference,
    AddressOf
};

/**
 * @brief Represents the kind of a binary operator, such as addition (+).
 */
enum class BinaryOperatorKind : u8 {
    // ARITHMETIC
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulo,
    // COMPARISON
    Equal,
    NotEqual,
    LessThan,
    LessThanOrEqual,
    GreaterThan,
    GreaterThanOrEqual,
    // BITWISE
    BitwiseAnd,
    BitwiseOr,
    BitwiseXor,
    ShiftLeft,
    ShiftRight,
    // LOGICAL
    LogicalAnd,
    LogicalOr,
};

/**
 * @brief The operator symbol represents an operator in the program.
 */
class OperatorSymbol : public Symbol {
public:
    /**
     * @brief Creates a new OperatorSymbol instance with a given unary operator.
     * @param impl The implementation type of the operator.
     * @param unaryOp The kind of unary operator.
     */
    OperatorSymbol(
        OperatorImplementation impl,
        UnaryOperatorKind unaryOp,
        types::Type* resultType,
        const std::vector<types::Type*>& operandTypes
    )
        : Symbol(SymbolKind::Operator),
        _implementation(impl),
        _operatorKind(OperatorSymbolKind::Unary),
        _unaryOp(unaryOp),
        _resultType(resultType),
        _operandTypes(operandTypes) {}
    /**
     * @brief Creates a new OperatorSymbol instance with a given binary operator.
     * @param impl The implementation type of the operator.
     * @param binaryOp The kind of binary operator.
     */
    OperatorSymbol(
        OperatorImplementation impl,
        BinaryOperatorKind binaryOp,
        types::Type* resultType,
        const std::vector<types::Type*>& operandTypes
    )
        : Symbol(SymbolKind::Operator),
        _implementation(impl),
        _operatorKind(OperatorSymbolKind::Binary),
        _binaryOp(binaryOp),
        _resultType(resultType),
        _operandTypes(operandTypes) {}

    virtual ~OperatorSymbol() = default;

    /**
     * @brief Gets the implementation kind of this operator symbol.
     * @return The implementation kind of this operator symbol.
     */
    inline OperatorImplementation getImplementation() const { return _implementation; }

    /**
     * @brief Gets the operator kind of this operator symbol.
     * @return The operator kind of this operator symbol.
     */
    inline OperatorSymbolKind getOperatorKind() const { return _operatorKind; }

    /**
     * @brief Gets the unary operator kind of this operator symbol.
     * @return The unary operator kind of this operator symbol.
     */
    inline UnaryOperatorKind getUnaryOperatorKind() const {
        VEE_ASSERT(_operatorKind == OperatorSymbolKind::Unary, "Operator is not unary");
        return _unaryOp;
    }
    /**
     * @brief Gets the binary operator kind of this operator symbol.
     * @return The binary operator kind of this operator symbol.
     */
    inline BinaryOperatorKind getBinaryOperatorKind() const {
        VEE_ASSERT(_operatorKind == OperatorSymbolKind::Binary, "Operator is not binary");
        return _binaryOp;
    }

    /**
     * @brief Gets the result type of this operator symbol.
     * @return The result type of this operator symbol.
     */
    inline types::Type* getResultType() const { return _resultType; }
    /**
     * @brief Gets the type of a specific operand at a given index.
     * @param index The index of the operand.
     * @return The type of the operand at the given index.
     */
    inline types::Type* getOperandType(size_t index) const {
        VEE_ASSERT(index < _operandTypes.size(), "Operand index out of bounds");
        return _operandTypes[index];
    }
    /**
     * @brief Gets the types of all operands for this operator symbol.
     * @return A vector of types for all operands of this operator symbol.
     */
    inline const std::vector<types::Type*>& getOperandTypes() const { return _operandTypes; }
    /**
     * @brief Gets the number of operands for this operator symbol.
     * @return The number of operands for this operator symbol.
     */
    inline size_t getOperandCount() const { return _operandTypes.size(); }

    /**
     * @brief Checks if the given symbol is an operator symbol.
     * @param s The symbol to check.
     * @return True if the symbol is an operator symbol, false otherwise.
     */
    static bool isClassOf(const Symbol* s) {
        return s->getKind() == SymbolKind::Operator;
    }

private:
    OperatorImplementation _implementation = OperatorImplementation::Builtin;
    OperatorSymbolKind _operatorKind = OperatorSymbolKind::Binary;
    union {
        UnaryOperatorKind _unaryOp;
        BinaryOperatorKind _binaryOp;
    };
    types::Type* _resultType = nullptr;
    std::vector<types::Type*> _operandTypes;
};

} // namespace symbols
VEEC_NAMESPACE_END
