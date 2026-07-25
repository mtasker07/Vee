#include "veec/ast_passes/TypeCheckerPass.hpp"

#include <span>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/CompilationContext.hpp"
#include "veec/basic/BigInt.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
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
#include "veec/ast/type/TypeNode.hpp"
#include "veec/ast/type/BuiltinTypeNode.hpp"
#include "veec/ast/type/NamedTypeNode.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/symbols/SymbolFwd.hpp"
#include "veec/symbols/OperatorTable.hpp"
#include "veec/symbols/ent/VariableSymbol.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/symbols/ent/FunctionSetSymbol.hpp"
#include "veec/symbols/ent/ClassSymbol.hpp"
#include "veec/symbols/ent/FieldSymbol.hpp"
#include "veec/symbols/ent/OperatorSymbol.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/TypeTable.hpp"
#include "veec/types/ErrorType.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/types/PointerType.hpp"
#include "veec/types/ArrayType.hpp"
#include "veec/types/ClassType.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace ast_passes {

void TypeCheckerPass::visitExpression(ast::ExpressionNode& node) {
    ast::AstWalker::visitExpression(node);
}
void TypeCheckerPass::visitParenthesizedExpr(ast::ParenthesizedExprNode& node) {
    ast::AstWalker::visitParenthesizedExpr(node);

    // Type is just type of inner expression
    types::Type* innerType = _sema.types.getNodeType(node.getInnerExpr());
    _sema.types.setNodeType(&node, innerType);
}
void TypeCheckerPass::visitIntLiteralExpr(ast::IntLiteralExprNode& node) {
    using BTK = types::BuiltinTypeKind;
    
    ast::AstWalker::visitIntLiteralExpr(node);

    // Integer literals are always i32
    // TODO: Support suffixes for different literal types
    types::BuiltinType* intType = _sema.types.getBuiltin(types::BuiltinTypeKind::I32);
    _sema.types.setNodeType(&node, intType);

    // Validate size of integer literal
    const basic::BigInt& value = node.getValue();

    u32 bitWidth = 0;
    bool isSigned = false;
    BTK builtinKind = intType->getBuiltinKind();
    switch (builtinKind) {
        case BTK::I8:
        case BTK::U8:
            bitWidth = 8;
            isSigned = (builtinKind == BTK::I8);
            break;
        case BTK::I16:
        case BTK::U16:
            bitWidth = 16;
            isSigned = (builtinKind == BTK::I16);
            break;
        case BTK::I32:
        case BTK::U32:
            bitWidth = 32;
            isSigned = (builtinKind == BTK::I32);
            break;
        case BTK::I64:
        case BTK::U64:
            bitWidth = 64;
            isSigned = (builtinKind == BTK::I64);
            break;
    }

    if (isSigned) {
        if (!value.fitsInSigned(bitWidth)) {
            _ctx.diagnostics.report(
                diagnostics::ERROR_INT_LITERAL_OUT_OF_RANGE,
                node.getRange(),
                value.toString(),
                bitWidth
            );
        }
    } else {
        if (!value.fitsInUnsigned(bitWidth)) {
            _ctx.diagnostics.report(
                diagnostics::ERROR_UINT_LITERAL_OUT_OF_RANGE,
                node.getRange(),
                value.toString(),
                bitWidth
            );
        }
    }
}
void TypeCheckerPass::visitFloatLiteralExpr(ast::FloatLiteralExprNode& node) {
    ast::AstWalker::visitFloatLiteralExpr(node);

    // Float literals are always f64
    // TODO: Support suffixes for different literal types
    types::Type* floatType = _sema.types.getBuiltin(types::BuiltinTypeKind::F64);
    _sema.types.setNodeType(&node, floatType);

    // TODO: Maybe check it fits in F64
}
void TypeCheckerPass::visitStringLiteralExpr(ast::StringLiteralExprNode& node) {
    ast::AstWalker::visitStringLiteralExpr(node);

    // String literals are always string
    types::Type* stringType = _sema.types.getBuiltin(types::BuiltinTypeKind::String);
    _sema.types.setNodeType(&node, stringType);
}
void TypeCheckerPass::visitBoolLiteralExpr(ast::BoolLiteralExprNode& node) {
    ast::AstWalker::visitBoolLiteralExpr(node);

    // Bool literals are always bool
    types::Type* boolType = _sema.types.getBuiltin(types::BuiltinTypeKind::Bool);
    _sema.types.setNodeType(&node, boolType);
}
void TypeCheckerPass::visitUnaryExpr(ast::UnaryExprNode& node) {
    ast::AstWalker::visitUnaryExpr(node);
    // ^^ Sets operand type

    types::ErrorType* errorType = _sema.types.getError();
    //types::Type* operandType = _sema.types.getNodeType(node.getOperand());
    types::Type* resultType = errorType;

    // Lookup operator
    symbols::UnaryOperatorKind semaOp = astToSemaUnaryOp(node.getOperator());
    std::span<symbols::OperatorSymbol* const> opSymbols = _sema.operators.getUnaryOperators(semaOp);

    VEE_ASSERT(resultType != nullptr, "Failed to infer type for unary expression");
    _sema.types.setNodeType(&node, resultType);
}
void TypeCheckerPass::visitBinaryExpr(ast::BinaryExprNode& node) {
    using BinOp = ast::BinaryOp;
    
    ast::AstWalker::visitBinaryExpr(node);
    // ^^ Infer types of lhs and rhs

    types::Type* lType = _sema.types.getNodeType(node.getLeft());
    types::Type* rType = _sema.types.getNodeType(node.getRight());
    VEE_ASSERT(lType != nullptr, "Failed to infer type for left-hand side of binary expression");
    VEE_ASSERT(rType != nullptr, "Failed to infer type for right-hand side of binary expression");

    types::ErrorType* errorType = _sema.types.getError();

    // Propogate errors
    if (lType == errorType || rType == errorType) {
        _sema.types.setNodeType(&node, errorType);
        return;
    }
}
void TypeCheckerPass::visitAssignmentExpr(ast::AssignmentExprNode& node) {
    ast::AstWalker::visitAssignmentExpr(node);

    types::Type* lType = _sema.types.getNodeType(node.getLeft());
    types::Type* rType = _sema.types.getNodeType(node.getRight());
    VEE_ASSERT(lType != nullptr, "Failed to infer type for left-hand side of assignment expression");
    VEE_ASSERT(rType != nullptr, "Failed to infer type for right-hand side of assignment expression");

    types::ErrorType* errorType = _sema.types.getError();

    // Propogate errors
    if (lType == errorType || rType == errorType) {
        _sema.types.setNodeType(&node, errorType);
        return;
    }

    // Assignment always has left-side type
    _sema.types.setNodeType(&node, lType);
}
void TypeCheckerPass::visitNameExpr(ast::NameExprNode& node) {
    ast::AstWalker::visitNameExpr(node);

    types::ErrorType* errorType = _sema.types.getError();

    // Type is just type of resolved symbol
    const symbols::Symbol* resolvedSymbol = node.getResolvedSymbol();
    if (resolvedSymbol) {
        types::Type* symbolType = errorType;

        if (resolvedSymbol->is<symbols::FunctionSetSymbol>()) {

        }

        switch (resolvedSymbol->getKind()) {
            case symbols::SymbolKind::Variable:
                symbolType = resolvedSymbol->as<symbols::VariableSymbol>()->getType();
                break;
            case symbols::SymbolKind::Function:
                // NOTE: Technically, this should never run, since we always bind
                // names to function set symbols. However, just in case this slips through
                // somewhere this logic is still reasonable and wont break anything.
                symbolType = resolvedSymbol->as<symbols::FunctionSymbol>()->getType();
                break;
            case symbols::SymbolKind::Field:
                symbolType = resolvedSymbol->as<symbols::FieldSymbol>()->getType();
                break;

            default:
                // Not a type-bearing symbol
                break;
        }
        _sema.types.setNodeType(&node, symbolType);
    } else {
        // Cannot infer unresolved symbol
        _sema.types.setNodeType(&node, errorType);
    }
}
void TypeCheckerPass::visitCallExpr(ast::CallExprNode& node) {
    ast::AstWalker::visitCallExpr(node);

    types::Type* calleeType = _sema.types.getNodeType(node.getCallee());

    types::ErrorType* errorType = _sema.types.getError();

    // Propogate errors
    if (calleeType == errorType) {
        _sema.types.setNodeType(&node, errorType);
        return;
    }

    types::FunctionType* calleeTypeAsFunction = calleeType->as<types::FunctionType>();
    if (calleeTypeAsFunction) {
        // Result type is the return type of the function
        types::Type* resultType = calleeTypeAsFunction->getReturnType();
        _sema.types.setNodeType(&node, resultType);
        return;
    }

    // Not a function (or object with function type)
    _sema.types.setNodeType(&node, errorType);
}
void TypeCheckerPass::visitIndexExpr(ast::IndexExprNode& node) {
    ast::AstWalker::visitIndexExpr(node);

    types::Type* objectType = _sema.types.getNodeType(node.getObject());
    types::Type* indexType = _sema.types.getNodeType(node.getIndex());

    types::ErrorType* errorType = _sema.types.getError();

    // Propogate errors
    if (objectType == errorType || indexType == errorType) {
        _sema.types.setNodeType(&node, errorType);
        return;
    }

    // For now, only arrays can be indexed
    // maybe in the future pointers or other types can be indexed too
    types::ArrayType* objectTypeAsArray = objectType->as<types::ArrayType>();
    if (objectTypeAsArray) {
        // Index must be an integer type
        types::BuiltinType* indexTypeAsBuiltin = indexType->as<types::BuiltinType>();
        if (indexTypeAsBuiltin && indexTypeAsBuiltin->isInteger()) {
            // Result type is the element type of the array
            types::Type* resultType = objectTypeAsArray->getElementType();
            _sema.types.setNodeType(&node, resultType);
            return;
        }
    }

    // TODO: Support index operator overloads (perhaps based on index type too?)
    _sema.types.setNodeType(&node, errorType);
}
void TypeCheckerPass::visitMemberAccessExpr(ast::MemberAccessExprNode& node) {
    ast::AstWalker::visitMemberAccessExpr(node);

    types::Type* objectType = _sema.types.getNodeType(node.getObject());
    
    types::ErrorType* errorType = _sema.types.getError();

    // Propogate errors
    if (objectType == errorType) {
        _sema.types.setNodeType(&node, errorType);
        return;
    }

    types::ClassType* objectTypeAsClass = objectType->as<types::ClassType>();
    if (objectTypeAsClass) {
        // Look up member
        types::Type* memberType = errorType;

        if (const auto* fieldSymbol = objectTypeAsClass->getField(node.getMember().id)) {
            memberType = fieldSymbol->getType();
        }
        else if (const auto* methodSymbols = objectTypeAsClass->getMethods(node.getMember().id)) {
            // TODO: Identify overload
            memberType = methodSymbols->front()->getType();
        }

        _sema.types.setNodeType(&node, memberType);
        return;
    }
}
void TypeCheckerPass::visitConstructExpr(ast::ConstructExprNode& node) {
    ast::AstWalker::visitConstructExpr(node);
    // ^^ Walks type and args

    types::Type* type = _sema.types.getNodeType(node.getType());
    
    types::ErrorType* errorType = _sema.types.getError();

    // Propogate errors
    if (type == errorType) {
        _sema.types.setNodeType(&node, errorType);
        return;
    }

    // Result type is the type being constructed
    _sema.types.setNodeType(&node, type);
}

