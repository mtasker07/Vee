#include "veec/codegen/c/CConstructPrinter.hpp"

#include <string>
#include <string_view>
#include <sstream>
#include <format>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/codegen/c/construct/CCompilationUnit.hpp"
#include "veec/codegen/c/construct/CFunction.hpp"
#include "veec/codegen/c/construct/CStruct.hpp"
#include "veec/codegen/c/construct/CEnum.hpp"
#include "veec/codegen/c/construct/type/CType.hpp"
#include "veec/codegen/c/construct/type/CPrimitiveType.hpp"
#include "veec/codegen/c/construct/type/CStructType.hpp"
#include "veec/codegen/c/construct/type/CEnumType.hpp"
#include "veec/codegen/c/construct/type/CFunctionType.hpp"
#include "veec/codegen/c/construct/type/CTypedefType.hpp"
#include "veec/codegen/c/construct/type/CPointerType.hpp"
#include "veec/codegen/c/construct/stmt/CBlockStmt.hpp"
#include "veec/codegen/c/construct/stmt/CCallStmt.hpp"
#include "veec/codegen/c/construct/stmt/CDeclStmt.hpp"
#include "veec/codegen/c/construct/stmt/CAssignmentStmt.hpp"
#include "veec/codegen/c/construct/stmt/CExprStmt.hpp"
#include "veec/codegen/c/construct/stmt/CReturnStmt.hpp"
#include "veec/codegen/c/construct/stmt/CGotoStmt.hpp"
#include "veec/codegen/c/construct/stmt/CLabelStmt.hpp"
#include "veec/codegen/c/construct/stmt/CIfStmt.hpp"
#include "veec/codegen/c/construct/stmt/CEmptyStmt.hpp"
#include "veec/codegen/c/construct/expr/CExpr.hpp"
#include "veec/codegen/c/construct/expr/CIdentifierExpr.hpp"
#include "veec/codegen/c/construct/expr/CIntLiteralExpr.hpp"
#include "veec/codegen/c/construct/expr/CFloatLiteralExpr.hpp"
#include "veec/codegen/c/construct/expr/CBoolLiteralExpr.hpp"
#include "veec/codegen/c/construct/expr/CStringLiteralExpr.hpp"
#include "veec/codegen/c/construct/expr/CBinaryExpr.hpp"
#include "veec/codegen/c/construct/expr/CUnaryExpr.hpp"
#include "veec/codegen/c/construct/expr/CCallExpr.hpp"
#include "veec/codegen/c/construct/expr/CCastExpr.hpp"
#include "veec/codegen/c/construct/expr/CCompoundLiteralExpr.hpp"

