#include "veec/mir/Module.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/mir/MirContext.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Function.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {

void Module::expectFunctions(size_t expectedCount) {
    _functions.reserve(expectedCount);
}
void Module::addFunction(Function* func) {
    VEE_ASSERT(func != nullptr, "Function cannot be null!");
    VEE_ASSERT(func->getModule() == nullptr, "Function already belongs to a module!");

    _functions.push_back(func);
    func->_module = this;
}

} // namespace mir
VEEC_NAMESPACE_END