bool TypeCheckerPass::checkTypeCompatibility(types::Type*, types::Type*) {
    return false;
}

symbols::UnaryOperatorKind TypeCheckerPass::astToSemaUnaryOp(ast::UnaryOp op) {
    using AstOp = ast::UnaryOp;
    using SemaOp = symbols::UnaryOperatorKind;
    
    switch (op) {
        case AstOp::Plus:
            return SemaOp::Plus;
        case AstOp::Minus:
            return SemaOp::Minus;
        case AstOp::Increment:
            return SemaOp::Increment;
        case AstOp::Decrement:
            return SemaOp::Decrement;
        case AstOp::LogicalNot:
            return SemaOp::LogicalNot;
        case AstOp::BitwiseNot:
            return SemaOp::BitwiseNot;
        case AstOp::Dereference:
            return SemaOp::Dereference;
        case AstOp::AddressOf:
            return SemaOp::AddressOf;
        
        default:
            VEE_UNREACHABLE("Unknown unary operator kind");    
    }
}
symbols::BinaryOperatorKind TypeCheckerPass::astToSemaBinaryOp(ast::BinaryOp op) {
    using AstOp = ast::BinaryOp;
    using SemaOp = symbols::BinaryOperatorKind;

    switch (op) {
        case AstOp::Add:
            return SemaOp::Add;
        case AstOp::Subtract:
            return SemaOp::Subtract;
        case AstOp::Multiply:
            return SemaOp::Multiply;
        case AstOp::Divide:
            return SemaOp::Divide;
        case AstOp::Modulo:
            return SemaOp::Modulo;
        case AstOp::Equal:
            return SemaOp::Equal;
        case AstOp::NotEqual:
            return SemaOp::NotEqual;
        case AstOp::LessThan:
            return SemaOp::LessThan;
        case AstOp::LessThanOrEqual:
            return SemaOp::LessThanOrEqual;
        case AstOp::GreaterThan:
            return SemaOp::GreaterThan;
        case AstOp::GreaterThanOrEqual:
            return SemaOp::GreaterThanOrEqual;
        case AstOp::LogicalAnd:
            return SemaOp::LogicalAnd;
        case AstOp::LogicalOr:
            return SemaOp::LogicalOr;

        default:
            VEE_UNREACHABLE("Unknown binary operator kind");
    }
}

symbols::OperatorSymbol* TypeCheckerPass::lookupUnaryOperator(symbols::UnaryOperatorKind kind, types::Type*) {
    std::span<symbols::OperatorSymbol* const> opSymbols = _sema.operators.getUnaryOperators(kind);
    std::vector<symbols::OperatorSymbol*> candidates;
    
    return nullptr; // TODO
}
symbols::OperatorSymbol* TypeCheckerPass::lookupBinaryOperator(symbols::BinaryOperatorKind kind, types::Type*, types::Type*) {
    std::span<symbols::OperatorSymbol* const> opSymbols = _sema.operators.getBinaryOperators(kind);

    return nullptr; // TODO
}

} // namespace ast_passes
VEEC_NAMESPACE_END