VEEC_NAMESPACE_BEGIN
namespace codegen {
namespace c {

namespace {

std::string_view unaryOperatorToString(construct::CUnaryOperator op) {
    switch (op) {
        case construct::CUnaryOperator::Neg: return "-";
        case construct::CUnaryOperator::BitNot: return "~";
        case construct::CUnaryOperator::LogicalNot: return "!";
        case construct::CUnaryOperator::Deref: return "*";
        case construct::CUnaryOperator::AddrOf: return "&";
    }
    VEE_UNREACHABLE("Unknown C unary operator: {}", std::to_string(static_cast<int>(op)));
}

std::string_view binaryOperatorToString(construct::CBinaryOperator op) {
    switch (op) {
        case construct::CBinaryOperator::Add: return "+";
        case construct::CBinaryOperator::Sub: return "-";
        case construct::CBinaryOperator::Mul: return "*";
        case construct::CBinaryOperator::Div: return "/";
        case construct::CBinaryOperator::Mod: return "%";
        case construct::CBinaryOperator::BitAnd: return "&";
        case construct::CBinaryOperator::BitOr: return "|";
        case construct::CBinaryOperator::BitXor: return "^";
        case construct::CBinaryOperator::Shl: return "<<";
        case construct::CBinaryOperator::Shr: return ">>";
        case construct::CBinaryOperator::LogicalAnd: return "&&";
        case construct::CBinaryOperator::LogicalOr: return "||";
        case construct::CBinaryOperator::Eq: return "==";
        case construct::CBinaryOperator::Ne: return "!=";
        case construct::CBinaryOperator::Lt: return "<";
        case construct::CBinaryOperator::Le: return "<=";
        case construct::CBinaryOperator::Gt: return ">";
        case construct::CBinaryOperator::Ge: return ">=";
    }
    VEE_UNREACHABLE("Unknown C binary operator: {}", std::to_string(static_cast<int>(op)));
}

std::string_view primitiveTypeToString(construct::CPrimitiveTypeKind kind) {
    switch (kind) {
        case construct::CPrimitiveTypeKind::Void: return "void";
        case construct::CPrimitiveTypeKind::Bool: return "bool";
        case construct::CPrimitiveTypeKind::Char: return "signed char";
        case construct::CPrimitiveTypeKind::UnsignedChar: return "unsigned char";
        case construct::CPrimitiveTypeKind::Short: return "short";
        case construct::CPrimitiveTypeKind::UnsignedShort: return "unsigned short";
        case construct::CPrimitiveTypeKind::Int: return "int";
        case construct::CPrimitiveTypeKind::UnsignedInt: return "unsigned int";
        case construct::CPrimitiveTypeKind::Long: return "long";
        case construct::CPrimitiveTypeKind::UnsignedLong: return "unsigned long";
        case construct::CPrimitiveTypeKind::LongLong: return "long long";
        case construct::CPrimitiveTypeKind::UnsignedLongLong: return "unsigned long long";
        case construct::CPrimitiveTypeKind::Float: return "float";
        case construct::CPrimitiveTypeKind::Double: return "double";
        case construct::CPrimitiveTypeKind::LongDouble: return "long double";
    }
    VEE_UNREACHABLE("Unknown C primitive type kind: {}", std::to_string(static_cast<int>(kind)));
}

} // namespace

std::string CConstructPrinter::printCompilationUnit(const construct::CCompilationUnit& unit) {
    _oss.str("");
    _oss.clear();

    for (const auto& strct : unit.structs) {
        emitStruct(*strct);
    }
    for (const auto& func : unit.functions) {
        emitFunction(*func);
    }

    return _oss.str();
}

void CConstructPrinter::emitFunction(const construct::CFunction& cFunc) {
    std::ostringstream returnTypeStream;
    emitType(returnTypeStream, *cFunc.returnType);
    writeIndented("{} {}(", returnTypeStream.str(), cFunc.name);

    if (cFunc.params.empty()) {
        write("void");
    } else {
        for (size_t i = 0; i < cFunc.params.size(); ++i) {
            if (i > 0) {
                write(", ");
            }
            std::ostringstream paramTypeStream;
            emitType(paramTypeStream, *cFunc.params[i]->type);
            write("{} {}", paramTypeStream.str(), cFunc.params[i]->name);
        }
    }
    writeLine(") {{");

    _indentLevel++;
    for (const construct::CStmt* stmt : cFunc.body->statements) {
        emitStmt(*stmt);
    }
    _indentLevel--;

    writeLine("}}");
    write("\n");
}
void CConstructPrinter::emitStruct(const construct::CStruct& cStruct) {
    writeLine("struct {} {{", cStruct.name);

    _indentLevel++;
    for (const construct::CStructMember* member : cStruct.members) {
        std::ostringstream typeStream;
        emitType(typeStream, *member->type);
        writeLineIndented("{} {};", typeStream.str(), member->name);
    }
    _indentLevel--;

    writeLine("}};");
    write("\n");
}
void CConstructPrinter::emitStmt(const construct::CStmt& cStmt) {
    if (const auto* block = dynamic_cast<const construct::CBlockStmt*>(&cStmt)) {
        writeLineIndented("{{");
        _indentLevel++;
        for (const construct::CStmt* stmt : block->statements) {
            emitStmt(*stmt);
        }
        _indentLevel--;
        writeLineIndented("}}");
        return;
    }
    if (const auto* decl = dynamic_cast<const construct::CDeclStmt*>(&cStmt)) {
        std::ostringstream typeStream;
        emitType(typeStream, *decl->type);
        writeIndented("{} {}", typeStream.str(), decl->name);
        if (decl->init) {
            write(" = ");
            emitExpr(*decl->init);
        }
        writeLine(";");
        return;
    }
    if (const auto* assign = dynamic_cast<const construct::CAssignmentStmt*>(&cStmt)) {
        writeIndented("");
        emitExpr(*assign->lhs);
        write(" = ");
        emitExpr(*assign->rhs);
        writeLine(";");
        return;
    }
    if (const auto* exprStmt = dynamic_cast<const construct::CExprStmt*>(&cStmt)) {
        writeIndented("");
        emitExpr(*exprStmt->expr);
        writeLine(";");
        return;
    }
    if (const auto* callStmt = dynamic_cast<const construct::CCallStmt*>(&cStmt)) {
        writeIndented("");
        emitExpr(*callStmt->function);
        write("(");
        for (size_t i = 0; i < callStmt->arguments.size(); ++i) {
            if (i > 0) {
                write(", ");
            }
            emitExpr(*callStmt->arguments[i]);
        }
        writeLine(");");
        return;
    }
    if (const auto* ret = dynamic_cast<const construct::CReturnStmt*>(&cStmt)) {
        if (ret->value) {
            writeIndented("return ");
            emitExpr(*ret->value);
            writeLine(";");
        } else {
            writeLineIndented("return;");
        }
        return;
    }
    if (const auto* gotoStmt = dynamic_cast<const construct::CGotoStmt*>(&cStmt)) {
        writeLineIndented("goto {};", gotoStmt->label);
        return;
    }
    if (const auto* label = dynamic_cast<const construct::CLabelStmt*>(&cStmt)) {
        writeLine("{}:", label->label);
        emitStmt(*label->stmt);
        return;
    }
    if (const auto* ifStmt = dynamic_cast<const construct::CIfStmt*>(&cStmt)) {
        writeIndented("if (");
        emitExpr(*ifStmt->condition);
        writeLine(") {{");
        _indentLevel++;
        emitStmt(*ifStmt->thenStmt);
        _indentLevel--;
        if (ifStmt->elseStmt) {
            writeLineIndented("}} else {{");
            _indentLevel++;
            emitStmt(*ifStmt->elseStmt);
            _indentLevel--;
        }
        writeLineIndented("}}");
        return;
    }
    if (dynamic_cast<const construct::CEmptyStmt*>(&cStmt)) {
        writeLineIndented(";");
        return;
    }

    VEE_FATAL("Unknown C statement construct");
}
void CConstructPrinter::emitExpr(const construct::CExpr& cExpr) {
    if (const auto* ident = dynamic_cast<const construct::CIdentifierExpr*>(&cExpr)) {
        write("{}", ident->name);
        return;
    }
    if (const auto* intLit = dynamic_cast<const construct::CIntLiteralExpr*>(&cExpr)) {
        write("{}", intLit->value.toString());
        return;
    }
    if (const auto* floatLit = dynamic_cast<const construct::CFloatLiteralExpr*>(&cExpr)) {
        write("{}{}", floatLit->value, floatLit->isDouble ? "" : "f");
        return;
    }
    if (const auto* boolLit = dynamic_cast<const construct::CBoolLiteralExpr*>(&cExpr)) {
        write("{}", boolLit->value ? "true" : "false");
        return;
    }
    if (const auto* strLit = dynamic_cast<const construct::CStringLiteralExpr*>(&cExpr)) {
        write("\"{}\"", strLit->value);
        return;
    }
    if (const auto* unExpr = dynamic_cast<const construct::CUnaryExpr*>(&cExpr)) {
        write("({}", unaryOperatorToString(unExpr->op));
        emitExpr(*unExpr->operand);
        write(")");
        return;
    }
    if (const auto* binExpr = dynamic_cast<const construct::CBinaryExpr*>(&cExpr)) {
        write("(");
        emitExpr(*binExpr->lhs);
        write(" {} ", binaryOperatorToString(binExpr->op));
        emitExpr(*binExpr->rhs);
        write(")");
        return;
    }
    if (const auto* callExpr = dynamic_cast<const construct::CCallExpr*>(&cExpr)) {
        emitExpr(*callExpr->callee);
        write("(");
        for (size_t i = 0; i < callExpr->args.size(); ++i) {
            if (i > 0) {
                write(", ");
            }
            emitExpr(*callExpr->args[i]);
        }
        write(")");
        return;
    }
    if (const auto* castExpr = dynamic_cast<const construct::CCastExpr*>(&cExpr)) {
        std::ostringstream typeStream;
        emitType(typeStream, *castExpr->targetType);
        write("(({})", typeStream.str());
        emitExpr(*castExpr->operand);
        write(")");
        return;
    }
    if (const auto* compound = dynamic_cast<const construct::CCompoundLiteralExpr*>(&cExpr)) {
        std::ostringstream typeStream;
        emitType(typeStream, *compound->type);
        write("({}) {{ ", typeStream.str());
        for (size_t i = 0; i < compound->values.size(); ++i) {
            if (i > 0) {
                write(", ");
            }
            emitExpr(*compound->values[i]);
        }
        write(" }}");
        return;
    }

    VEE_FATAL("Unknown C expression construct");
}

void CConstructPrinter::emitType(std::ostringstream& ss, const construct::CType& type) {
    if (const auto* prim = dynamic_cast<const construct::CPrimitiveType*>(&type)) {
        ss << primitiveTypeToString(prim->kind);
        return;
    }
    if (const auto* pointerTy = dynamic_cast<const construct::CPointerType*>(&type)) {
        emitType(ss, *pointerTy->pointeeType);
        ss << "*";
        return;
    }
    if (const auto* structTy = dynamic_cast<const construct::CStructType*>(&type)) {
        if (structTy->kind == construct::CStructTypeKind::ReferenceToDefined) {
            ss << "struct " << structTy->referenceToDefined->name;
        } else {
            ss << "struct { ";
            for (const construct::CType* member : structTy->inlineMembers) {
                emitType(ss, *member);
                ss << "; ";
            }
            ss << "}";
        }
        return;
    }
    if (const auto* enumTy = dynamic_cast<const construct::CEnumType*>(&type)) {
        emitType(ss, *enumTy->underlyingType);
        return;
    }
    if (const auto* functionTy = dynamic_cast<const construct::CFunctionType*>(&type)) {
        emitType(ss, *functionTy->returnType);
        ss << " (*)(";
        for (size_t i = 0; i < functionTy->paramTypes.size(); ++i) {
            if (i > 0) {
                ss << ", ";
            }
            emitType(ss, *functionTy->paramTypes[i]);
        }
        ss << ")";
        return;
    }
    if (const auto* typedefTy = dynamic_cast<const construct::CTypedefType*>(&type)) {
        ss << typedefTy->name;
        return;
    }

    VEE_FATAL("Unknown C type construct");
}

std::string CConstructPrinter::getIndentStr() {
    return std::string(_indentLevel * 4, ' ');
}

template<typename... Args>
void CConstructPrinter::write(const std::string_view fmt, Args... args) {
    _oss << std::vformat(fmt, std::make_format_args(args...));
}
template<typename... Args>
void CConstructPrinter::writeIndented(const std::string_view fmt, Args... args) {
    _oss << getIndentStr();
    _oss << std::vformat(fmt, std::make_format_args(args...));
}
template<typename... Args>
void CConstructPrinter::writeLine(const std::string_view fmt, Args... args) {
    _oss << std::vformat(fmt, std::make_format_args(args...)) << "\n";
}
template<typename... Args>
void CConstructPrinter::writeLineIndented(const std::string_view fmt, Args... args) {
    _oss << getIndentStr();
    _oss << std::vformat(fmt, std::make_format_args(args...)) << "\n";
}

} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
