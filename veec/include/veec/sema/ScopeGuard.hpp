/**
 * @file ScopeGuard.hpp
 * @brief This file contains the definition of the ScopeGuard class.
 * The ScopeGuard class is used to ensure that the current scope is correctly restored
 * when exiting a block.
 */

#pragma once

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/sema/Scope.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema {

/**
 * @class ScopeGuard
 * @brief Ensures that the current scope is correctly restored when exiting a block.
 */
class ScopeGuard {
public:
    /**
     * @brief Creates a new ScopeGuard instance.
     * @param currentScope A pointer to the current scope pointer, which will be updated to the new scope
     * and restored to the previous scope when the ScopeGuard is destroyed.
     * @param newScope The new scope to set as the current scope.
     */
    ScopeGuard(Scope** currentScope, Scope* newScope)
        : _currentScope(currentScope), _newScope(newScope) {
        enter();
    }

    /**
     * @brief Destroys the ScopeGuard instance, restoring the previous scope.
     */
    ~ScopeGuard() {
        exit();
    }

private:
    Scope** _currentScope = nullptr;
    Scope* _newScope = nullptr;

    inline void enter() {
        *_currentScope = _newScope;
    }
    inline void exit() {
        *_currentScope = (*_currentScope)->getParent();
    }
};

} // namespace sema
VEEC_NAMESPACE_END
