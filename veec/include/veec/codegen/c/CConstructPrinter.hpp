/**
 * @file CConstructPrinter.hpp
 * @brief This file contains the definition of the CConstructPrinter class which is
 * a class for emitting C code from C constructs.
 */

#pragma once

#include <string>
#include <sstream>
#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/codegen/c/CCodegenContext.hpp"
#include "veec/codegen/c/construct/CConstructFwd.hpp"
#include "veec/codegen/c/construct/CCompilationUnit.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {

/**
 * @class CConstructPrinter
 * @brief Responsible for emitting C code from C constructs.
 */
class CConstructPrinter {
public:
    CConstructPrinter(compilation::CompilationContext& ctx, CCodegenContext& cctx)
        : _ctx(ctx), _cctx(cctx) {}

    ~CConstructPrinter() = default;

    std::string printCompilationUnit(const construct::CCompilationUnit& unit);

private:
    compilation::CompilationContext& _ctx;
    CCodegenContext& _cctx;

    i32 _indentLevel = 0;
    std::ostringstream _oss;
    const construct::CCompilationUnit* _unit;
    
    void emitFunction(const construct::CFunction& cFunc);
    void emitStruct(const construct::CStruct& cStruct);
    void emitStmt(const construct::CStmt& cStmt);
    void emitExpr(const construct::CExpr& cExpr);

    void emitType(std::ostringstream& ss, const construct::CType& type);

    //
    // Writer helpers
    //
    std::string getIndentStr();

    template<typename... Args>
    void write(const std::string_view fmt, Args... args);
    template<typename... Args>
    void writeIndented(const std::string_view fmt, Args... args);
    template<typename... Args>
    void writeLine(const std::string_view fmt, Args... args);
    template<typename... Args>
    void writeLineIndented(const std::string_view fmt, Args... args);
};

} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
