/**
 * @file ClassType.hpp
 * @brief This file contains the definition of the class type class.
 *
 * The class type class represents class types defined by the user.
 */

#pragma once

#include <string>
#include <format>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/FieldSymbol.hpp"
#include "veec/types/TypeId.hpp"
#include "veec/types/TypeKind.hpp"
#include "veec/types/Type.hpp"

VEEC_NAMESPACE_BEGIN
namespace types {

/**
 * @brief Represents a class type in the Vee language.
 */
class ClassType : public Type {
public:
    /**
     * @brief Creates a class type with the specified name ID.
     * @param nameId The name ID of the class type.
     */
    ClassType(symbols::ClassSymbol* classSymbol)
        : Type(TypeKind::Class), _classSymbol(classSymbol) {}

    virtual ~ClassType() = default;

    /**
     * @brief Converts this class type to a string representation.
     * @return A string representation of this class type.
     */
    std::string toString() const override {
        return std::format("class {}", _classSymbol->getNameValue());
    }

    /**
     * @brief Gets the name ID of this class type.
     * @return The name ID of this class type.
     */
    inline symbols::ClassSymbol* getClassSymbol() const { return _classSymbol; }

    /**
     * @brief Gets the fields of this class type.
     * @return A map of (name -> field)s in this class type.
     */
    inline const std::unordered_map<basic::StringId, symbols::FieldSymbol*>& getFields() const {
        return _fields;
    }
    /**
     * @brief Gets the methods of this class type.
     * @return A map of (name -> methods)s in this class type.
     */
    inline const std::unordered_map<basic::StringId, std::vector<symbols::FunctionSymbol*>>& getMethods() const {
        return _methods;
    }

    /**
     * @brief Gets the field with the specified name ID.
     * @param nameId The name ID of the field to get.
     * @return A pointer to the field symbol with the specified name ID, or nullptr if no such field exists
     * in this class type.
     */
    inline const symbols::FieldSymbol* getField(basic::StringId nameId) const {
        auto it = _fields.find(nameId);
        if (it != _fields.end()) {
            return it->second;
        }
        return nullptr;
    }
    /**
     * @brief Gets the methods with the specified name ID.
     * @param nameId The name ID of the methods to get.
     * @return A pointer to the vector of method symbols with the specified name ID, or nullptr if no such methods exist
     * in this class type.
     */
    inline const std::vector<symbols::FunctionSymbol*>* getMethods(basic::StringId nameId) const {
        auto it = _methods.find(nameId);
        if (it != _methods.end()) {
            return &it->second;
        }
        return nullptr;
    }

    /**
     * @brief Adds a field to this class type.
     * @param field The field symbol to add to this class type.
     */
    inline void addField(symbols::FieldSymbol* field) {
        VEE_ASSERT(field != nullptr, "Cannot add a null field symbol to class type");
        VEE_ASSERT(_fields.find(field->getNameValue()) == _fields.end(), "Field with the same name already exists in class type");
        _fields[field->getNameValue()] = field;
    }
    /**
     * @brief Adds a method to this class type.
     * @param method The method symbol to add to this class type.
     */
    inline void addMethod(symbols::FunctionSymbol* method) {
        VEE_ASSERT(method != nullptr, "Cannot add a null method symbol to class type");
        _methods[method->getNameValue()].push_back(method);
    }

    /**
     * @brief Gets the static kind of this type, which is Class.
     * @return The static kind of this type, which is Class.
     */
    static TypeKind getStaticKind() { return TypeKind::Class; }

private:
    symbols::ClassSymbol* _classSymbol;
    std::unordered_map<basic::StringId, symbols::FieldSymbol*> _fields;
    std::unordered_map<basic::StringId, std::vector<symbols::FunctionSymbol*>> _methods;
};

} // namespace types
VEEC_NAMESPACE_END
