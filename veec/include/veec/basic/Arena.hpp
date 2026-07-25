/**
 * @file Arena.hpp
 * @brief This file contains the definition of the Arena class.
 */

#pragma once

#include <vector>
#include <memory>
#include <type_traits>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace basic {

/**
 * @class Arena
 * @brief Block based allocator for fast allocation of objects with similar lifetimes.
 * @tparam BlockSize The size of each block in bytes. Default is 4096 bytes.
 */
template<size_t BlockSize = 4096>
class Arena {
public:
    /**
     * @brief Creates a new Arena instance.
     */
    Arena() = default;
    ~Arena() = default;

    // Disable copy & assign
    Arena(const Arena&) = delete;
    Arena& operator=(const Arena&) = delete;

    /**
     * @brief Allocates memory for an object of type T in the arena.
     * @tparam T The type of the object to allocate.
     * @tparam Args The types of the constructor arguments for T.
     * @param args The constructor arguments for T.
     * @return A pointer to the newly allocated object of type T.
     */
    template<typename T, typename... Args>
    inline T* create(Args&&... args) {
        void* memory = allocate(sizeof(T), alignof(T));
        T* obj = new (memory) T(std::forward<Args>(args)...);

        // Store destructor for non-trivial types
        if constexpr (!std::is_trivially_destructible_v<T>) {
            _destructors.push_back({[](void* obj) { static_cast<T*>(obj)->~T(); }, obj});
        }

        return obj;
    }

    /**
     * @brief Resets the arena, freeing all allocated memory.
     */
    inline void reset() {
        for (auto it = _destructors.rbegin(); it != _destructors.rend(); ++it)
            it->destroy(it->object);

        _destructors.clear();
        _blocks.clear();
    }

private:
    struct Block {
        std::unique_ptr<u8[]> data;
        size_t capacity;
        size_t used;

        explicit Block(size_t size)
            : data(std::make_unique<u8[]>(size)), capacity(size), used(0) {}
    };

    struct Destructor {
        void (*destroy)(void*);
        void* object;
    };

    // Internal alloc
    void* allocate(size_t size, size_t alignment);

    std::vector<Block> _blocks;
    std::vector<Destructor> _destructors;
};

template<size_t BlockSize>
void* Arena<BlockSize>::allocate(size_t size, size_t alignment) {
    if (_blocks.empty()) {
        _blocks.emplace_back(std::max(BlockSize, size));
    }

    Block* block = &_blocks.back();

    uintptr_t current =
        reinterpret_cast<uintptr_t>(block->data.get()) + block->used;

    uintptr_t aligned =
        (current + alignment - 1) & ~(alignment - 1);

    std::size_t offset =
        aligned - reinterpret_cast<uintptr_t>(block->data.get());

    if (offset + size > block->capacity)
    {
        std::size_t newSize = std::max(BlockSize, size);

        _blocks.emplace_back(newSize);
        block = &_blocks.back();

        current = reinterpret_cast<uintptr_t>(block->data.get());
        aligned = (current + alignment - 1) & ~(alignment - 1);
        offset = aligned - current;
    }

    block->used = offset + size;

    return block->data.get() + offset;
}

} // namespace basic
VEEC_NAMESPACE_END
