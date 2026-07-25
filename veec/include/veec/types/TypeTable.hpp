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

    /**
     * @brief Gets the type of a given AST node.
     * @param node The AST node to get the type for.
     * @return A pointer to the type of the AST node, or nullptr if not set.
     */
    Type* getNodeType(ast::AstNode* node);
    /**
     * @brief Sets the type for a given AST node.
     * @param node The AST node to set the type for.
     * @param type The type to set for the AST node.
     */
    void setNodeType(ast::AstNode* node, Type* type);

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
    //
    // KEYS
    //

    struct ArrayKey {
        Type* elementType;
        size_t size;

        bool operator==(const ArrayKey& other) const {
            return elementType == other.elementType && size == other.size;
        }
    };

    struct FunctionKey {
        Type* returnType;
        std::vector<Type*> parameterTypes;

        bool operator==(const FunctionKey& other) const {
            if (returnType != other.returnType) return false;
            if (parameterTypes.size() != other.parameterTypes.size()) return false;
            for (size_t i = 0; i < parameterTypes.size(); ++i) {
                if (parameterTypes[i] != other.parameterTypes[i]) return false;
            }
            return true;
        }
    };

    //
    // KEY HASHERS
    //

    struct ArrayKeyHasher {
        std::size_t operator()(const ArrayKey& key) const {
            std::size_t h1 = std::hash<Type*>{}(key.elementType);
            std::size_t h2 = std::hash<size_t>{}(key.size);
            return h1 ^ (h2 << 1); // Combine the two hashes
        }
    };

    struct FunctionKeyHasher {
        std::size_t operator()(const FunctionKey& key) const {
            std::size_t h1 = std::hash<Type*>{}(key.returnType);
            std::size_t h2 = 0;
            for (const auto& paramType : key.parameterTypes) {
                h2 ^= std::hash<Type*>{}(paramType) + 0x9e3779b9 + (h2 << 6) + (h2 >> 2);
            }
            return h1 ^ (h2 << 1);
        }
    };


    basic::Arena<> _typeArena;
    std::vector<Type*> _types; // by id
    ErrorType* _errorType = nullptr;
    std::vector<BuiltinType*> _builtinTypes; // by kind
    std::unordered_map<Type*, PointerType*> _pointerTypes; // by pointee
    std::unordered_map<ArrayKey, ArrayType*, ArrayKeyHasher> _arrayTypes; // by (elementType, size)
    std::unordered_map<FunctionKey, FunctionType*, FunctionKeyHasher> _functionTypes; // by (returnType, parameterTypes)
    std::unordered_map<symbols::ClassSymbol*, ClassType*> _classTypes; // by symbol
    
    std::unordered_map<ast::AstNode*, Type*> _astNodeTypes;

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
