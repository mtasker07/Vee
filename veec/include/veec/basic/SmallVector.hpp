/**
 * @file SmallVector.hpp
 * @brief This file contains the definition of the SmallVector class.
 * 
 * The SmallVector is an std::vector like container that is optimized for small sizes of objects.
 * By storing `N` elements inline, it avoids unnecessary heap allocations for relatively few elements,
 * while still allowing for dynamic growth when the number of elements exceeds `N`.
 * 
 */

#pragma once

#include <utility>
#include <type_traits>
#include <memory>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @brief Represents a small vector of type `T`. A SmallVector can store a small number of elements
 * inline and dynamically allocate memory for larger sizes, making it more efficient for small sizes.
 * @tparam T The type of elements stored in the SmallVector.
 * @tparam N The number of elements that can be stored inline (default 4).
 * @tparam Allocator The allocator used for allocating internal objects (default std::allocator<T>).
 */
template<typename T, size_t N = 4, class Allocator = std::allocator<T>>
class SmallVector {
    static_assert(!std::is_void_v<T>,
        "SmallVector<T> cannot be used with void type");
    static_assert(std::is_move_constructible_v<T>,
        "SmallVector<T> requires move-constructible types");
    static_assert(N > 0,
        "SmallVector<T> inline capacity must be greater than zero");

public:
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    /**
     * @brief Constructs an empty SmallVector.
     */
    SmallVector() = default;
    /**
     * @brief Constructs a SmallVector with the given capacity. Does not initialise any elements.
     * Equivalent to calling `reserve(capacity)`.
     * @param capacity The initial capacity of the SmallVector.
     */
    SmallVector(size_t capacity) {
        if (capacity > InlineCapacity)
            allocateHeap(capacity);
    }
    /**
     * @brief Constructs a SmallVector with the given size and default value for each element.
     * @param size The number of elements in the SmallVector.
     * @param value The default value for each element.
     */
    SmallVector(size_t size, const T& value) {
        if (size > InlineCapacity)
            allocateHeap(size);

        for (size_t i = 0; i < size; ++i)
            AllocatorTraits::construct(_allocator, &_data[i], value);
        _size = size;
    }
    /**
     * @brief Constructs a SmallVector from an std::initializer_list.
     * @param init The initializer list to copy elements from.
     */
    SmallVector(std::initializer_list<T> init) {
        if (init.size() > InlineCapacity)
            allocateHeap(init.size());

        for (const auto& value : init) {
            AllocatorTraits::construct(_allocator, &_data[_size], value);
            ++_size;
        }
    }
    /**
     * @brief Creates a new SmallVector by copying all elements from a given std::vector.
     * @param vec The std::vector to copy elements from.
     */
    SmallVector(const std::vector<T, Allocator>& vec) {
        if (vec.size() > InlineCapacity)
            allocateHeap(vec.size());

        for (const T& value : vec) {
            AllocatorTraits::construct(_allocator, &_data[_size], value);
            ++_size;
        }
    }


    /**
     * @brief Destroys the SmallVector, deallocating any heap memory and destroying all elements.
     */
    ~SmallVector() {
        destroyAll();
        deallocateHeap();
    }

    //
    // Move/Copy
    //

    SmallVector(const SmallVector& other) {
        copyFrom(other);
    }
    SmallVector& operator=(const SmallVector& other) {
        if (this != &other)
            copyFrom(other);
        return *this;
    }

    SmallVector(SmallVector&& other) noexcept(std::is_nothrow_move_constructible_v<T>) {
        moveFrom(std::move(other));
    }
    SmallVector& operator=(SmallVector&& other) noexcept(std::is_nothrow_move_assignable_v<T>) {
        if (this != &other)
            moveFrom(std::move(other));
        return *this;
    }

    //
    // Element Manipulation
    //

    /**
     * @brief Appends an element to the end of the vector.
     * @param value The value to append.
     */
    inline void push_back(const T& value) {
        tryGrow();
        AllocatorTraits::construct(_allocator, &_data[_size], value);
        ++_size;
    }
    /**
     * @brief Appends an element to the end of the vector.
     * @param value The value to append.
     */
    inline void push_back(T&& value) {
        tryGrow();
        AllocatorTraits::construct(_allocator, &_data[_size], std::move(value));
        ++_size;
    }
    /**
     * @brief Removes the last element from the vector and returns it.
     * If the vector is empty, this function will assert.
     * @return The last element of the vector.
     */
    inline T pop_back() {
        VEE_ASSERT(_size > 0, "pop_back() called on empty SmallVector");
        --_size;
        T value = std::move(_data[_size]);
        _data[_size].~T();
        return value;
    }

