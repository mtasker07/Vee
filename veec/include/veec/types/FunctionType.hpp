/**
 * @file FunctionType.hpp
 * @brief This file contains the definition of the FunctionType class.
 *
 * The function type class represents function types defined by the user.
 */

#pragma once

#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/TypeId.hpp"
#include "veec/types/TypeKind.hpp"
#include "veec/types/Type.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/**
 * @brief Represents a function type in the Vee language.
 */
class FunctionType : public Type {
public:
    /**
     * @brief Creates a function type with the specified return type and parameter types.
     * @param returnType The return type of the function.
     * @param parameterTypes The parameter types of the function.
     */
    FunctionType(Type* returnType, const std::vector<Type*>& parameterTypes)
        : Type(TypeKind::Function), _returnType(returnType), _parameterTypes(parameterTypes) {}

    virtual ~FunctionType() = default;

    /**
     * @brief Gets the return type of this function type.
     * @return The return type of this function type.
     */
    inline Type* getReturnType() const {
        return _returnType;
    }

    /**
     * @brief Gets the parameter types of this function type.
     * @return The parameter types of this function type.
     */
    inline const std::vector<Type*>& getParameterTypes() const {
        return _parameterTypes;
    }
    /**
     * @brief Gets the parameter type at a given index.
     * @param index The index of the parameter type to retrieve.
     * @return The parameter type at the specified index.
     */
    inline Type* getParameterType(size_t index) const {
        VEE_ASSERT(index < _parameterTypes.size(), "Parameter index out of bounds");
        return _parameterTypes[index];
    }
    /**
     * @brief Gets the number of parameters of this function type.
     * @return The number of parameters of this function type.
     */
    inline size_t getParameterCount() const {
        return _parameterTypes.size();
    }

    /**
     * @brief Gets the static kind of this type, which is Function.
     * @return The static kind of this type, which is Function.
     */
    static TypeKind getStaticKind() { return TypeKind::Function; }

private:
    Type* _returnType = nullptr;
    std::vector<Type*> _parameterTypes;
};

} // namespace types
VEEC_NAMESPACE_END
