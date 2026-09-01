/**
 * @file TypeTable.hpp
 * @brief This file contains the definition of the TypeTable class.
 * 
 * The TypeTable manages types within a module, and ensures that each unique type
 * is only created once. It provides methods to retrieve existing types or create new ones
 * if they do not already exist.
 */

#pragma once

#include <vector>
#include <unordered_map>
#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/Arena.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirType.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace support {

/**
 * @class TypeTable
 * @brief Represents a table of types in the MIR.
 */
class TypeTable {
public:
    TypeTable() = default;
    ~TypeTable() = default;

    /**
     * @brief Gets the void type (read-only).
     * @return A pointer to the void type.
     */
    const MirVoidType* getVoid();
    /**
     * @brief Gets the boolean type (read-only)
     * @return A pointer to the boolean type.
     */
    const MirBoolType* getBool();

    /**
     * @brief Gets an integer type with the specified bit width and signedness (read-only).
     * @param bitWidth The bit width of the integer type.
     * @param isSigned Whether the integer type is signed.
     * @return A pointer to the integer type.
     */
    const MirIntegerType* getInteger(u32 bitWidth, bool isSigned);
    /**
     * @brief Gets a floating-point type with the specified bit width (read-only).
     * @param bitWidth The bit width of the floating-point type.
     * @return A pointer to the floating-point type.
     */
    const MirFloatType* getFloat(u32 bitWidth);

    /**
     * @brief Gets a pointer type to the specified pointee type (read-only).
     * @param pointeeType The type being pointed to.
     * @return A pointer to the pointer type.
     */
    const MirPointerType* getPointer(const MirType* pointeeType);
    /**
     * @brief Gets an array type with the specified element type and size (read-only).
     * @param elementType The type of the array elements.
     * @param size The number of elements in the array.
     * @return A pointer to the array type.
     */
    const MirArrayType* getArray(const MirType* elementType, u64 size);

    /**
     * @brief Gets a function type with the specified return type and parameter types (read-only).
     * @param returnType The return type of the function.
     * @param paramTypes The types of the function parameters.
     * @return A pointer to the function type.
     */
    const MirFunctionType* getFunction(const MirType* returnType, const basic::SmallVector<const MirType*>& paramTypes);

    /**
     * @brief Creates an opaque struct type (a struct with no defined members).
     * The struct members should be filled later if necessary. Since structs at this stage
     * don't really have anything to uniquely identify them, all are considered unique, regardless
     * of member types. Therefore, it is the responsibility of the user to manage and track
     * struct instances appropriately.
     * @return A pointer to the opaque struct type.
     */
    MirStructType* createOpaqueStruct();
    /**
     * @brief Creates a struct type with the specified member types. Since structs at this stage
     * don't really have anything to uniquely identify them, all are considered unique, regardless
     * of member types. Therefore, it is the responsibility of the user to manage and track
     * struct instances appropriately.
     * @param memberTypes The types of the struct members.
     * @return A pointer to the struct type.
     */
    MirStructType* createStruct(const basic::SmallVector<const MirType*>& memberTypes);

    /**
     * @brief Gets the string struct type. Since there isn't a native primitive string type,
     * this struct is used to represent the built-in string type. Unlike other structs, there is
     * only one string struct type and it cannot be modified.
     * @return A pointer to the string struct type.
     */
    const MirStructType* getStringStruct();

private:
    basic::Arena<> _typeArena;
    std::vector<MirType*> _types;
    // Only one void and bool type
    MirVoidType* _voidType = nullptr;
    MirBoolType* _boolType = nullptr;
    // vv bitWidth is key for integer and float types, in each pair first is unsigned, second is signed
    std::unordered_map<u32, std::pair<MirIntegerType*, MirIntegerType*>> _integers;
    std::unordered_map<u32, MirFloatType*> _floats;
    // vv elementType is key for pointer types
    std::unordered_map<const MirType*, MirPointerType*> _pointers;
    // vv elementType and size are key for array types
    // dont use a second unordered map since its probably less efficient
    std::unordered_map<const MirType*, std::vector<std::pair<u64, MirArrayType*>>> _arrays;
    // vv returnType and paramTypes are key for function types
    std::unordered_map<const MirType*, std::vector<std::pair<basic::SmallVector<const MirType*>, MirFunctionType*>>> _functions;
    // Don't intern structs, consider each unique regardless of member types
    std::vector<MirStructType*> _structs;
    // One unique string struct type
    MirStructType* _stringStruct = nullptr;
    
    // Get next unique struct ID
    u32 getNextStructId();
};

} // namespace support
} // namespace mir
VEEC_NAMESPACE_END
