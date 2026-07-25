#include "veec/types/TypeTable.hpp"

#include <vector>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/ErrorType.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/types/PointerType.hpp"
#include "veec/types/ArrayType.hpp"
#include "veec/types/FunctionType.hpp"
#include "veec/types/ClassType.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

ErrorType* TypeTable::getError() {
    if (_errorType == nullptr) {
        _errorType = createType<ErrorType>();
    }
    return _errorType;
}
BuiltinType* TypeTable::getBuiltin(BuiltinTypeKind kind) {
    // Lazily build all builtin types if not already done
    if (_builtinTypes.empty()) {
        for (u8 i = 0; i < static_cast<u8>(BuiltinTypeKind::Count); ++i) {
            BuiltinTypeKind builtinKind = static_cast<BuiltinTypeKind>(i);
            BuiltinType* newBuiltinType = createType<BuiltinType>(builtinKind);
            _builtinTypes.push_back(newBuiltinType);
        }
    }
    return _builtinTypes[static_cast<size_t>(kind)];
}
PointerType* TypeTable::getPointer(Type* pointeeType) {
    if (auto it = _pointerTypes.find(pointeeType); it != _pointerTypes.end()) {
        return it->second;
    }

    // If not found, create a new pointer type
    PointerType* newPointerType = createType<PointerType>(pointeeType);
    _pointerTypes[pointeeType] = newPointerType;
    return newPointerType;
}
ArrayType* TypeTable::getArray(Type* elementType, size_t size) {
    ArrayKey key{elementType, size};
    if (auto it = _arrayTypes.find(key); it != _arrayTypes.end()) {
        return it->second;
    }

    // If not found, create a new array type
    ArrayType* newArrayType = createType<ArrayType>(elementType, size);
    _arrayTypes[key] = newArrayType;
    return newArrayType;
}
FunctionType* TypeTable::getFunction(Type* returnType, const std::vector<Type*>& parameterTypes) {
    FunctionKey key{returnType, parameterTypes};
    if (auto it = _functionTypes.find(key); it != _functionTypes.end()) {
        return it->second;
    }

    // If not found, create a new function type
    FunctionType* newFunctionType = createType<FunctionType>(returnType, parameterTypes);
    _functionTypes[key] = newFunctionType;
    return newFunctionType;
}
ClassType* TypeTable::getClass(symbols::ClassSymbol* classSymbol) {
    if (auto it = _classTypes.find(classSymbol); it != _classTypes.end()) {
        return it->second;
    }

    // If not found, create a new class type
    ClassType* newClassType = createType<ClassType>(classSymbol);
    _classTypes[classSymbol] = newClassType;
    return newClassType;
}

Type* TypeTable::getNodeType(ast::AstNode* node) {
    auto it = _astNodeTypes.find(node);
    if (it != _astNodeTypes.end()) {
        return it->second;
    }
    return nullptr;
}
void TypeTable::setNodeType(ast::AstNode* node, Type* type) {
    _astNodeTypes[node] = type;
}

const Type& TypeTable::get(TypeId id) const {
    VEE_ASSERT(Type::isValidId(id), "Invalid type ID!");

    size_t index = typeIdToIndex(id);
    VEE_ASSERT(index < _types.size(), "Type ID out of range!");

    return *_types[index];
}
Type& TypeTable::get(TypeId id) {
    VEE_ASSERT(Type::isValidId(id), "Invalid type ID!");

    size_t index = typeIdToIndex(id);
    VEE_ASSERT(index < _types.size(), "Type ID out of range!");

    return *_types[index];
}

} // namespace types
VEEC_NAMESPACE_END