    /**
     * @brief Constructs a new element in place at the specified position.
     * @param it The position at which to construct the new element.
     */
    template<typename... Args>
    inline void emplace(iterator it, Args&&... args) {
        VEE_ASSERT(it >= begin() && it <= end(), "emplace() iterator out of bounds in SmallVector");
        size_t index = it - begin();
        tryGrow();
        if (index < _size) {
            // Move elements to the right
            for (size_t i = _size; i > index; --i) {
                AllocatorTraits::construct(_allocator, &_data[i], std::move(_data[i - 1]));
                _data[i - 1].~T();
            }
        }
        AllocatorTraits::construct(_allocator, &_data[index], std::forward<Args>(args)...);
        ++_size;
    }
    /**
     * @brief Constructs a new element in place at the end of the vector.
     * @tparam Args The types of the arguments to forward to the constructor of T.
     * @param args The arguments to forward to the constructor of T.
     */
    template<typename... Args>
    inline void emplace_back(Args&&... args) {
        tryGrow();
        AllocatorTraits::construct(_allocator, &_data[_size], std::forward<Args>(args)...);
        ++_size;
    }

    /**
     * @brief Clears the vector, destroying all elements and setting the size to 0.
     */
    inline void clear() {
        destroyAll();
        _size = 0;
    }

    //
    // Accessors
    //

    /**
     * @brief Gets the element at the specified index (read-only).
     * @param index The index of the element to retrieve.
     */
    inline const T& operator[](size_t index) const {
        VEE_ASSERT(index < _size, "Index out of bounds in SmallVector");
        return _data[index];
    }
    /**
     * @brief Gets the element at the specified index.
     * @param index The index of the element to retrieve.
     */
    inline T& operator[](size_t index) {
        VEE_ASSERT(index < _size, "Index out of bounds in SmallVector");
        return _data[index];
    }

    /**
     * @brief Gets the last element of the vector (read-only).
     * @return The last element of the vector.
     */
    inline const T& back() const {
        VEE_ASSERT(_size > 0, "back() called on empty SmallVector");
        return _data[_size - 1];
    }
    /**
     * @brief Gets the last element of the vector.
     * @return The last element of the vector.
     */
    inline T& back() {
        VEE_ASSERT(_size > 0, "back() called on empty SmallVector");
        return _data[_size - 1];
    }
    /**
     * @brief Gets the first element of the vector (read-only).
     * @return The first element of the vector.
     */
    inline const T& front() const {
        VEE_ASSERT(_size > 0, "front() called on empty SmallVector");
        return _data[0];
    }
    /**
     * @brief Gets the first element of the vector.
     * @return The first element of the vector.
     */
    inline T& front() {
        VEE_ASSERT(_size > 0, "front() called on empty SmallVector");
        return _data[0];
    }

    //
    // Iterators
    // 

