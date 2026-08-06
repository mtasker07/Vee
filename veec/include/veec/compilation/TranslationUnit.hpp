/**
 * @file TranslationUnit.hpp
 * @brief This file contains the definition of the TranslationUnit class,
 * which is used to hold context for a single translation unit.
 */

#pragma once

#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/TokenList.hpp"
#include "veec/source/SourceFileId.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/mir/MirFwd.hpp"

VEEC_NAMESPACE_BEGIN
namespace compilation {

/**
 * @class TranslationUnit
 * @brief Used to hold context for a single translation unit.
 */
class TranslationUnit {
public:
    source::SourceFileId fileId;
    basic::TokenList tokens;
    ast::CompilationUnitNode* ast;
    mir::Module* mir;

    TranslationUnit() = default;
    ~TranslationUnit() = default;
};

} // namespace compilation
VEEC_NAMESPACE_END
