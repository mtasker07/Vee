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
#include "veec/mir/MirNode.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/support/ConstantTable.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class Module
 * @brief Represents a module in the MIR. A module is a collection of Functions.
 */
class Module : public MirNode {
public:
    Module(MirKey)
        : MirNode(MirKey{}, MirKind::Module) {}

    ~Module() = default;

    /**
     * @brief Gets all the functions contained in this module.
     * @return A list of functions in this module.
     */
    const std::vector<Function*>& getFunctions() const { return _functions; }
    /**
     * @brief Sets the number of expected modules for this Module. This is used
     * to reserve space in the internal vector and avoid reallocations when the number
     * of functions can be estimated beforehand.
     * @param expectedCount The expected number of functions.
     */
    void expectFunctions(size_t expectedCount);
    /**
     * @brief Adds a function to this module. The function must not belong to a module already.
     * @param func The function to add.
     */
    void addFunction(Function* func);

    /**
     * @brief Gets all the constants contained in this module.
     * @return A list of constants in this module.
     */
    const std::vector<Constant*>& getConstants() const { return _constants.getAllConstants(); }

private:
    friend class MirFactory;

    std::vector<Function*> _functions;
    support::ConstantTable _constants;
};

} // namespace mir
VEEC_NAMESPACE_END
