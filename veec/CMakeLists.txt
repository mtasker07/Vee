cmake_minimum_required(VERSION 3.20)

# VEEC shared library
set(VEEC_SOURCES
    src/Driver.cpp
    src/BuiltinRegistrar.cpp

    src/compilation/Compilation.cpp

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

    src/cli/CLI.cpp
    src/cli/CLIOption.cpp
    src/cli/CLIRawArgs.cpp
    src/cli/CLIValue.cpp
    src/cli/commands/Compile.cpp
    src/cli/delegate/CLIOptionValidationDelegates.cpp
    src/cli/descriptor/CLICommandDescriptor.cpp
    src/cli/descriptor/CLIOptionDescriptor.cpp
    src/cli/descriptor/CLIRootDescriptor.cpp
    src/cli/lexing/CLIArgsLexer.cpp
    src/cli/parsing/CLIArgsParser.cpp
    src/cli/validation/CLIArgsValidator.cpp

    src/lexing/Lexer.cpp
    
    src/parsing/Parser.cpp
    
    src/ast/AstNode.cpp
    src/ast/AstWalker.cpp
    src/ast/AstPrinter.cpp
    
    src/sema_passes/SymbolCollectionPass.cpp
    src/sema_passes/TopLevelUseResolutionPass.cpp
    src/sema_passes/SymbolResolutionPass.cpp
    src/sema_passes/TypeConstructionPass.cpp
    src/sema_passes/TypeResolutionPass.cpp
    src/sema_passes/TypeCheckerPass.cpp
    src/sema_passes/ControlFlowValidationPass.cpp
    
    src/mir/MirNode.cpp
    src/mir/MirFactory.cpp
    src/mir/MirBuilder.cpp
    src/mir/Module.cpp
    src/mir/Function.cpp
    src/mir/BasicBlock.cpp
    src/mir/Instruction.cpp
    src/mir/Value.cpp
    src/mir/Constant.cpp
    src/mir/MirType.cpp
    src/mir/pretty/MirPrinter.cpp
    src/mir/support/ConstantTable.cpp
    src/mir/support/TypeTable.cpp
    
    src/mirgen/AstToMirLowerer.cpp
    src/mirgen/MirTypeConverter.cpp
    src/mirgen/support/FunctionCollector.cpp
    
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
