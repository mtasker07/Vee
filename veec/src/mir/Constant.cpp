#include "veec/mir/Constant.hpp"

#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

//
// CONSTANT
//

const ConstantInt* Constant::asInteger() const {
    if (!isInteger())
        return nullptr;
    return static_cast<const ConstantInt*>(this);
}
ConstantInt* Constant::asInteger() {
    if (!isInteger())
        return nullptr;
    return static_cast<ConstantInt*>(this);
}
const ConstantFloat* Constant::asFloat() const {
    if (!isFloat())
        return nullptr;
    return static_cast<const ConstantFloat*>(this);
}
ConstantFloat* Constant::asFloat() {
    if (!isFloat())
        return nullptr;
    return static_cast<ConstantFloat*>(this);
}
const ConstantString* Constant::asString() const {
    if (!isString())
        return nullptr;
    return static_cast<const ConstantString*>(this);
}
ConstantString* Constant::asString() {
    if (!isString())
        return nullptr;
    return static_cast<ConstantString*>(this);
}
const ConstantBool* Constant::asBool() const {
    if (!isBool())
        return nullptr;
    return static_cast<const ConstantBool*>(this);
}
ConstantBool* Constant::asBool() {
    if (!isBool())
        return nullptr;
    return static_cast<ConstantBool*>(this);
}

} // namespace mir
VEEC_NAMESPACE_END
