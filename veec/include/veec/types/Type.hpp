/**
 * @file Type.hpp
 * @brief This file contains the definition of the type class.
 * 
 * The type class is the base class for all Vee data types.
 * Types represent data types in the program, such as i32.
 * 
 * Every Type object should represent a single unique type in the program.
 * For example, i32 should only have one Type object representing it.
 * This makes it easy to compare types by comparing their pointers.
 * This logic is enforced in the TypeTable class, which is responsible for
 * creating and managing ALL Type objects.
 */

#pragma once

#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/types/TypeId.hpp"
#include "veec/types/TypeKind.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/**
 * @class Type
 * @brief The base class for all types in the semantic analysis phase.
 * Types represent various entities in the program, such as functions.
 */
class Type {
public:
    virtual ~Type() = default;

    /**
     * @brief Checks if the given type ID is valid (non-zero).
     * @param id The type ID to check.
     * @return True if the type ID is valid, false otherwise.
     */
    static inline bool isValidId(TypeId id) {
        return id != 0;
    }

    /**
     * @brief Gets the unique identifier of this type.
     * @return The unique identifier of this type.
     */
    inline TypeId getId() const { return _id; }
    /**
     * @brief Gets the kind of this type.
     * @return The kind of this type.
     */
    inline TypeKind getKind() const { return _kind; }

    /**
     * @brief Gets the static kind of this type.
     * @return The static kind of this type.
     */
    template<typename T>
    inline bool is() const {
        static_assert(std::is_base_of_v<Type, T>, "T must be derived from Type");
        return getKind() == T::getStaticKind();
    }
    /**
     * @brief Casts this type to the specified type (read-only).
     * @tparam T The type to cast to, which must be derived from Type.
     * @return A pointer to this type cast to the specified type, or nullptr if this type
     * is not of the specified type.
     */
    template<typename T>
    inline const T* as() const {
        static_assert(std::is_base_of_v<Type, T>, "T must be derived from Type");

        if (is<T>())
            return static_cast<const T*>(this);
        return nullptr;
    }
    /**
     * @brief Casts this type to the specified type.
     * @tparam T The type to cast to, which must be derived from Type.
     * @return A pointer to this type cast to the specified type, or nullptr if this type
     * is not of the specified type.
     */
    template<typename T>
    inline T* as() {
        static_assert(std::is_base_of_v<Type, T>, "T must be derived from Type");

        if (is<T>())
            return static_cast<T*>(this);
        return nullptr;
    }

protected:
    /**
     * @brief Constructs a new Type instance with the specified name and kind.
     * @param kind The kind of the type.
     */
    Type(TypeKind kind)
        : _id(0), _kind(kind) {}

private:
    friend class TypeTable; // For ID assignment

    TypeId _id = 0;
    TypeKind _kind;

    // MUST be called upon construction!!
    inline void assignId(TypeId id) {
        VEE_ASSERT(isValidId(id), "Invalid type ID!");
        VEE_ASSERT(!isValidId(_id), "Type ID already assigned!");
        
        _id = id;
    }
};

} // namespace types
VEEC_NAMESPACE_END
