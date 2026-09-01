/**
 * @file CompilationContext.hpp
 * @brief This file contains the definition of the CompilationContext struct,
 * which is used to hold context for the compilation process.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/UnitManager.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/io/IWriter.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/ast/AstContext.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/mir/MirContext.hpp"
#include "veec/types/TypeContext.hpp"
#include "veec/codegen/CodegenContext.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

/**
 * @class CompilationContext
 * @brief Used to hold context for the compilation process.
 */
class CompilationContext {
public:
    UnitManager units;
    basic::StringPool strings;
    source::SourceManager sources;
    diagnostics::DiagnosticEngine diagnostics;

    // Subcontexts
    ast::AstContext ast;
    sema::SemaContext sema;
    types::TypeContext types;
    mir::MirContext mir;
    codegen::CodegenContext codegen;

    CompilationContext() = default;
    ~CompilationContext() = default;
};

} // namespace compilation
VEEC_NAMESPACE_END
