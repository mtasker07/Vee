/**
 * @file Argument.hpp
 * @brief This file contains the definition of the Argument class.
 * 
 * The Argument class represents a function argument in the MIR.
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

VEEC_NAMESPACE_BEGIN
namespace mir {

/**
 * @class Argument
 * @brief Represents a function argument in the MIR.
 */
class Argument : public Value {
public:
    Argument(
        MirKey key,
        const MirType* type,
        Function* function
    )
        : Value(key, MirKind::Argument, type),
        _function(function) {}

    ~Argument() = default;

    /**
     * @brief Gets the function to which this argument belongs.
     * @return The function to which this argument belongs.
     */
    inline Function* getFunction() const {
        return _function;
    }

private:
    Function* _function;
};

} // namespace mir
VEEC_NAMESPACE_END
