cmake_minimum_required(VERSION 3.20)

# VEEC shared library
set(VEEC_SOURCES
    #src/Compilation.cpp

    src/basic/APInt.cpp
    src/basic/BigInt.cpp
    src/basic/Arena.cpp
    src/basic/StringPool.cpp
    
    src/util/CharUtils.cpp

    src/source/SourceFile.cpp
    src/source/SourceManager.cpp

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
    
    src/sema/BuiltinRegistrar.cpp
    src/sema/ScopeManager.cpp

    src/symbols/Symbol.cpp
    src/symbols/SymbolTable.cpp

    src/types/TypeTable.cpp
    src/types/TypeSystem.cpp
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
