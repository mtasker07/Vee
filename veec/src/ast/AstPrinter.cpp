#include "veec/ast/AstPrinter.hpp"

#include <format>
#include <string_view>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/basic/BigInt.hpp"
#include "veec/basic/Token.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/io/IWriter.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/ast/AstKind.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
#include "veec/ast/ItemNode.hpp"
#include "veec/ast/name/QualifiedNameNode.hpp"
#include "veec/ast/generic/GenericArgsNode.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"
#include "veec/ast/expr/ParenthesizedExprNode.hpp"
#include "veec/ast/expr/LiteralExprNode.hpp"
#include "veec/ast/expr/IntLiteralExprNode.hpp"
#include "veec/ast/expr/FloatLiteralExprNode.hpp"
#include "veec/ast/expr/StringLiteralExprNode.hpp"
#include "veec/ast/expr/BoolLiteralExprNode.hpp"
#include "veec/ast/expr/UnaryExprNode.hpp"
#include "veec/ast/expr/BinaryExprNode.hpp"
#include "veec/ast/expr/AssignmentExprNode.hpp"
#include "veec/ast/expr/NameExprNode.hpp"
#include "veec/ast/expr/CallExprNode.hpp"
#include "veec/ast/expr/IndexExprNode.hpp"
#include "veec/ast/expr/MemberAccessExprNode.hpp"
#include "veec/ast/expr/ConstructExprNode.hpp"
#include "veec/ast/decl/DeclarationNode.hpp"
#include "veec/ast/decl/ModuleDeclNode.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/ParameterDeclNode.hpp"
#include "veec/ast/decl/ClassDeclNode.hpp"
#include "veec/ast/decl/MethodDeclNode.hpp"
#include "veec/ast/decl/FieldDeclNode.hpp"
#include "veec/ast/decl/VariableDeclNode.hpp"
#include "veec/ast/stmt/StatementNode.hpp"
#include "veec/ast/stmt/BlockStmtNode.hpp"
#include "veec/ast/stmt/ExpressionStmtNode.hpp"
#include "veec/ast/stmt/IfStmtNode.hpp"
#include "veec/ast/stmt/LoopStmtNode.hpp"
#include "veec/ast/stmt/WhileStmtNode.hpp"
#include "veec/ast/stmt/ForStmtNode.hpp"
#include "veec/ast/stmt/ReturnStmtNode.hpp"
#include "veec/ast/stmt/UseStmtNode.hpp"
#include "veec/ast/type/TypeNode.hpp"
#include "veec/ast/type/BuiltinTypeNode.hpp"
#include "veec/ast/type/NamedTypeNode.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast {

namespace {

/// Internal walker class used by AstPrinter to traverse
/// the AST and construct string.
class AstPrinterWalker : public ConstAstWalker {
public:
    AstPrinterWalker(const source::SourceManager& sm, const basic::StringPool& sp)
        : _sm(sm), _sp(sp) {}

    ~AstPrinterWalker() = default;

    inline void reset() {
        _result.clear();
        _indent = 0;
    }

    inline std::string getResult() {
        return _result;
    }

private:
    const source::SourceManager& _sm;
    const basic::StringPool& _sp;
    std::string _result;
    int _indent = 0;

