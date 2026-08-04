#include <vector>
#include <string>
#include <string_view>
#include <iostream>
#include <memory>

#include "veec/Compilation.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/BuiltinRegistrar.hpp"
#include "veec/basic/Token.hpp"
#include "veec/source/SourceView.hpp"
#include "veec/lexing/Lexer.hpp"
#include "veec/parsing/Parser.hpp"
#include "veec/ast/AstContext.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/PassManager.hpp"
#include "veec/sema/Scope.hpp"
#include "veec/sema/ScopeManager.hpp"
#include "veec/ast_passes/SymbolCollectionPass.hpp"
#include "veec/ast_passes/SymbolResolutionPass.hpp"
#include "veec/ast_passes/TypeConstructionPass.hpp"
#include "veec/ast_passes/TypeResolutionPass.hpp"
#include "veec/ast_passes/TypeCheckerPass.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirNode.hpp"
#include "veec/mir/Module.hpp"
#include "veec/mir/pretty/MirPrinter.hpp"
#include "veec/mirgen/AstToMirLowerer.hpp"
#include "veec/symbols/ent/ClassSymbol.hpp"
#include "veec/types/TypeTable.hpp"
#include "veec/types/ClassType.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"

int main() {
    using namespace veec;
    
    /*veec::Compiler compiler;
    veec::Compilation compilation = compiler.createCompilation();

    veec::CompilationContext ctx = compilation.getContext();

    // Add source files to the compilation
    ctx.addVirtualFile("virtual_file.v", "func main() -> int { defer return 0; }");
    compilation.run();
    */

    CompilationContext ctx;
    BuiltinRegistrar registrar(ctx);
    registrar.registerAll();

    constexpr std::string_view sourceCode = R"(

func add(a: i32, b: i32) -> i32 {
    return a + b;
}

func main() -> i32 {
    let x: i64 = 6;
    let y: i32 = add(x, 10);
    return y;
}

)";

	auto sourceId = ctx.sources.addVirtualFile("dummy.v", sourceCode);
    lexing::Lexer lexer(ctx, ctx.sources.getView(sourceId));

	std::cout << "Source Code:\n" << sourceCode << std::endl;

    std::cout << "\n\nTokens:\n";
    std::vector<lexing::Token> tokens = lexer.tokenize();
    for (const auto& token : tokens) {
        if (token.isTrivia()) continue;
        std::cout << "Token: " << veec::basic::tokenTypeToString(token.type()) << ", Range: ("
                  << token.range().startOffset << ", " << token.range().endOffset << ")\n";
    }

    parsing::TokenList tokenList{sourceId, std::move(tokens)};
    parsing::Parser parser(ctx, ctx.ast, tokenList);
    ast::CompilationUnitNode* ast = parser.parse();
    if (ast) {
        std::cout << "\n\nAST Generated:\n" + ast->toString(ctx) << std::endl;
    }

    if (!ast) return 1;

    // SEMAAAA
    sema::PassManager spm(ctx, ctx.sema);
    spm.addPass<ast_passes::SymbolCollectionPass>();
    spm.addPass<ast_passes::SymbolResolutionPass>();
    spm.addPass<ast_passes::TypeConstructionPass>();
    spm.addPass<ast_passes::TypeResolutionPass>();
    spm.addPass<ast_passes::TypeCheckerPass>();
    spm.runAll(*ast);

    for (const auto& diag : ctx.diagnostics.getDiagnostics()) {
        std::cout << diag.toString() << std::endl;
    }

    // MIRRRR
    mir::Module* mirModule = mirgen::AstToMirLowerer(ctx).lower(*ast);
    mir::pretty::MirPrinter printer(ctx);
    std::cout << "\n\nMIR:\n" << printer.printNode(*mirModule) << std::endl;

    return 0;
}
