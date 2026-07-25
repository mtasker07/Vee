#include "veec/Compilation.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationConfig.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/lexing/Lexer.hpp"
#include "veec/parsing/Parser.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/ProgramNode.hpp"
#include "veec/sema/SemanticAnalysisPass.hpp"
#include "veec/sema/SemanticPassManager.hpp"
#include "veec/sema/passes/SymbolCollectionPass.hpp"
#include "veec/sema/passes/SymbolResolutionPass.hpp"
#include "veec/sema/passes/InstructionValidationPass.hpp"

VEEC_NAMESPACE_BEGIN

void Compilation::addVirtualFile(std::string name, std::string_view contents) {
    _sourceManager.addVirtualFile(std::move(name), contents);
}

CompilationResult Compilation::run() {
    // Tokenize
    lexing::Lexer lexer(_ctx, _sourceManager.getView(_config.entryFileId));
    std::vector<lexing::Token> tokens = lexer.tokenize();
    if (ctx.diagnosticEngine.hasErrors()) {
        return false; // Tokenization failed
    }
    onTokenized(tokens);

    // Parse
    parsing::Parser parser(ctx, tokens);
    std::unique_ptr<parsing::ast::ProgramNode> program = parser.parse();
    if (ctx.diagnosticEngine.hasErrors()) {
        return false; // Parsing failed
    }
    onParsed(program.get());

    // Semantic analysis
    sema::SemanticPassManager passManager(ctx);
    passManager.addPass<sema::passes::SymbolCollectionPass>();
    passManager.addPass<sema::passes::SymbolResolutionPass>();
    passManager.addPass<sema::passes::InstructionValidationPass>();

    if (!passManager.runAll(program.get())) {
        return false; // Semantic analysis failed
    }

    // Lower to IR
    irgen::IRGenerator irGen;
    std::unique_ptr<ir::UASMModule> module = irGen.generateIR(program.get());
    if (!module) {
        UASM_FATAL("IR generation failed");
    }
    onLoweredToIR(*module);

    // Validate IR
    ir::IRValidator validator;
    if (!validator.validateModule(*module)) {
        UASM_FATAL("IR validation failed");
    }

    // Generate bytecode module
    bytecode::BytecodeGenerator bcGen;
    bytecode::BytecodeEncoder bcEncoder;
    uasm::bytecode::BytecodeModule bcModule = bcGen.generateModule(*module);
    onLoweredToBytecode(bcModule);

    // Encode module
    std::vector<u8> bytecode = bcEncoder.encodeModule(bcModule);
    onBytecodeEncoded(bytecode);

    CompilationResult result(bytecode);
    return result;
}

VEEC_NAMESPACE_END
