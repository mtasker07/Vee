/**
 * @file Path.hpp
 * @brief This file contains the definition of the Path class.
 * 
 * Prefer using this class in the compiler over std::string or std::filesystem::path
 * as it provides a much more consistent interface for working with paths in the compiler.
 */

#pragma once

#include <string>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace fs {

class Path {
public:
    /**
     * @brief The type used to represent the path as a string.
     */
    using StringType = std::string;
    /**
     * @brief The type used to represent the path as a string view.
     */
    using StringViewType = std::string_view;

    /**
     * @brief Constructs a new empty Path instance.
     */
    Path() = default;
    /**
     * @brief Constructs a new Path instance from the given string.
     * @param path The string representing the path.
     */
    explicit Path(const StringType& path) : _path(path) {}
    /**
     * @brief Constructs a new Path instance from the given string.
     * @param path The string representing the path.
     */
    explicit Path(StringType&& path) : _path(std::move(path)) {}
    /**
     * @brief Constructs a new Path instance from the given string view.
     * @param path The string view representing the path.
     */
    explicit Path(StringViewType path) : _path(path) {}

    StringViewType str() const {
        return _path;
    }

private:
    StringType _path;
};

} // namespace fs
VEEC_NAMESPACE_END

// HASH + EQUALS
namespace std {
    template<>
    struct hash<VEEC_NAMESPACE::fs::Path> {
        std::size_t operator()(const VEEC_NAMESPACE::fs::Path& path) const noexcept {
            return std::hash<VEEC_NAMESPACE::fs::Path::StringViewType>()(path.str());
        }
    };

    template<>
    struct equal_to<VEEC_NAMESPACE::fs::Path> {
        bool operator()(const VEEC_NAMESPACE::fs::Path& lhs, const VEEC_NAMESPACE::fs::Path& rhs) const noexcept {
            return lhs.str() == rhs.str();
        }
    };
}
