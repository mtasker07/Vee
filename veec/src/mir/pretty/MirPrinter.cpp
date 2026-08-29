#include "veec/mir/pretty/MirPrinter.hpp"

#include <format>
#include <string>
#include <string_view>
#include <sstream>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/BigInt.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/io/IWriter.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/mir/MirContext.hpp"
#include "veec/mir/MirKind.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirNode.hpp"
#include "veec/mir/Module.hpp"
#include "veec/mir/Function.hpp"
#include "veec/mir/BasicBlock.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/mir/Value.hpp"
#include "veec/mir/Constant.hpp"
#include "veec/mir/pretty/ValueNameMap.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/types/Type.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace pretty {

void MirPrinter::printNode(const MirNode& node, io::IWriter& writer) {
    _oss.str("");
    _oss.clear();
    _indent = 0;

    switch (node.getNodeKind()) {
        case MirKind::Module:
            printModule(static_cast<const Module&>(node));
            break;
        case MirKind::Function:
            printFunction(static_cast<const Function&>(node));
            break;
        case MirKind::BasicBlock:
            printBasicBlock(static_cast<const BasicBlock&>(node));
            break;
        case MirKind::Instruction:
            printInstruction(static_cast<const Instruction&>(node));
            break;
        case MirKind::Local:
        case MirKind::Constant:
            printOperand(static_cast<const Value&>(node));
            break;
        default:
            VEE_UNREACHABLE("Unknown MIR node kind");
    }
    writer.write(_oss.str());
}

void MirPrinter::printModule(const Module& module) {
    _funcCounter = 1;
    _unnamedFunctionNames.clear();

    addLineIndented("[Module] {\n");
    for (const Constant* constant : module.getConstants()) {
        addLineIndented(printConstant(*constant));
    }
    append("\n");
    for (const Function* func : module.getFunctions()) {
        printFunction(*func);
        addLineIndented("");
    }
    // Account for newline after last function
    addLineIndented("}");
}
void MirPrinter::printFunction(const Function& function) {
    // Values are unique to functions, so reset the counter here
    _valueCounter = 1;
    _unnamedValueNames.clear();

    // Print signature
    std::string_view funcName = nameOfFunction(function);
    std::string args;
    for (size_t i = 0; i < function.getParameterCount(); ++i) {
        const Local& param = *function.getParameter(i);
        if (i > 0) args += ", ";
        std::string paramTypeName = typeOfValue(param)->toString();
        std::string_view paramName = nameOfValue(param);
        args += std::format("{} {}", paramTypeName, paramName);
    }
    std::string returnTypeName = typeOfValue(function)->toString();

    // Print body
    addLineIndented(std::format("func {}({}) -> {} {{", funcName, args, returnTypeName));
    _indent++;
    for (const BasicBlock* block : function.getBlocks()) {
        printBasicBlock(*block);
    }
    _indent--;
    addLineIndented("}");
}
void MirPrinter::printBasicBlock(const BasicBlock& block) {
    addLineIndented(std::format("{}:", nameOfValue(block)));
    _indent++;
    for (const Instruction* instr : block.getInstructions()) {
        printInstruction(*instr);
    }
    _indent--;
}
void MirPrinter::printInstruction(const Instruction& instruction) {
    std::ostringstream line;

    // Print result value if instruction produces one
    if (instruction.producesValue()) {
        std::string_view valueName = nameOfValue(instruction);
        line << std::format("%{} = ", valueName);
    }

    // Print opcode
    std::string_view opcodeStr = toString(instruction.getOpcode());
    line << opcodeStr;

    // Print operands
    for (size_t i = 0; i < instruction.getOperandCount(); ++i) {
        line << " ";
        line << printOperand(*instruction.getOperand(i));
        if (i < instruction.getOperandCount() - 1) {
            line << ",";
        }
    }

    addLineIndented(line.str());
}
std::string MirPrinter::printOperand(const Value& value) {
    switch (value.getNodeKind()) {
        case MirKind::Function: {
            const Function* func = static_cast<const Function*>(&value);
            return std::format("func {}", nameOfFunction(*func));
        }
        case MirKind::BasicBlock: {
            const BasicBlock* block = static_cast<const BasicBlock*>(&value);
            return std::format("{}", nameOfValue(*block));
        }
        case MirKind::Instruction: {
            const Instruction* instr = static_cast<const Instruction*>(&value);
            return std::format("%{}", nameOfValue(*instr));
        }
        case MirKind::Local: {
            const Local* local = static_cast<const Local*>(&value);
            std::string typeName = typeOfValue(*local)->toString();
            return std::format("{} %{}", typeName, nameOfValue(*local));
        }
        case MirKind::Constant: {
            const Constant* constant = static_cast<const Constant*>(&value);
            std::string typeName = typeOfValue(*constant)->toString();
            return std::format("{} ${}", typeName, nameOfValue(*constant));
        }

        default:
            VEE_UNREACHABLE("Unknown Value kind");
    }
}
std::string MirPrinter::printConstant(const Constant& constant) {
    std::string_view constName = _ctx.mir.valueNames.getName(&constant);

    std::string valueStr;
    switch (constant.getKind()) {
        case ConstantKind::Int: {
            const ConstantInt* intConst = constant.asInteger();
            valueStr = intConst->getValue().toString();
            break;
        }
        case ConstantKind::Float: {
            const ConstantFloat* floatConst = constant.asFloat();
            valueStr = std::to_string(floatConst->getValue());
            break;
        }
        case ConstantKind::String: {
            const ConstantString* stringConst = constant.asString();
            valueStr = std::string(stringConst->getValue());
            break;
        }
        case ConstantKind::Bool: {
            const ConstantBool* boolConst = constant.asBool();
            valueStr = boolConst->getValue() ? "true" : "false";
            break;
        }
    }

    return std::format("${} = {}", constName, valueStr);
}

std::string MirPrinter::indentStr() const {
    return std::string(_indent * 4, ' ');
}
void MirPrinter::append(const std::string& str) {
    _oss << str;
}
void MirPrinter::addIndented(const std::string& str) {
    _oss << indentStr() << str;
}
void MirPrinter::addLineIndented(const std::string& str) {
    _oss << indentStr() << str << "\n";
}
std::string_view MirPrinter::poolText(basic::StringId id) const {
    return _sp.get(id);
}

types::Type* MirPrinter::typeOfValue(const Value& value) const {
    return _ctx.mir.valueTypes.getValueType(&value);
}
std::string_view MirPrinter::nameOfValue(const Value& value) const {
    std::string_view name = _ctx.mir.valueNames.getName(&value);
    if (name.empty()) {
        if (!_unnamedValueNames.contains(&value)) {
            _unnamedValueNames[&value] = std::to_string(_valueCounter++);
        }
        return _unnamedValueNames[&value];
    }
    return name;
}
std::string_view MirPrinter::nameOfFunction(const Function& function) const {
    std::string_view name = _ctx.mir.valueNames.getName(&function);
    if (name.empty()) {
        if (!_unnamedFunctionNames.contains(&function)) {
            _unnamedFunctionNames[&function] = "fn" + std::to_string(_funcCounter++);
        }
        return _unnamedFunctionNames[&function];
    }
    return name;
}

} // namespace pretty
} // namespace mir
VEEC_NAMESPACE_END
