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
#include <filesystem>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"

VEEC_NAMESPACE_BEGIN
namespace fs {

/**
 * @brief Represents a filesystem path. By default, always normalized to the platform's native
 * path separator.
 */
class Path {
public:
    /**
     * @brief Constructs a new empty Path instance.
     */
    Path() = default;
    /**
     * @brief Constructs a new Path instance from the given std::filesystem::path.
     * @param path The std::filesystem::path representing the path.
     */
    Path(const std::filesystem::path& path)
        : _path(path) {
        normalize();
    }
    /**
     * @brief Constructs a new Path instance from the given std::filesystem::path.
     * @param path The std::filesystem::path representing the path.
     */
    Path(std::filesystem::path&& path)
        : _path(std::move(path)) {
        normalize();
    }
	/**
	 * @brief Constructs a new Path instance from the given c string.
	 * @param path The c string representing the path.
	 */
    Path(const char* path)
        : _path(path) {
        normalize();
    }

    /**
     * @brief Gets the native path separator for the current platform.
     * @return The native path separator character. Usually '/' on Unix-like systems and '\\' on Windows.
     */
    static char getNativePathSeparator();

    /**
     * @brief Checks if the path is empty.
     * @return True if the path is empty, false otherwise.
     */
    inline bool isEmpty() const {
        return _path.empty();
    }

    /**
     * @brief Checks if the path exists in the filesystem.
     * @return True if the path exists, false otherwise.
     */
    bool exists() const;

    /**
     * @brief Checks if the path exists and is a file.
     * @return True if the path exists and is a file, false otherwise.
     */
    bool isFile() const;
    /**
     * @brief Checks if the path exists and is a directory.
     * @return True if the path exists and is a directory, false otherwise.
     */
    bool isDirectory() const;

    //
    // Path manipulation
    //

    /**
     * @brief Gets the parent directory of the path.
     * @return A new Path instance representing the parent directory. Empty
     * if the path has no parent (e.g., root directory).
     */
    fs::Path getParentDirectory() const;
    /**
     * @brief Gets the file name (last component) of the path.
     * @return A string representing the file name. Empty if the path has no file name.
     */
    fs::Path getFileName() const;
	/**
	 * @brief Gets the file stem (file name without extension) of the path.
	 * @return A string representing the file stem. Empty if the path has no file name.
	 */
	fs::Path getFileStem() const;
	/**
	 * @brief Gets the file extension of the path.
	 * @return A string representing the file extension, including the leading dot.
     * Empty if the path has no extension.
	 */
	fs::Path getFileExtension() const;

    //
    // Relativity
    //

	/**
	 * @brief Computes the relative path from the given base path to this path.
	 * @param base The base path to which the relative path is computed.
	 * @return A new Path instance representing the relative path from base to this path.
	 * If the paths are on different root directories, returns an absolute path instead.
	 */
	fs::Path relativeTo(const fs::Path& base) const;
	/**
	 * @brief Converts the path to an absolute path.
	 * @return A new Path instance representing the absolute path.
	 */
    fs::Path absolute() const;

    /**
     * @brief Resolves any symbolic links in the path. Must be valid and exist in the filesystem,
     * otherwise this function will assert.
     */
    void resolveSymbolicLinks();

    /**
     * @brief Gets the underlying std::filesystem::path of this Path.
     * @return The path as a std::filesystem::path.
     */
    const std::filesystem::path& filesystemPath() const {
        return _path;
    }

    /**
     * @brief Gets this Path as a string.
     * @return The path as a string.
     */
    std::string toString() const {
        return _path.generic_string();
    }

    //
    // Operators
    //

	inline Path operator/(const Path& other) const {
		return Path(_path / other._path);
	}
    inline Path operator/(std::string_view other) const {
        return Path(_path / other);
    }

private:
    std::filesystem::path _path;

    void normalize();
};

} // namespace fs
VEEC_NAMESPACE_END

// HASH + EQUALS
namespace std {
    template<>
    struct hash<veec::fs::Path> {
        std::size_t operator()(const veec::fs::Path& path) const noexcept {
            return std::hash<std::string>()(path.toString());
        }
    };

    template<>
    struct equal_to<veec::fs::Path> {
        bool operator()(const veec::fs::Path& lhs, const veec::fs::Path& rhs) const noexcept {
            return lhs.toString() == rhs.toString();
        }
    };
}
