#include "veec/mir/support/ConstantTable.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/Constant.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace support {

ConstantInt* ConstantTable::getInt(types::Type* type, basic::APInt value) {
    ConstantInt::KeyType key{type, value};
    auto it = _integers.find(key);
    return it != _integers.end() ? it->second : nullptr;
}
ConstantFloat* ConstantTable::getFloat(types::Type* type, double value) {
    ConstantFloat::KeyType key{type, value};
    auto it = _floats.find(key);
    return it != _floats.end() ? it->second : nullptr;
}
ConstantString* ConstantTable::getString(types::Type* type, std::string_view value) {
    ConstantString::KeyType key{type, value};
    auto it = _strings.find(key);
    return it != _strings.end() ? it->second : nullptr;
}
ConstantBool* ConstantTable::getBool(bool value) {
    return value ? _trueConstant : _falseConstant;
}

void ConstantTable::intern(types::Type* type, ConstantInt* constant) {
    ConstantInt::KeyType key{type, constant->getValue()};
    VEE_ASSERT(_integers.find(key) == _integers.end(), "Integer constant already interned");
    _integers[key] = constant;
    _constants.push_back(constant);
}
void ConstantTable::intern(types::Type* type, ConstantFloat* constant) {
    ConstantFloat::KeyType key{type, constant->getValue()};
    VEE_ASSERT(_floats.find(key) == _floats.end(), "Float constant already interned");
    _floats[key] = constant;
    _constants.push_back(constant);
}
void ConstantTable::intern(types::Type* type, ConstantString* constant) {
    ConstantString::KeyType key{type, constant->getValue()};
    VEE_ASSERT(_strings.find(key) == _strings.end(), "String constant already interned");
    _strings[key] = constant;
    _constants.push_back(constant);
}
void ConstantTable::intern(ConstantBool* constant) {
    if (constant->getValue()) {
        VEE_ASSERT(_trueConstant == nullptr, "True constant already interned");
        _trueConstant = constant;
    } else {
        VEE_ASSERT(_falseConstant == nullptr, "False constant already interned");
        _falseConstant = constant;
    }
    _constants.push_back(constant);
}

} // namespace support
} // namespace mir
VEEC_NAMESPACE_END
