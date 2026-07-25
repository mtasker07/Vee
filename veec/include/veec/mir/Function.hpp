/**
 * @file Function.hpp
 * @brief This file contains the definition of the Function class.
 * 
 * The Function class represents a function in the middle intermediate representation (MIR) of the compiler.
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
 * @class Function
 * @brief Represents a function in the middle intermediate representation (MIR) of the compiler.
 */
class Function {
public:
    ~Function() = default;

    inline static Function* create(MirContext& ctx) {
        return ctx._arena.create<Function>();
    }

private:
    template<typename>
    friend class Arena;

    std::vector<Block*> _blocks;

    Function() = default;
};

} // namespace mir
VEEC_NAMESPACE_END
