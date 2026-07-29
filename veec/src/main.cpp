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

use std::io::{Read, Write};

class MyClass {
    x: i32;
    y: i32;

    func new(_x: i32, _y: i32) -> MyClass {
        return MyClass { _x, _y };
    }

    func mult(a: i32, b: i32) -> i32 {
        return a * b;
    }
    func mult(a: i64, b: i64) -> i64 {
        return a * b;
    }
    
    func addByX(a: i32) -> i32 {
        return x + a;
    }
}

func main() -> i32 {
    let x: i64 = 6;

    let result1 = MyClass::mult(x, 4);

    let myObj = MyClass::new(x, 20);

    return myObj.addByX(x);
}

)";

	auto sourceId = ctx.sources.addVirtualFile("dummy.v", sourceCode);
    lexing::Lexer lexer(ctx, ctx.sources.getView(sourceId));

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
        std::cout << "AST Generated:\n" + ast->toString(ctx) << std::endl;
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

    std::cout << "TYPES:\n";
    types::TypeTable& typeTable = ctx.types.table;
    for (const auto& type : typeTable.getAllTypes()) {
        if (types::ClassType* classType = type->as<types::ClassType>()) {
            std::cout << "Class Type: " << ctx.strings.get(classType->getClassSymbol()->getNameValue()) << "\n";
            std::cout << "Fields:\n";
            for (const auto& [fieldNameId, fieldSymbol] : classType->getFields()) {
                std::cout << "  " << ctx.strings.get(fieldNameId) << "\n";
            }
            std::cout << "Methods:\n";
            for (const auto& [methodNameId, methodSymbols] : classType->getMethods()) {
                std::cout << "  " << ctx.strings.get(methodNameId) << "\n";
            }
            std::cout << std::endl;
        }
    }

    std::cout << "SYMBOLS:\n";
    symbols::SymbolTable& symTable = ctx.sema.symbols;
    for (const auto& symbol : symTable.getAllSymbols()) {
        std::string_view symbolName = symbol->getName().hasValue() ? ctx.strings.get(symbol->getNameValue()) : "<unnamed>";
        std::cout << "Symbol: '" << symbolName << "'  ";

        switch (symbol->getKind()) {
            case symbols::SymbolKind::Module:
                std::cout << "  Kind: Module\n";
                break;
            case symbols::SymbolKind::FunctionSet:
                std::cout << "  Kind: FunctionSet\n";
                break;
            case symbols::SymbolKind::Function:
                std::cout << "  Kind: Function\n";
                break;
            case symbols::SymbolKind::Class:
                std::cout << "  Kind: Class\n";
                break;
            case symbols::SymbolKind::Field:
                std::cout << "  Kind: Field\n";
                break;
            case symbols::SymbolKind::Variable:
                std::cout << "  Kind: Variable\n";
                break;
            case symbols::SymbolKind::Operator:
                std::cout << "  Kind: Operator\n";
                break;
            default:
                std::cout << "  Kind: Unknown\n";
                break;
        }
    }

    return 0;
}
