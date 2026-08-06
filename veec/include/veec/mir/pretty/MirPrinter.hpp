/**
 * @file MirPrinter.hpp
 * @brief Contains the definition of the MirPrinter class, which is
 * responsible for converting MIR nodes into human-readable string
 * representations for debugging and visualization purposes.
 */

#pragma once

#include <string>
#include <sstream>
#include <unordered_map>

#include "vee/core/CoreDefines.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/types/TypeFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace mir {
namespace pretty {

/**
 * @class MirPrinter
 * @brief Responsible for converting MIR nodes into human-readable
 * strings for debugging and visualization purposes.
 */
class MirPrinter {
public:
    /**
     * @brief Creates a new MirPrinter instance with the given SourceManager.
     * @param ctx The compilation context object.
     */
    MirPrinter(const compilation::CompilationContext& ctx)
        : _ctx(ctx), _sm(ctx.sources), _sp(ctx.strings) {}

    ~MirPrinter() = default;

    /**
     * @brief Converts the given MIR node to a human-readable string representation.
     * @param node The MIR node to convert.
     * @return A human-readable string representation of the MIR node.
     */
    std::string printNode(const MirNode& node);

private:
    std::ostringstream _oss;
    i32 _indent = 0;

    const compilation::CompilationContext& _ctx;
    const source::SourceManager& _sm;
    const basic::StringPool& _sp;

    mutable u32 _valueCounter = 1;
    mutable u32 _funcCounter = 1;
	mutable std::unordered_map<const Value*, std::string> _unnamedValueNames;
	mutable std::unordered_map<const Function*, std::string> _unnamedFunctionNames;

    void printModule(const Module& module);
    void printFunction(const Function& function);
    void printBasicBlock(const BasicBlock& block);
    void printInstruction(const Instruction& instruction);
    std::string printOperand(const Value& value);
    std::string printConstant(const Constant& constant);

    // Helpers
    std::string indentStr() const;
    void append(const std::string& str);
    void addIndented(const std::string& str);
    void addLineIndented(const std::string& str);
    std::string_view poolText(basic::StringId id) const;

    types::Type* typeOfValue(const Value& value) const;
    std::string_view nameOfValue(const Value& value) const;
    std::string_view nameOfFunction(const Function& function) const;
};

} // namespace pretty
} // namespace mir
VEEC_NAMESPACE_END
