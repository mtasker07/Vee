/**
 * @file Function.hpp
 * @brief This file contains the definition of the Function class.
 * 
 * The Function class represents a function in the middle intermediate representation (MIR) of the compiler.
 */

#pragma once

#include <vector>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirNode.hpp"
#include "veec/mir/MirType.hpp"
#include "veec/mir/Value.hpp"
#include "veec/mir/Argument.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class Function
 * @brief Represents a function in the MIR. A function is a collection of basic
 * blocks that can be called and executed. Functions have parameters and a return type.
 * Functions own all the local variables created within them. Functions can also
 * be called from other functions, hence, Function is a subclass of Value.
 */
class Function : public Value {
public:
    Function(
        MirKey key,
        const MirFunctionType* type
    )
        : Value(key, MirKind::Function, type) {}

    ~Function() = default;

    /**
     * @brief Gets the module to which this function belongs.
     * @return A pointer to the module that owns this function. Nullptr
     * if the function is not owned by any module.
     */
    inline Module* getModule() const { return _module; }

    /**
     * @brief Gets the number of arguments in this function.
     * @return The number of arguments in this function.
     */
    inline size_t getArgCount() const { return _args.size(); }
    /**
     * @brief Gets the arguments of this function.
     * @return A list of arguments of this function.
     */
    inline const basic::SmallVector<Argument*>& getArgs() const {
        return _args;
    }
    /**
     * @brief Gets the argument at the given index.
     * @param index The index of the argument to get.
     * @return A pointer to the argument at the given index.
     */
    inline Argument* getArg(size_t index) const {
        VEE_ASSERT(index < _args.size(), "Index out of bounds");
        return _args[index];
    }

    /**
     * @brief Gets the function type of this function (read-only).
     * @return The function type of this function.
     */
    inline const MirFunctionType* getFunctionType() const {
        return static_cast<const MirFunctionType*>(getType());
    }
    /**
     * @brief Gets the return type of this function (read-only).
     * @return The return type of this function.
     */
    inline const MirType* getReturnType() const {
        return getFunctionType()->getReturnType();
    }
    /**
     * @brief Gets the parameter types of this function (read-only).
     * @return The parameter types of this function.
     */
    inline const basic::SmallVector<const MirType*>& getParameterTypes() const {
        return getFunctionType()->getParameterTypes();
    }

    /**
     * @brief Gets the basic blocks of this function (read-only).
     * @return A list of basic blocks of this function.
     */
    inline const std::vector<BasicBlock*>& getBlocks() const { return _blocks; }
    /**
     * @brief Gets the basic block at the given index in this function.
     * @param index The index of the basic block to get.
     * @return A pointer to the basic block at the given index.
     */
    inline BasicBlock* getBlock(size_t index) const {
        VEE_ASSERT(index < _blocks.size(), "Index out of bounds");
        return _blocks[index];
    }
    /**
     * @brief Gets the entry block of this function.
     * @return A pointer to the entry block of this function, or nullptr if the function has no blocks.
     * @note You should ALWAYS use this function instead of getBlock(0), since that will assert if the function
     * has no blocks instead of returning null.
     */
    inline BasicBlock* getEntryBlock() const {
        return _blocks.empty() ? nullptr : _blocks.front();
    }
    /**
     * @brief Adds a basic block to this function. The block must not belong to
     * a function already.
     * @param block The block to add.
     */
    void addBlock(BasicBlock& block);

private:
    friend class MirFactory;
    friend class Module;

    Module* _module = nullptr;
    basic::SmallVector<Argument*> _args;
    
    std::vector<BasicBlock*> _blocks;
};

} // namespace mir
VEEC_NAMESPACE_END
