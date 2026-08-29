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
#include "veec/mir/Value.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/types/TypeFwd.hpp"

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
        symbols::FunctionSymbol* sym
    )
        : Value(key, MirKind::Function),
        _sym(sym) {}

    ~Function() = default;

    /**
     * @brief Gets the module to which this function belongs.
     * @return A pointer to the module that owns this function. Nullptr
     * if the function is not owned by any module.
     */
    inline Module* getModule() const { return _module; }

    /**
     * @brief Gets the function symbol associated with this function.
     * @return A pointer to the function symbol associated with this function.
     */
    inline symbols::FunctionSymbol* getSymbol() const { return _sym; }

    /**
     * @brief Gets the number of parameters in this function.
     * @return The number of parameters in this function.
     */
    inline size_t getParameterCount() const { return _parameters.size(); }
    /**
     * @brief Gets the parameters of this function.
     * @return A list of parameters of this function.
     */
    inline const basic::SmallVector<Local*>& getParameters() const {
        return _parameters;
    }
    /**
     * @brief Gets the parameter at the given index.
     * @param index The index of the parameter to get.
     * @return A pointer to the parameter at the given index.
     */
    inline Local* getParameter(size_t index) const {
        VEE_ASSERT(index < _parameters.size(), "Index out of bounds");
        return _parameters[index];
    }
    /**
     * @brief Adds a parameter to this function. The parameter must not belong to
     * a function already.
     * @param param The parameter to add. MUST be of kind LocalKind::Parameter.
     */
    inline void addParameter(Local* param) {
        VEE_ASSERT(param != nullptr, "Parameter cannot be null");
        VEE_ASSERT(param->getLocalKind() == LocalKind::Parameter, "Local is not a parameter");
        _parameters.push_back(param);
    }

    /**
     * @brief Gets the return type of this function.
     * @return The return type of this function.
     */
    inline types::Type* getReturnType() const {
        return _sym->getReturnType();
    }

    /**
     * @brief Gets the locals of this function.
     * @return A list of locals of this function.
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
    symbols::FunctionSymbol* _sym = nullptr;
    basic::SmallVector<Local*> _parameters;

    basic::SmallVector<Local*> _locals;
    std::vector<BasicBlock*> _blocks;
};

} // namespace mir
VEEC_NAMESPACE_END
