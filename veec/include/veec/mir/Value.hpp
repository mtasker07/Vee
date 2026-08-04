/**
 * @file Value.hpp
 * @brief This file contains the definition of the Value class and its related/derived
 * classes.
 * 
 * The Value class represents a value in the MIR.
 */

#pragma once

#include <vector>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirKind.hpp"
#include "veec/mir/MirNode.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/symbols/SymbolFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

//
// VALUE
//

/**
 * @class Value
 * @brief Represents a value in the MIR. A value can be anything that either holds a value or produces a value.
 * For example, an instruction is a value because it sometimes produces one.
 */
class Value : public MirNode {
public:
    ~Value() = default;

protected:
    Value(MirKey, MirKind kind)
        : MirNode(MirKey{}, kind) {}
};

//
// USER
//

/**
 * @brief A user is a value that can have other values as operands.
 */
class User : public Value {
public:
    using OperandList = basic::SmallVector<Value*, 2>;

    ~User() = default;

    //
    // Operands
    //

    /**
     * @brief Gets the operands of this user.
     * @return A list of operands of this user.
     */
    inline const OperandList& getOperands() const {
        return _operands;
    }
    /**
     * @brief Gets the number of operands of this user.
     * @return The number of operands of this user.
     */
    inline size_t getOperandCount() const {
        return _operands.size();
    }
    /**
     * @brief Gets the operand at the given index.
     * @param index The index of the operand to get.
     */
    inline Value* getOperand(size_t index) const {
        VEE_ASSERT(index < _operands.size(), "Index out of bounds");
        return _operands[index];
    }
    /**
     * @brief Adds an operand to this user.
     * @param value The operand to add.
     */
    inline void addOperand(Value* value) {
        _operands.push_back(value);
    }

protected:
    User(MirKey key, MirKind kind)
        : Value(key, kind) {}
    User(MirKey key, MirKind kind, OperandList&& operands)
        : Value(key, kind), _operands(std::move(operands)) {}

private:
    OperandList _operands;
};

//
// LOCAL
//

/**
 * @enum LocalKind
 * @brief Represents the kind of a local.
 */
enum class LocalKind : u8 {
    Variable,
    Parameter
};

/**
 * @class Local
 * @brief Represents a local variable or parameter in the middle intermediate representation (MIR) of the compiler.
 */
class Local : public Value {
public:
    Local(
        MirKey key,
        Function* func,
        LocalKind kind,
        symbols::VariableSymbol* sym
    )
        : Value(key, MirKind::Local),
        _func(func),
        _kind(kind),
        _symbol(sym) {}

    ~Local() = default;

    /**
     * @brief Gets the function to which this local belongs.
     * @return A pointer to the function that owns this local.
     */
    inline Function* getFunction() const { return _func; }

    /**
     * @brief Gets the kind of this local.
     * @return The kind of this local.
     */
    inline LocalKind getLocalKind() const { return _kind; }

    /**
     * @brief Gets the variable symbol associated with this local.
     * @return A pointer to the variable symbol associated with this local.
     */
    inline symbols::VariableSymbol* getSymbol() const { return _symbol; }

private:
    friend class MirFactory;
    friend class Function;

    Function* _func;
    LocalKind _kind;
    symbols::VariableSymbol* _symbol;
};

} // namespace mir
VEEC_NAMESPACE_END