#include "veec/compilation/Compilation.hpp"

#include <utility>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/BuiltinRegistrar.hpp"
#include "veec/compilation/CompilationConfig.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/compilation/TranslationUnit.hpp"
#include "veec/basic/TokenList.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/source/SourceFile.hpp"
#include "veec/source/SourceView.hpp"
#include "veec/lexing/Lexer.hpp"
#include "veec/parsing/Parser.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/PassManager.hpp"
#include "veec/sema_passes/SymbolCollectionPass.hpp"
#include "veec/sema_passes/SymbolResolutionPass.hpp"
#include "veec/sema_passes/TypeConstructionPass.hpp"
#include "veec/sema_passes/TypeResolutionPass.hpp"
#include "veec/sema_passes/TypeCheckerPass.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mirgen/AstToMirLowerer.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

CompilationResult Compilation::compile() {
    CompilationResult result;
    result.success = false;
    
    // Handle diagnostics generated before main process
    if (_ctx.diagnostics.hasErrors()) {
        result.diagnostics = _ctx.diagnostics.getDiagnostics();
        return result;
    }

    // Create units for all source files
    runForEachFile([this](source::SourceFile* sourceFile) {
        _ctx.units.createUnit(sourceFile);
        return true;
    });

    auto runPhase = [this, &result](auto phaseFn) -> bool {
        if (!phaseFn()) {
            result.diagnostics = _ctx.diagnostics.getDiagnostics();
            return false;
        }
        return true;
    };

    //
    // -- MAIN PIPELINE --
    //
    
    registerBuiltins();

    if (!runPhase([this]() { return tokenize(); })) {
        return result;
    }
    if (!runPhase([this]() { return parse(); })) {
        return result;
    }
    if (!runPhase([this]() { return analyze(); })) {
        return result;
    }
    if (!runPhase([this]() { return generateMir(); })) {
        return result;
    }

    result.success = true;
    return result;
}

void Compilation::registerBuiltins() {
    BuiltinRegistrar registrar(_ctx);
    registrar.registerAll();
}
bool Compilation::tokenize() {
    return runForEachUnit([this](TranslationUnit* unit) {
        source::SourceView sourceView = source::SourceView(unit->sourceFile);
        lexing::Lexer lexer(_ctx, sourceView);
        basic::TokenList tokens = lexer.tokenize();
        unit->tokens = std::move(tokens);

        return !_ctx.diagnostics.hasErrors();
    });
}
bool Compilation::parse() {
    return runForEachUnit([this](TranslationUnit* unit) {
        const basic::TokenList& tokens = unit->tokens;
        parsing::Parser parser(_ctx, tokens);
        ast::CompilationUnitNode* ast = parser.parse();
        unit->ast = ast;

        return !_ctx.diagnostics.hasErrors();
    });
}
bool Compilation::analyze() {
    sema::PassManager pm(_ctx);
    
    // vv ALL SEMA PASSES GO HERE (ORDERED) vv
    pm.addPass<sema_passes::SymbolCollectionPass>();
    pm.addPass<sema_passes::SymbolResolutionPass>();
    pm.addPass<sema_passes::TypeConstructionPass>();
    pm.addPass<sema_passes::TypeResolutionPass>();
    pm.addPass<sema_passes::TypeCheckerPass>();

    const std::vector<TranslationUnit*>& units = _ctx.units.getAllUnits();
    return pm.runAllForUnits(units);
}
bool Compilation::generateMir() {
    // NOTE: mirgen doesn't "fail" like other passes, errors during mirgen
    // are fatal, because they should all have been caught during semantic analysis.
    // If mirgen fails, it indicates a bug in the compiler. Therefore we always
    // return success here.
    mirgen::AstToMirLowerer lowerer(_ctx);
    return runForEachUnit([this, &lowerer](TranslationUnit* unit) {
        ast::CompilationUnitNode* ast = unit->ast;
        VEE_ASSERT(ast != nullptr, "AST for unit is null");

        mir::Module* module = lowerer.lower(*ast);
        unit->mir = module;

        return true;
    });
}

} // namespace compilation
VEEC_NAMESPACE_END
