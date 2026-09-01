#include "veec/mir/support/TypeTable.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirType.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace support {

const MirVoidType* TypeTable::getVoid() {
    if (!_voidType) {
        _voidType = new (_typeArena.allocate<MirVoidType>()) MirVoidType();
        _types.push_back(_voidType);
    }
    return _voidType;
}
const MirBoolType* TypeTable::getBool() {
    if (!_boolType) {
        _boolType = new (_typeArena.allocate<MirBoolType>()) MirBoolType();
        _types.push_back(_boolType);
    }
    return _boolType;
}

const MirIntegerType* TypeTable::getInteger(u32 bitWidth, bool isSigned) {
    auto it = _integers.find(bitWidth);
    if (it != _integers.end()) {
        if (isSigned && it->second.second) {
            return it->second.second;
        } else if (it->second.first) {
            return it->second.first;
        }
    }

    MirIntegerType* intType = new (_typeArena.allocate<MirIntegerType>()) MirIntegerType(bitWidth, isSigned);
    if (isSigned) {
        _integers[bitWidth].second = intType;
    } else {
        _integers[bitWidth].first = intType;
    }
    _types.push_back(intType);
    return intType;
}
const MirFloatType* TypeTable::getFloat(u32 bitWidth) {
    auto it = _floats.find(bitWidth);
    if (it != _floats.end()) {
        return it->second;
    }

    MirFloatType* floatType = new (_typeArena.allocate<MirFloatType>()) MirFloatType(bitWidth);
    _floats[bitWidth] = floatType;
    _types.push_back(floatType);
    return floatType;
}

const MirPointerType* TypeTable::getPointer(const MirType* pointeeType) {
    auto it = _pointers.find(pointeeType);
    if (it != _pointers.end()) {
        return it->second;
    }

    MirPointerType* ptrType = new (_typeArena.allocate<MirPointerType>()) MirPointerType(pointeeType);
    _pointers[pointeeType] = ptrType;
    _types.push_back(ptrType);
    return ptrType;
}
const MirArrayType* TypeTable::getArray(const MirType* elementType, u64 size) {
    auto it = _arrays.find(elementType);
    if (it != _arrays.end()) {
        // We dont hash size, so just a simple linear search for now,
        // may improve this later...
        for (const auto& pair : it->second) {
            if (pair.first == size) {
                return pair.second;
            }
        }
    }

    MirArrayType* arrayType = new (_typeArena.allocate<MirArrayType>()) MirArrayType(elementType, size);
    _arrays[elementType].emplace_back(size, arrayType);
    _types.push_back(arrayType);
    return arrayType;
}

const MirFunctionType* TypeTable::getFunction(const MirType* returnType, const basic::SmallVector<const MirType*>& paramTypes) {
    auto it = _functions.find(returnType);
    if (it != _functions.end()) {
        // Same as arrays, we dont hash paramTypes, so just a simple linear search for now
        for (const auto& pair : it->second) {
            if (pair.first == paramTypes) {
                return pair.second;
            }
        }
    }

    MirFunctionType* funcType = new (_typeArena.allocate<MirFunctionType>()) MirFunctionType(returnType, paramTypes);
    _functions[returnType].emplace_back(paramTypes, funcType);
    _types.push_back(funcType);
    return funcType;
}

MirStructType* TypeTable::createOpaqueStruct() {
    MirStructType* structType = new (_typeArena.allocate<MirStructType>()) MirStructType(getNextStructId());
    _structs.push_back(structType);
    _types.push_back(structType);
    return structType;
}
MirStructType* TypeTable::createStruct(const basic::SmallVector<const MirType*>& memberTypes) {
    MirStructType* structType = new (_typeArena.allocate<MirStructType>()) MirStructType(getNextStructId(), memberTypes);
    _structs.push_back(structType);
    _types.push_back(structType);
    return structType;
}

const MirStructType* TypeTable::getStringStruct() {
    if (!_stringStruct) {
        // String struct:
        // - data: u8*
        // - length: u64

        const basic::SmallVector<const MirType*> memberTypes = {
            getPointer(getInteger(8, false)), // u8*
            getInteger(64, false)             // u64
        };
        
        _stringStruct = new (_typeArena.allocate<MirStructType>()) MirStructType(getNextStructId(), memberTypes);
        _structs.push_back(_stringStruct);
        _types.push_back(_stringStruct);
    }
    return _stringStruct;
}

u32 TypeTable::getNextStructId() {
    return static_cast<u32>(_structs.size());
}

} // namespace support
} // namespace mir
VEEC_NAMESPACE_END
