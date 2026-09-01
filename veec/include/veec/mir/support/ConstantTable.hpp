/**
 * @file ConstantTable.hpp
 * @brief This file contains the definition of the ConstantTable class.
 * 
 * The ConstantTable manages constants within a module, and ensures that each unique constant
 * is only created once. It provides methods to retrieve existing constants or create new ones
 * if they do not already exist.
 * 
 * Note that the ConstantTable does not manage lifetime or creation of constants. That is the
 * responsibility of the MIR factory. It is for this reason that get() methods
 * return nullptr if not found, instead of creating a new object like the TypeTable does.
 * Constants must be interned manually into the table after creation.
 */

#pragma once

#include <vector>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/SmallVector.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Constant.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace support {

/**
 * @class ConstantTable
 * @brief Represents a table of constants in the MIR.
 */
class ConstantTable {
public:
    ConstantTable() = default;
    ~ConstantTable() = default;

    /**
     * @brief Gets the integer constant with the specified type and value.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @return A pointer to the constant if found, nullptr otherwise.
     */
    ConstantInt* getInt(const MirType* type, basic::APInt value);
    /**
     * @brief Gets the float constant with the specified type and value.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @return A pointer to the constant if found, nullptr otherwise.
     */
    ConstantFloat* getFloat(const MirType* type, double value);
    /**
     * @brief Gets the string constant with the specified type and value.
     * @param type The type of the constant.
     * @param value The value of the constant.
     * @return A pointer to the constant if found, nullptr otherwise.
     */
    ConstantString* getString(const MirType* type, std::string_view value);
    /**
     * @brief Gets the boolean constant with the specified value.
     * @param value The value of the constant.
     * @return A pointer to the constant if found, nullptr otherwise.
     */
    ConstantBool* getBool(bool value);

    /**
     * @brief Interns the given integer constant into the table.
     * @param type The type of the constant.
     * @param constant The constant to intern.
     */
    void intern(const MirType* type, ConstantInt* constant);
    /**
     * @brief Interns the given float constant into the table.
     * @param type The type of the constant.
     * @param constant The constant to intern.
     */
    void intern(const MirType* type, ConstantFloat* constant);
    /**
     * @brief Interns the given string constant into the table.
     * @param type The type of the constant.
     * @param constant The constant to intern.
     */
    void intern(const MirType* type, ConstantString* constant);
    /**
     * @brief Interns the given boolean constant into the table.
     * @param constant The constant to intern.
     */
    void intern(ConstantBool* constant);

    /**
     * @brief Gets all constants stored within this table.
     * @return A list of all constants stored within this table.
     */
    inline const std::vector<Constant*>& getAllConstants() const {
        return _constants;
    }

private:
    std::vector<Constant*> _constants;
    std::unordered_map<ConstantInt::KeyType, ConstantInt*, ConstantInt::Hasher> _integers;
    std::unordered_map<ConstantFloat::KeyType, ConstantFloat*, ConstantFloat::Hasher> _floats;
    std::unordered_map<ConstantString::KeyType, ConstantString*, ConstantString::Hasher> _strings;
    // Dont bother with a map for bools, we only have 2 values
    ConstantBool* _trueConstant = nullptr;
    ConstantBool* _falseConstant = nullptr;
};

} // namespace support
} // namespace mir
VEEC_NAMESPACE_END