    void visitCompilationUnit(const CompilationUnitNode& node) override;
    void visitQualifiedName(const QualifiedNameNode& node) override;
    void visitGenericArgs(const GenericArgsNode& node) override;
    void visitParenthesizedExpr(const ParenthesizedExprNode& node) override;
    void visitLiteralExpr(const LiteralExprNode& node) override;
    void visitIntLiteralExpr(const IntLiteralExprNode& node) override;
    void visitFloatLiteralExpr(const FloatLiteralExprNode& node) override;
    void visitStringLiteralExpr(const StringLiteralExprNode& node) override;
    void visitBoolLiteralExpr(const BoolLiteralExprNode& node) override;
    void visitUnaryExpr(const UnaryExprNode& node) override;
    void visitBinaryExpr(const BinaryExprNode& node) override;
    void visitAssignmentExpr(const AssignmentExprNode& node) override;
    void visitNameExpr(const NameExprNode& node) override;
    void visitCallExpr(const CallExprNode& node) override;
    void visitIndexExpr(const IndexExprNode& node) override;
    void visitMemberAccessExpr(const MemberAccessExprNode& node) override;
    void visitConstructExpr(const ConstructExprNode& node) override;
    void visitModuleDecl(const ModuleDeclNode& node) override;
    void visitFunctionDecl(const FunctionDeclNode& node) override;
    void visitParameterDecl(const ParameterDeclNode& node) override;
    void visitClassDecl(const ClassDeclNode& node) override;
    void visitMethodDecl(const MethodDeclNode& node) override;
    void visitFieldDecl(const FieldDeclNode& node) override;
    void visitVariableDecl(const VariableDeclNode& node) override;
    void visitBlockStmt(const BlockStmtNode& node) override;
    void visitExpressionStmt(const ExpressionStmtNode& node) override;
    void visitIfStmt(const IfStmtNode& node) override;
    void visitLoopStmt(const LoopStmtNode& node) override;
    void visitWhileStmt(const WhileStmtNode& node) override;
    void visitForStmt(const ForStmtNode& node) override;
    void visitReturnStmt(const ReturnStmtNode& node) override;
    void visitUseStmt(const UseStmtNode& node) override;
    void visitBuiltinType(const BuiltinTypeNode& node) override;
    void visitNamedType(const NamedTypeNode& node) override;

    // Helpers
    std::string indentStr() const;
    void addIndented(const std::string& str);
    void addLineIndented(const std::string& str);
    std::string_view tokenText(const Token& token) const;
    std::string_view srcText(const source::SourceRange& range) const;
    std::string_view poolText(basic::StringId id) const;
};

/// Convert BuiltinTypeKind to string.
std::string_view builtinTypeKindToString(BuiltinTypeKind kind) {
    switch (kind) {
        case BuiltinTypeKind::Void: return "void";
        case BuiltinTypeKind::I8: return "i8";
        case BuiltinTypeKind::U8: return "u8";
        case BuiltinTypeKind::I16: return "i16";
        case BuiltinTypeKind::U16: return "u16";
        case BuiltinTypeKind::I32: return "i32";
        case BuiltinTypeKind::U32: return "u32";
        case BuiltinTypeKind::I64: return "i64";
        case BuiltinTypeKind::U64: return "u64";
        case BuiltinTypeKind::F32: return "f32";
        case BuiltinTypeKind::F64: return "f64";
        default:
            VEE_UNREACHABLE("Unknown BuiltinTypeKind");
    }
}

/// Convert LiteralType to string.
std::string_view literalTypeToString(LiteralType type) {
    switch (type) {
        case LiteralType::Integer: return "Integer";
        case LiteralType::Float: return "Float";
        case LiteralType::String: return "String";
        case LiteralType::Bool: return "Bool";
        default:
            VEE_UNREACHABLE("Unknown LiteralType");
    }
}

std::string_view memberAccessOpToString(MemberAccessOp op) {
    switch (op) {
        case MemberAccessOp::Dot: return ".";
        case MemberAccessOp::Arrow: return "->";
        default:
            VEE_UNREACHABLE("Unknown MemberAccessOp");
    }
}

} // namespace

