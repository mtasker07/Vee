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

    /**
     * @brief Gets the type of this value.
     * @return The type of this value, or nullptr if it has no type.
     */
    inline const MirType* getType() const { return _type; }

protected:
    Value(MirKey key, MirKind kind, const MirType* type = nullptr)
        : MirNode(key, kind), _type(type) {}

private:
    friend class MirFactory;

    const MirType* _type = nullptr;
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
    User(MirKey key, MirKind kind, const MirType* type = nullptr)
        : Value(key, kind, type) {}
    User(MirKey key, MirKind kind, OperandList&& operands, const MirType* type = nullptr)
        : Value(key, kind, type), _operands(std::move(operands)) {}

private:
    OperandList _operands;
};

} // namespace mir
VEEC_NAMESPACE_END