    /**
     * @brief Gets an iterator to the beginning of the vector.
     * @return An iterator to the beginning of the vector.
     */
    iterator begin() { return _data; }
    /**
     * @brief Gets an iterator to the end of the vector.
     * @return An iterator to the end of the vector.
     */
    iterator end() { return _data + _size; }
    /**
     * @brief Gets a const iterator to the beginning of the vector.
     * @return A const iterator to the beginning of the vector.
     */
    const_iterator begin() const { return _data; }
    /**
     * @brief Gets a const iterator to the end of the vector.
     * @return A const iterator to the end of the vector.
     */
    const_iterator end() const { return _data + _size; }
    /**
     * @brief Gets a const iterator to the beginning of the vector.
     * @return A const iterator to the beginning of the vector.
     */
    const_iterator cbegin() const { return _data; }
    /**
     * @brief Gets a const iterator to the end of the vector.
     * @return A const iterator to the end of the vector.
     */
    const_iterator cend() const { return _data + _size; }
    /**
     * @brief Gets a reverse iterator to the beginning of the vector.
     * @return A reverse iterator to the beginning of the vector.
     */
    reverse_iterator rbegin() { return reverse_iterator(end()); }
    /**
     * @brief Gets a reverse iterator to the end of the vector.
     * @return A reverse iterator to the end of the vector.
     */
    reverse_iterator rend() { return reverse_iterator(begin()); }
    /**
     * @brief Gets a const reverse iterator to the beginning of the vector.
     * @return A const reverse iterator to the beginning of the vector.
     */
    const_reverse_iterator rbegin() const { return const_reverse_iterator(end()); }
    /**
     * @brief Gets a const reverse iterator to the end of the vector.
     * @return A const reverse iterator to the end of the vector.
     */
    const_reverse_iterator rend() const { return const_reverse_iterator(begin()); }
    /**
     * @brief Gets a const reverse iterator to the beginning of the vector.
     * @return A const reverse iterator to the beginning of the vector.
     */
    const_reverse_iterator crbegin() const { return const_reverse_iterator(cend()); }
    /**
     * @brief Gets a const reverse iterator to the end of the vector.
     * @return A const reverse iterator to the end of the vector.
     */
    const_reverse_iterator crend() const { return const_reverse_iterator(cbegin()); }

    //
    // Size/Capacity
    //

    /**
     * @brief Checks if the vector is empty.
     * @return True if the vector is empty, false otherwise.
     */
    inline bool empty() const { return _size == 0; }
    /**
     * @brief Gets the size of the vector.
     * @return The number of elements in the vector.
     */
    inline size_t size() const { return _size; }
    /**
     * @brief Gets the capacity of the vector.
     * @return The number of elements that can be stored in the vector before requiring reallocation.
     */
    inline size_t capacity() const { return _capacity; }
    /**
     * @brief Gets the inline capacity of the vector.
     * @return The number of elements that can be stored inline before requiring dynamic memory
     * allocation.
     */
    inline constexpr size_t inline_capacity() const { return N; }

    /**
     * @brief Reserves space for at least `newCapacity` elements. If `newCapacity` is less than or equal
     * to the current capacity, this function does nothing.
     * @param newCapacity The minimum capacity to reserve.
     */
    inline void reserve(size_t newCapacity) {
        if (newCapacity <= _capacity || newCapacity <= InlineCapacity)
            return;

        reallocateHeap(newCapacity);
        _capacity = newCapacity;
    }
    /**
     * @brief Resizes the vector to contain `newSize` elements. If `newSize` is less than the current
     * size, excess elements are destroyed. If `newSize` is greater than the current size, new elements
     * are default-constructed.
     * @param newSize The new size of the vector.
     */
    inline void resize(size_t newSize) {
        if (newSize < _size) {
            // Destroy excess elements
            for (size_t i = newSize; i < _size; ++i)
                AllocatorTraits::destroy(_allocator, &_data[i]);
        } else if (newSize > _size) {
            // Grow if necessary
            if (newSize > _capacity)
                reallocateHeap(newSize);

            // Construct new elements
            for (size_t i = _size; i < newSize; ++i)
                AllocatorTraits::construct(_allocator, &_data[i]);
        }
        _size = newSize;
    }
    /**
     * @brief Shrinks the capacity of the vector to fit its size. If the size is less than or equal to
     * the inline capacity, the vector will move its elements to inline storage. If the size
     * is less than the current capacity, the vector will shrink its capacity to match the size.
     */
    inline void shrink_to_fit() {
        if (_size <= InlineCapacity)
            moveToInline();
        else if (_size < _capacity)
            reallocateHeap(_size);
    }

    //
    // Underlying Data Usage
    //

    /**
     * @brief Gets a pointer to the underlying data of the vector (read-only).
     * @return A pointer to the underlying data of the vector.
     */
    inline const T* data() const {
        return _data;
    }
    /**
     * @brief Gets a pointer to the underlying data of the vector.
     * @return A pointer to the underlying data of the vector.
     */
    inline T* data() {
        return _data;
    }
    /**
     * @brief Gets the inline storage of the vector (read-only).
     * @return A pointer to the inline storage of the vector.
     */
    inline const T* inline_data() const {
        return std::launder(reinterpret_cast<const T*>(_inlineStorage));
    }
    /**
     * @brief Gets the inline storage of the vector.
     * @return A pointer to the inline storage of the vector.
     */
    inline T* inline_data() {
        return std::launder(reinterpret_cast<T*>(_inlineStorage));
    }
    /**
     * @brief Checks if the vector is using its inline storage (capacity < N).
     * @return True if the vector is using its inline storage, false otherwise.
     */
    inline bool is_inline() const {
        return _data == inline_data();
    }
    
private:
    using AllocatorTraits = std::allocator_traits<Allocator>;