void AstPrinterWalker::visitCompilationUnit(const CompilationUnitNode& node) {
    addLineIndented("<CompilationUnitNode>");
    _indent++;
    addLineIndented("Items:");
    _indent++;
    const auto& items = node.getItems();
    if (items.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& child : items) {
        walk(*child);
    }
    _indent -= 2;
}
void AstPrinterWalker::visitQualifiedName(const QualifiedNameNode& node) {
    addLineIndented("<QualifiedNameNode>");
    _indent++;
    addLineIndented("Segments:");
    _indent++;
    for (const auto& segment : node.getSegments()) {
        addLineIndented(std::format("Segment: {}", poolText(segment.identifier.id)));
        if (segment.genericArgs) {
            addLineIndented("GenericArgs:");
            _indent++;
            walk(*segment.genericArgs);
            _indent--;
        }
    }
    _indent -= 2;
}
void AstPrinterWalker::visitGenericArgs(const GenericArgsNode& node) {
    addLineIndented("<GenericArgsNode>");
    _indent++;
    addLineIndented("Arguments:");
    _indent++;
    const auto& args = node.getArgs();
    if (args.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& arg : args) {
        walk(*arg);
    }
    _indent -= 2;
}
void AstPrinterWalker::visitParenthesizedExpr(const ParenthesizedExprNode& node) {
    addLineIndented("<ParenthesizedExprNode>");
    _indent++;
    if (node.getInnerExpr()) {
        addLineIndented("InnerExpression:");
        _indent++;
        walk(*node.getInnerExpr());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitLiteralExpr(const LiteralExprNode& node) {
    addLineIndented("<LiteralExprNode>");
    _indent++;
    addLineIndented(std::format("Type: {}", literalTypeToString(node.getLiteralType())));
    addLineIndented(std::format("Token: {}", srcText(node.getRange())));
    _indent--;
}
void AstPrinterWalker::visitIntLiteralExpr(const IntLiteralExprNode& node) {
    addLineIndented("<IntLiteralExprNode>");
    _indent++;
    const basic::BigInt& value = node.getValue();
    addLineIndented(std::format("Value: {} (0x{})", value.toString(10), value.toString(16)));
    addLineIndented(std::format("Token: {}", srcText(node.getRange())));
    _indent--;
}
void AstPrinterWalker::visitFloatLiteralExpr(const FloatLiteralExprNode& node) {
    addLineIndented("<FloatLiteralExprNode>");
    _indent++;
    addLineIndented(std::format("Value: {}", node.getValue()));
    addLineIndented(std::format("Token: {}", srcText(node.getRange())));
    _indent--;
}
void AstPrinterWalker::visitStringLiteralExpr(const StringLiteralExprNode& node) {
    addLineIndented("<StringLiteralExprNode>");
    _indent++;
    addLineIndented(std::format("Value: {}", node.getValue()));
    addLineIndented(std::format("Token: {}", srcText(node.getRange())));
    _indent--;
}
void AstPrinterWalker::visitBoolLiteralExpr(const BoolLiteralExprNode& node) {
    addLineIndented("<BoolLiteralExprNode>");
    _indent++;
    addLineIndented(std::format("Value: {}", node.getValue() ? "true" : "false"));
    addLineIndented(std::format("Token: {}", srcText(node.getRange())));
    _indent--;
}
void AstPrinterWalker::visitUnaryExpr(const UnaryExprNode& node) {
    addLineIndented("<UnaryExprNode>");
    _indent++;
    addLineIndented("Operand:");
    _indent++;
    walk(*node.getOperand());
    _indent -= 2;
}
void AstPrinterWalker::visitBinaryExpr(const BinaryExprNode& node) {
    addLineIndented("<BinaryExprNode>");
    _indent++;
    addLineIndented("Left:");
    _indent++;
    walk(*node.getLeft());
    _indent--;
    addLineIndented("Right:");
    _indent++;
    walk(*node.getRight());
    _indent -= 2;
}
void AstPrinterWalker::visitAssignmentExpr(const AssignmentExprNode& node) {
    addLineIndented("<AssignmentExprNode>");
    _indent++;
    addLineIndented("Left:");
    _indent++;
    walk(*node.getLeft());
    _indent--;
    addLineIndented("Right:");
    _indent++;
    walk(*node.getRight());
    _indent -= 2;
}
void AstPrinterWalker::visitNameExpr(const NameExprNode& node) {
    addLineIndented("<NameExprNode>");
    _indent++;
    addLineIndented("QualifiedName:");
    _indent++;
    walk(node.getQualifiedName());
    _indent -= 2;
}
void AstPrinterWalker::visitCallExpr(const CallExprNode& node) {
    addLineIndented("<CallExprNode>");
    _indent++;
    addLineIndented("Callee:");
    _indent++;
    walk(*node.getCallee());
    _indent--;
    addLineIndented("Arguments:");
    _indent++;
    const auto& args = node.getArgs();
    if (args.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& arg : args) {
        walk(*arg);
    }
    _indent -= 2;
}
void AstPrinterWalker::visitIndexExpr(const IndexExprNode& node) {
    addLineIndented("<IndexExprNode>");
    _indent++;
    addLineIndented("Object:");
    _indent++;
    walk(*node.getObject());
    _indent--;
    addLineIndented("Index:");
    _indent++;
    walk(*node.getIndex());
    _indent -= 2;
}
void AstPrinterWalker::visitMemberAccessExpr(const MemberAccessExprNode& node) {
    addLineIndented("<MemberAccessExprNode>");
    _indent++;
    addLineIndented(std::format("Operator: {}", memberAccessOpToString(node.getOperator())));
    addLineIndented("Object:");
    _indent++;
    walk(*node.getObject());
    _indent--;
    addLineIndented(std::format("Member: {}", poolText(node.getMember().id)));
    _indent--;
}
void AstPrinterWalker::visitConstructExpr(const ConstructExprNode& node) {
    addLineIndented("<ConstructExprNode>");
    _indent++;
    if (node.getType()) {
        addLineIndented("Type:");
        _indent++;
        walk(*node.getType());
        _indent--;
    }
    addLineIndented("Arguments:");
    _indent++;
    const auto& args = node.getArgs();
    if (args.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& arg : args) {
        walk(*arg);
    }
    _indent -= 2;
}
void AstPrinterWalker::visitModuleDecl(const ModuleDeclNode& node) {
    addLineIndented("<ModuleDeclNode>");
    _indent++;
    addLineIndented(std::format("Name: {}", poolText(node.getName().id)));
    addLineIndented("Items:");
    _indent++;
    const auto& items = node.getItems();
    if (items.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& item : items) {
        walk(*item);
    }
    _indent -= 2;
}
void AstPrinterWalker::visitFunctionDecl(const FunctionDeclNode& node) {
    addLineIndented("<FunctionDeclNode>");
    _indent++;
    std::string_view fnName = poolText(node.getName().id);
    addLineIndented(std::format("Name: {}", fnName));
    if (node.getReturnType()) {
        addLineIndented("Return Type:");
        _indent++;
        walk(*node.getReturnType());
        _indent--;
    }
    addLineIndented("Parameters:");
    _indent++;
    const auto& params = node.getParameters();
    if (params.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& param : params) {
        walk(*param);
    }
    _indent--;
    if (node.getBody()) {
        addLineIndented("Body:");
        _indent++;
        walk(*node.getBody());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitParameterDecl(const ParameterDeclNode& node) {
    addLineIndented("<ParameterDeclNode>");
    _indent++;
    addLineIndented(std::format("Name: {}", poolText(node.getName().id)));
    if (node.getType()) {
        addLineIndented("Type:");
        _indent++;
        walk(*node.getType());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitClassDecl(const ClassDeclNode& node) {
    addLineIndented("<ClassDeclNode>");
    _indent++;
    addLineIndented(std::format("Name: {}", poolText(node.getName().id)));
    addLineIndented("Items:");
    _indent++;
    const auto& items = node.getItems();
    if (items.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& item : items) {
        walk(*item);
    }
    _indent--;
}
void AstPrinterWalker::visitMethodDecl(const MethodDeclNode& node) {
    addLineIndented("<MethodDeclNode>");
    _indent++;
    addLineIndented(std::format("Name: {}", poolText(node.getName().id)));
    if (node.getReturnType()) {
        addLineIndented("Return Type:");
        _indent++;
        walk(*node.getReturnType());
        _indent--;
    }
    addLineIndented("Parameters:");
    _indent++;
    const auto& params = node.getParameters();
    if (params.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& param : params) {
        walk(*param);
    }
    _indent--;
    if (node.getBody()) {
        addLineIndented("Body:");
        _indent++;
        walk(*node.getBody());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitFieldDecl(const FieldDeclNode& node) {
    addLineIndented("<FieldDeclNode>");
    _indent++;
    addLineIndented(std::format("Name: {}", poolText(node.getName().id)));
    if (node.getType()) {
        addLineIndented("Type:");
        _indent++;
        walk(*node.getType());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitVariableDecl(const VariableDeclNode& node) {
    addLineIndented("<VariableDeclNode>");
    _indent++;
    addLineIndented(std::format("Name: {}", poolText(node.getName().id)));
    if (node.getType()) {
        addLineIndented("Type:");
        _indent++;
        walk(*node.getType());
        _indent--;
    }
    if (node.getInitializer()) {
        addLineIndented("Initializer:");
        _indent++;
        walk(*node.getInitializer());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitBlockStmt(const BlockStmtNode& node) {
    addLineIndented("<BlockStmtNode>");
    _indent++;
    addLineIndented("Items:");
    _indent++;
    const auto& items = node.getItems();
    if (items.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& child : items) {
        walk(*child);
    }
    _indent -= 2;
}
void AstPrinterWalker::visitExpressionStmt(const ExpressionStmtNode& node) {
    addLineIndented("<ExpressionStmtNode>");
    _indent++;
    if (node.getExpression()) {
        addLineIndented("Expression:");
        _indent++;
        walk(*node.getExpression());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitIfStmt(const IfStmtNode& node) {
    addLineIndented("<IfStmtNode>");
    _indent++;
    if (node.getCondition()) {
        addLineIndented("Condition:");
        _indent++;
        walk(*node.getCondition());
        _indent--;
    }
    if (node.getThen()) {
        addLineIndented("Then:");
        _indent++;
        walk(*node.getThen());
        _indent--;
    }
    if (node.getElse()) {
        addLineIndented("Else:");
        _indent++;
        walk(*node.getElse());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitLoopStmt(const LoopStmtNode& node) {
    addLineIndented("<LoopStmtNode>");
    _indent++;
    if (node.getBody()) {
        addLineIndented("Body:");
        _indent++;
        walk(*node.getBody());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitWhileStmt(const WhileStmtNode& node) {
    addLineIndented("<WhileStmtNode>");
    _indent++;
    if (node.getCondition()) {
        addLineIndented("Condition:");
        _indent++;
        walk(*node.getCondition());
        _indent--;
    }
    if (node.getBody()) {
        addLineIndented("Body:");
        _indent++;
        walk(*node.getBody());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitForStmt(const ForStmtNode& node) {
    addLineIndented("<ForStmtNode>");
    _indent++;
    if (node.getInit()) {
        addLineIndented("Init:");
        _indent++;
        walk(*node.getInit());
        _indent--;
    }
    if (node.getCondition()) {
        addLineIndented("Condition:");
        _indent++;
        walk(*node.getCondition());
        _indent--;
    }
    if (node.getIncrement()) {
        addLineIndented("Increment:");
        _indent++;
        walk(*node.getIncrement());
        _indent--;
    }
    if (node.getBody()) {
        addLineIndented("Body:");
        _indent++;
        walk(*node.getBody());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitReturnStmt(const ReturnStmtNode& node) {
    addLineIndented("<ReturnStmtNode>");
    _indent++;
    if (node.getValue()) {
        addLineIndented("Value:");
        _indent++;
        walk(*node.getValue());
        _indent--;
    }
    _indent--;
}
void AstPrinterWalker::visitUseStmt(const UseStmtNode& node) {
    addLineIndented("<UseStmtNode>");
    _indent++;
    addLineIndented("ModulePath:");
    _indent++;
    const auto& modulePath = node.getModulePath();
    if (modulePath.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& segment : modulePath) {
        addLineIndented(std::format("Segment: {}", poolText(segment.id)));
    }
    _indent--;
    addLineIndented("Imports:");
    _indent++;
    const auto& imports = node.getImports();
    if (imports.empty()) {
        addLineIndented("<Empty>");
    }
    else for (const auto& import : imports) {
        addLineIndented(std::format("Import: {}", poolText(import.id)));
    }
    _indent -= 2;
}
void AstPrinterWalker::visitBuiltinType(const BuiltinTypeNode& node) {
    addLineIndented("<BuiltinTypeNode>");
    _indent++;
    addLineIndented(std::format("Type: {}", builtinTypeKindToString(node.getBuiltinTypeKind())));
    _indent--;
}
void AstPrinterWalker::visitNamedType(const NamedTypeNode& node) {
    addLineIndented("<NamedTypeNode>");
    _indent++;
    addLineIndented("QualifiedName:");
    _indent++;
    walk(*node.getQualifiedName());
    _indent -= 2;
}

std::string AstPrinterWalker::indentStr() const {
    return std::string(_indent * 4, ' ');
}
void AstPrinterWalker::addIndented(const std::string& str) {
    _result += indentStr() + str;
}
void AstPrinterWalker::addLineIndented(const std::string& str) {
    _result += indentStr() + str + "\n";
}
std::string_view AstPrinterWalker::tokenText(const Token& token) const {
    return token.range().getText();
}
std::string_view AstPrinterWalker::srcText(const source::SourceRange& range) const {
    return range.getText();
}
std::string_view AstPrinterWalker::poolText(basic::StringId id) const {
    return _sp.get(id);
}

void AstPrinter::printNode(const AstNode& node, io::IWriter& writer) const {
    AstPrinterWalker walker(_sm, _sp);
    walker.walk(node);
    writer.write(walker.getResult());
    // TODO: ^^ Walker should write directly to the writer
}

} // namespace ast
VEEC_NAMESPACE_END
