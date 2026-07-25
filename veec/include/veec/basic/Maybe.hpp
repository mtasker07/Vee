/**
 * @file Maybe.hpp
 * @brief This file contains the definition of the Maybe class.
 */

#pragma once

#include <utility>
#include <type_traits>
#include <memory>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @brief Represents an optional value of type T. A Maybe can either contain a value or be empty.
 */
template<typename T>
class Maybe {
    static_assert(!std::is_reference_v<T>,
        "Maybe<T> cannot be used with reference types");
    static_assert(!std::is_void_v<T>,
        "Maybe<T> cannot be used with void type");
    static_assert(!std::is_array_v<T>,
        "Maybe<T> cannot be used with array types");

public:
    /**
     * @brief Constructs a Maybe with the given value.
     * @param value The value of the Maybe.
     */
    Maybe(const T& value)
        : _hasValue(true) {
        new(&_storage) T(value);
    }
    /**
     * @brief Constructs a Maybe with the given value.
     * @param value The value of the Maybe.
     */
    Maybe(T&& value)
        : _hasValue(true) {
        new(&_storage) T(std::move(value));
    }

    /**
     * @brief Constructs an empty Maybe.
     */
    Maybe() : _hasValue(false) {}

    /**
     * @brief Destroys the Maybe and its resources.
     */
    ~Maybe() {
        destroyValue();
    }

    /// Copy
    Maybe(const Maybe& other)
        : _hasValue(other._hasValue) {
        if (_hasValue)
        {
            new(&_storage) T(other.value());
        }
    }
    /// Copy assign
    Maybe& operator=(const Maybe& other) {
        if (this == &other)
            return *this;

        if (_hasValue && other._hasValue) {
            value() = other.value();
        }
        else if (other._hasValue) {
            new(&_storage) T(other.value());
            _hasValue = true;
        }
        else {
            destroyValue();
        }

        return *this;
    }
    /// Move
    Maybe(Maybe&& other) noexcept(std::is_nothrow_move_constructible_v<T>)
        : _hasValue(other._hasValue) {
        if (_hasValue)
        {
            new(&_storage) T(std::move(other.value()));
        }
    }
    /// Move assign
    Maybe& operator=(Maybe&& other) noexcept(std::is_nothrow_move_assignable_v<T> &&
                                            std::is_nothrow_move_constructible_v<T>) {
        if (this == &other)
            return *this;

        if (_hasValue && other._hasValue) {
            value() = std::move(other.value());
        }
        else if (other._hasValue) {
            new(&_storage) T(std::move(other.value()));
            _hasValue = true;
        }
        else {
            destroyValue();
        }

        return *this;
    }
    
    //
    // Object access/manipulation
    //

    /**
     * @brief Checks if the Maybe has a value.
     * @return True if the Maybe has a value, false otherwise.
     */
    constexpr bool hasValue() const noexcept { return _hasValue; }

    /**
     * @brief Gets the value of the Maybe (read-only).
     * @return The value of the Maybe.
     * @note Asserts if the Maybe has no value, check first using
     * hasValue().
     */
    inline const T& value() const {
        VEE_ASSERT(_hasValue, "Cannot get value from Maybe when it has no value");
        return valueFromStorage();
    }
    /**
     * @brief Gets the value of the Maybe.
     * @return The value of the Maybe.
     * @note Asserts if the Maybe has no value, check first using
     * hasValue().
     */
    inline T& value() {
        VEE_ASSERT(_hasValue, "Cannot get value from Maybe when it has no value");
        return valueFromStorage();
    }

    /**
     * @brief Emplaces a new value in the Maybe, destroying any existing value.
     * @tparam Args The types of the arguments to forward to the constructor of T.
     * @param args The arguments to forward to the constructor of T.
     * @return A reference to the newly emplaced value.
     */
    template<typename... Args>
    T& emplace(Args&&... args) {
        destroyValue();
        new(&_storage) T(std::forward<Args>(args)...);
        _hasValue = true;
        return valueFromStorage();
    }

    //
    // Operators
    //
    
    /**
     * @brief Shorthand for hasValue().
     * @return True if the Maybe has a value, false otherwise.
     */
    explicit operator bool() const { return hasValue(); }

    /**
     * @brief Gets the value of the Maybe (read-only).
     * @return A pointer to the value of the Maybe.
     */
    const T* operator->() const {
        VEE_ASSERT(_hasValue, "Cannot dereference Maybe when it has no value");
        return &valueFromStorage();
    }
    /**
     * @brief Gets the value of the Maybe.
     * @return A pointer to the value of the Maybe.
     */
    T* operator->() {
        VEE_ASSERT(_hasValue, "Cannot dereference Maybe when it has no value");
        return &valueFromStorage();
    }

    /**
     * @brief Gets the value of the Maybe (read-only).
     * @return A reference to the value of the Maybe.
     */
    const T& operator*() const {
        VEE_ASSERT(_hasValue, "Cannot dereference Maybe when it has no value");
        return valueFromStorage();
    }
    /**
     * @brief Gets the value of the Maybe.
     * @return A reference to the value of the Maybe.
     */
    T& operator*() {
        VEE_ASSERT(_hasValue, "Cannot dereference Maybe when it has no value");
        return valueFromStorage();
    }



private:
    std::aligned_storage_t<sizeof(T), alignof(T)> _storage;
    bool _hasValue = false;

    // Internal helpers to get the value from the storage
    inline const T& valueFromStorage() const {
        return *std::launder(reinterpret_cast<const T*>(&_storage));
    }
    inline T& valueFromStorage() {
        return *std::launder(reinterpret_cast<T*>(&_storage));
    }

    // Destroy
    inline void destroyValue() {
        if (_hasValue) {
            std::destroy_at(&valueFromStorage());
            _hasValue = false;
        }
    }
};

} // namespace basic
VEEC_NAMESPACE_END
