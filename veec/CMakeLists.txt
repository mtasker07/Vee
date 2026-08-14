cmake_minimum_required(VERSION 3.20)

# VEEC shared library
set(VEEC_SOURCES
    src/BuiltinRegistrar.cpp

    src/compilation/Compilation.cpp
    src/compilation/CompilationConfig.cpp
    src/compilation/CompilerDriver.cpp

    src/basic/APInt.cpp
    src/basic/BigInt.cpp
    src/basic/Arena.cpp
    src/basic/StringPool.cpp
    
    src/io/StreamWriter.cpp
    src/io/StringWriter.cpp
    src/io/FileWriter.cpp
    
    src/source/SourceFile.cpp
    src/source/SourceManager.cpp
    src/source/SourceRange.cpp

    src/fs/Path.cpp

    src/cli/args/CLIArgs.cpp
    src/cli/args/CLIOption.cpp
    src/cli/args/CLIOptionsGenerator.cpp
    src/cli/args/CLIValue.cpp
    src/cli/args/lexing/CLIArgsLexer.cpp
    src/cli/args/parsing/CLIArgsParser.cpp
    src/cli/args/validation/CLIArgsValidator.cpp
    
    src/lexing/Lexer.cpp
    
    src/parsing/Parser.cpp
    
    src/ast/AstNode.cpp
    src/ast/AstWalker.cpp
    src/ast/AstPrinter.cpp
    
    src/ast_passes/SymbolCollectionPass.cpp
    src/ast_passes/TopLevelUseResolutionPass.cpp
    src/ast_passes/SymbolResolutionPass.cpp
    src/ast_passes/TypeConstructionPass.cpp
    src/ast_passes/TypeResolutionPass.cpp
    src/ast_passes/TypeCheckerPass.cpp
    
    src/mir/MirNode.cpp
    src/mir/MirFactory.cpp
    src/mir/MirBuilder.cpp
    src/mir/Module.cpp
    src/mir/Function.cpp
    src/mir/BasicBlock.cpp
    src/mir/Instruction.cpp
    src/mir/Value.cpp
    src/mir/Constant.cpp
    src/mir/pretty/MirPrinter.cpp
    src/mir/support/ConstantTable.cpp
    
    src/mirgen/AstToMirLowerer.cpp
    
    src/sema/ScopeManager.cpp
    
    src/symbols/Symbol.cpp
    src/symbols/SymbolTable.cpp
    
    src/types/TypeTable.cpp
    src/types/TypeSystem.cpp
    
    src/diagnostics/DiagnosticRange.cpp
    src/diagnostics/rendering/TerminalDiagnosticRenderer.cpp
    src/util/CharUtils.cpp

)
    
add_library(veec_lib
    ${VEEC_SOURCES}
)

# VEEC main executable
add_executable(veec
    src/main.cpp
)

target_link_libraries(veec_lib
PUBLIC
	vee_core
)

target_include_directories(veec_lib
PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(veec
PRIVATE
	veec_lib
)

add_subdirectory(tests)
