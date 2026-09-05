/**
 * @file CConstructGenerator.hpp
 * @brief This file contains the definition of the CConstructGenerator class which is
 * a class for generating C constructs from MIR constructs.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/codegen/c/CCodegenContext.hpp"
#include "veec/codegen/c/construct/CConstructFwd.hpp"
#include "veec/codegen/c/construct/CCompilationUnit.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {

/**
 * @class CConstructGenerator
 * @brief Responsible for generating C constructs from MIR constructs.
 */
class CConstructGenerator {
public:
    /**
     * @brief Creates a new CConstructGenerator instance.
     * @param ctx The compilation context.
     * @param cctx The C codegen context.
     */
    CConstructGenerator(compilation::CompilationContext& ctx, CCodegenContext& cctx)
        : _ctx(ctx), _cctx(cctx) {}

    ~CConstructGenerator() = default;

    /**
     * @brief Generates a C compilation unit for the given MIR module.
     * @param module The module to generate C constructs for.
     * @return A CCompilationUnit containing the generated C constructs.
     */
    construct::CCompilationUnit generateModule(const mir::Module& module);
    /**
     * @brief Generates a C compilation unit for the given MIR modules.
     * @param modules The modules to generate C constructs for.
     * @return A CCompilationUnit containing the generated C constructs.
     */
    construct::CCompilationUnit generateModules(const std::vector<const mir::Module*>& modules);

private:
    compilation::CompilationContext& _ctx;
    CCodegenContext& _cctx;

    construct::CCompilationUnit _unit;

    std::unordered_map<const mir::MirStructType*, construct::CStruct*> _structMap;

    //
    // Function state
    //

    std::unordered_map<const mir::Value*, std::string> _valueNames;
    std::unordered_map<const mir::BasicBlock*, std::string> _blockLabels;
    u32 _tempCounter = 0;
    u32 _labelCounter = 0;

    construct::CFunction* generateFunction(const mir::Function& func);
    construct::CStruct* generateStruct(const mir::MirStructType* type);

    void generateBlock(const mir::BasicBlock& block, std::vector<construct::CStmt*>& out);
    construct::CStmt* generateStmt(const mir::Instruction& inst);
    construct::CExpr* generateValue(const mir::Value* value);
    construct::CExpr* generateConstant(const mir::Constant* constant);

    // Creates new name if necessary vv
    const std::string& getValueName(const mir::Value* value);
    const std::string& getBlockLabel(const mir::BasicBlock* block);

    construct::CType* getType(const mir::MirType* type);
};

} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
