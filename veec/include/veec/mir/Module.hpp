/**
 * @file Module.hpp
 * @brief This file contains the definition of the Module class.
 * 
 * The Module class represents a module in the middle intermediate representation (MIR) of the compiler.
 * A module is a collection of functions and other top-level entities that are compiled together.
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
 * @class Module
 * @brief Represents a module in the middle intermediate representation (MIR) of the compiler.
 */
class Module {
public:
    ~Module() = default;

    inline static Module* create(MirContext& ctx) {
        return ctx._arena.create<Module>();
    }

private:
    template<typename>
    friend class Arena;

    std::vector<Function*> _functions;

    Module() = default;
};

} // namespace mir
VEEC_NAMESPACE_END
