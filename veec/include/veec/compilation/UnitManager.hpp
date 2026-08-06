/**
 * @file UnitManager.hpp
 * @brief This file contains the definition of the UnitManager class,
 * which is used to manage translation units.
 */

#pragma once

#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/TranslationUnit.hpp"
#include "veec/source/SourceFileId.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

/**
 * @class UnitManager
 * @brief Used to manage translation units.
 */
class UnitManager {
public:
    UnitManager() = default;
    ~UnitManager() = default;

    /**
     * @brief Gets the translation unit for a given source file ID.
     * @param fileId The source file ID of the translation unit to retrieve.
     * @return A pointer to the translation unit, or nullptr if it does not exist.
     */
    inline TranslationUnit* getUnit(source::SourceFileId fileId) {
        auto it = _units.find(fileId);
        if (it != _units.end()) {
            return &it->second;
        }
        return nullptr;
    }
    /**
     * @brief Creates a new translation unit for a given source file ID.
     * @param fileId The source file ID of the translation unit to create.
     * @return A pointer to the newly created translation unit.
     */
    inline TranslationUnit* createUnit(source::SourceFileId fileId) {
        auto [it, inserted] = _units.emplace(fileId, TranslationUnit{});
        if (inserted) {
            it->second.fileId = fileId;
        }
        return &it->second;
    }
    
    /**
     * @brief Gets all translation units in this manager.
     * @return A list of all translation units in this manager.
     */
    inline std::vector<TranslationUnit*> getAllUnits() {
        std::vector<TranslationUnit*> units;
        units.reserve(_units.size());
        for (auto& [fileId, unit] : _units) {
            units.push_back(&unit);
        }
        return units;
    }

private:
    std::unordered_map<source::SourceFileId, TranslationUnit> _units;
};

} // namespace compilation
VEEC_NAMESPACE_END
