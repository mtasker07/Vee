/**
 * @file TypeTable.hpp
 * @brief This file contains the definition of the type table class.
 * 
 * The type table class is responsible for managing types.
 */

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <unordered_map>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/types/TypeId.hpp"
#include "veec/types/TypeKind.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/util/HashUtils.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/// @brief Manages all types.
class TypeTable {
public:
    /**
     * @brief Creates a new TypeTable instance.
     */
    TypeTable() = default;
    ~TypeTable() = default;

    /**
     * @brief Gets the error type.
     * @return A pointer to the error type object.
     */
    ErrorType* getError();
    /**
     * @brief Gets a builtin type by its kind.
     * @param kind The kind of the builtin type to retrieve.
     * @return A pointer to the builtin type object.
     */
    BuiltinType* getBuiltin(BuiltinTypeKind kind);
    /**
     * @brief Gets a pointer type by its pointee type.
     * @param pointeeType The type that the pointer type points to.
     */
    PointerType* getPointer(Type* pointeeType);
    /**
     * @brief Gets an array type by its element type and size.
     * @param elementType The type of the array elements.
     * @param size The size of the array.
     * @return A pointer to the array type object.
     */
    ArrayType* getArray(Type* elementType, size_t size);
    /**
     * @brief Gets a function type by its return type and parameter types.
     * @param returnType The return type of the function type.
     */
    FunctionType* getFunction(Type* returnType, const std::vector<Type*>& parameterTypes);
    /**
     * @brief Gets a class type by its associated class symbol.
     * @param classSymbol The class symbol associated with the class type to retrieve.
     * @return A pointer to the class type object.
     */
    ClassType* getClass(symbols::ClassSymbol* classSymbol);

    //
    // Builtin helpers
    //

    /**
     * @brief Gets the void type.
     * @return A pointer to the void type object.
     */
    BuiltinType* getVoid();
    /**
     * @brief Gets the bool type.
     * @return A pointer to the bool type object.
     */
    BuiltinType* getBool();
    /**
     * @brief Gets the string type.
     * @return A pointer to the string type object.
     */
    BuiltinType* getString();
    /**
     * @brief Gets the i8 type.
     * @return A pointer to the i8 type object.
     */
    BuiltinType* getI8();
    /**
     * @brief Gets the i16 type.
     * @return A pointer to the i16 type object.
     */
    BuiltinType* getI16();
    /**
     * @brief Gets the i32 type.
     * @return A pointer to the i32 type object.
     */
    BuiltinType* getI32();
    /**
     * @brief Gets the i64 type.
     * @return A pointer to the i64 type object.
     */
    BuiltinType* getI64();
    /**
     * @brief Gets the u8 type.
     * @return A pointer to the u8 type object.
     */
    BuiltinType* getU8();
    /**
     * @brief Gets the u16 type.
     * @return A pointer to the u16 type object.
     */
    BuiltinType* getU16();
    /**
     * @brief Gets the u32 type.
     * @return A pointer to the u32 type object.
     */
    BuiltinType* getU32();
    /**
     * @brief Gets the u64 type.
     * @return A pointer to the u64 type object.
     */
    BuiltinType* getU64();
    /**
     * @brief Gets the f32 type.
     * @return A pointer to the f32 type object.
     */
    BuiltinType* getF32();
    /**
     * @brief Gets the f64 type.
     * @return A pointer to the f64 type object.
     */
    BuiltinType* getF64();


    /**
     * @brief Gets the type of a given AST node.
     * @param node The AST node to get the type for.
     * @return A pointer to the type of the AST node, or nullptr if not set.
     */
    Type* getNodeType(const ast::AstNode* node);
    /**
     * @brief Sets the type for a given AST node.
     * @param node The AST node to set the type for.
     * @param type The type to set for the AST node.
     */
    void setNodeType(const ast::AstNode* node, Type* type);

    /**
     * @brief Gets a type by ID in this type table (read-only).
     * @param id The ID of the type to retrieve.
     * @return A reference to the type.
     */
    const Type& get(TypeId id) const;
    /**
     * @brief Gets a type by ID in this type table.
     * @param id The ID of the type to retrieve.
     * @return A reference to the type.
     */
    Type& get(TypeId id);

    /**
     * @brief Gets all types in this type table (read-only).
     * @return A vector of all types in this type table, including built-in types.
     * @note It is generally advised not to use this function unless
     * for debugging purposes.
     */
    const std::vector<Type*>& getAllTypes() const { return _types; }

private:
    using ArrayKey = util::HashUtils::CompositeKey<Type*, size_t>;
    using ArrayHasher = util::HashUtils::CompositeHasher;
    using FunctionKey = util::HashUtils::CompositeKey<Type*, std::vector<Type*>>;
    using FunctionHasher = util::HashUtils::CompositeHasher;

    basic::Arena<> _typeArena;
    std::vector<Type*> _types; // by id
    ErrorType* _errorType = nullptr;
    std::vector<BuiltinType*> _builtinTypes; // by kind
    std::unordered_map<Type*, PointerType*> _pointerTypes; // by pointee
    std::unordered_map<ArrayKey, ArrayType*, ArrayHasher> _arrayTypes; // by (elementType, size)
    std::unordered_map<FunctionKey, FunctionType*, FunctionHasher> _functionTypes; // by (returnType, parameterTypes)
    std::unordered_map<symbols::ClassSymbol*, ClassType*> _classTypes; // by symbol
    
    std::unordered_map<const ast::AstNode*, Type*> _astNodeTypes;

    // Index conversion helpers
    inline size_t typeIdToIndex(TypeId id) const {
        VEE_ASSERT(Type::isValidId(id), "Invalid type ID!");
        return static_cast<size_t>(id - 1);
    }
    inline TypeId indexToTypeId(size_t index) const {
        return static_cast<TypeId>(index + 1);
    }

    // Create type object of kind T with unique ID
    template<typename T, typename... Args>
    inline T* createType(Args&&... args) {
        static_assert(std::is_base_of_v<Type, T>, "T must be derived from Type");

        T* newType = _typeArena.create<T>(std::forward<Args>(args)...);
        TypeId id = indexToTypeId(_types.size());
        newType->assignId(id);
        _types.push_back(newType);
        return newType;
    }
};

} // namespace types
VEEC_NAMESPACE_END
