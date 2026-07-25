/**
 * @file Block.hpp
 * @brief This file contains the definition of the Block class.
 * 
 * The Block class represents a block in the middle intermediate representation (MIR) of the compiler.
 * A block stores a list of instructions and is part of a function.
 */

#pragma once

#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/mir/MirContext.hpp"
#include "veec/mir/MirFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class Block
 * @brief Represents a block in the middle intermediate representation (MIR) of the compiler.
 */
class Block {
public:
    ~Block() = default;

    inline static Block* create(MirContext& ctx) {
        return ctx._arena.create<Block>();
    }

private:
    template<typename>
    friend class Arena;

    std::vector<Instruction*> _instructions;

    Block() = default;
};

} // namespace mir
VEEC_NAMESPACE_END