    static constexpr size_t InlineCapacity = N;

    Allocator _allocator;
    size_t _size = 0;
    size_t _capacity = InlineCapacity;
    alignas(T) std::byte _inlineStorage[sizeof(T) * N];
    T* _data = std::launder(reinterpret_cast<T*>(_inlineStorage));

    // grow() if _size == _capacity
    inline void tryGrow() {
        if (_size == _capacity)
            grow();
    }
    inline void grow() {
        size_t newCapacity = _capacity > 0 ? _capacity * 2 : 1;
        reallocateHeap(newCapacity);
    }

    // Deallocate heap memory
    inline void deallocateHeap() {
        if (is_inline())
            return;

        AllocatorTraits::deallocate(
            _allocator,
            _data,
            _capacity
        );
    }
    // Allocate heap memory for newCapacity elements and set _data to point to it
    inline void allocateHeap(size_t newCapacity) {
        _data = AllocatorTraits::allocate(_allocator, newCapacity);
        _capacity = newCapacity;
    }
    // Allocate heap memory for newCapacity elements, move existing elements
    // into it, and deallocate the old memory
    inline void reallocateHeap(size_t newCapacity) {
        T* newData = AllocatorTraits::allocate(_allocator, newCapacity);

        moveConstructRange(_data, newData, _size);
        destroyRange(_data, _size);
        deallocateHeap();

        _data = newData;
        _capacity = newCapacity;
    }
    // Move elements to inline storage and deallocate heap memory
    inline void moveToInline() {
        if (is_inline())
            return;

        T* newData = inline_data();

        moveConstructRange(_data, newData, _size);
        destroyRange(_data, _size);
        deallocateHeap();

        _data = newData;
        _capacity = InlineCapacity;
    }

    // Construct `count` elements from `src` into `dest` using move semantics
    inline void moveConstructRange(T* src, T* dest, size_t count) noexcept {
        for (size_t i = 0; i < count; ++i)
            AllocatorTraits::construct(_allocator, &dest[i], std::move(src[i]));
    }
    // Construct `count` elements from `src` into `dest` using copy semantics
    inline void copyConstructRange(T* src, T* dest, size_t count) noexcept {
        for (size_t i = 0; i < count; ++i)
            AllocatorTraits::construct(_allocator, &dest[i], src[i]);
    }
    // Destroy `count` elements starting from `ptr`
    inline void destroyRange(T* ptr, size_t count) noexcept {
        for (size_t i = 0; i < count; ++i)
            AllocatorTraits::destroy(_allocator, &ptr[i]);
    }

    // Destroy all elements without resetting size or deallocating memory
    inline void destroyAll() noexcept {
        destroyRange(_data, _size);
    }

    // Copy/move helpers
    inline void copyFrom(const SmallVector& other);
    inline void moveFrom(SmallVector&& other);
};

template<typename T, size_t N, class Allocator>
inline void SmallVector<T, N, Allocator>::copyFrom(const SmallVector& other) {
    destroyRange(_data, _size);

    if (other._size > _capacity) {
        deallocateHeap();
        allocateHeap(other._capacity);
    }

    copyConstructRange(other._data, _data, other._size);
    _size = other._size;
}
template<typename T, size_t N, class Allocator>
inline void SmallVector<T, N, Allocator>::moveFrom(SmallVector&& other) {
    // O(N) if other is inline, O(1) if other is heap allocated
    if (other.is_inline()) {
        if (other._size > _capacity) {
            deallocateHeap();
            allocateHeap(other._capacity);
        }

        moveConstructRange(other._data, _data, other._size);
        destroyRange(other._data, other._size);
    }
    else {
        destroyRange(_data, _size);
        deallocateHeap();

        _data = other._data;
        _capacity = other._capacity;
    }
    _size = other._size;

    other._data = other.inline_data();
    other._capacity = InlineCapacity;
    other._size = 0;
}

} // namespace basic
VEEC_NAMESPACE_END
