#include "veec/ast_passes/TypeCheckerPass.hpp"

#include <span>
#include <vector>
#include <algorithm>
#include <iostream>

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
#include "veec/ast/decl/VariableDeclNode.hpp"
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
#include "veec/types/TypeContext.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/TypeTable.hpp"
#include "veec/types/TypeSystem.hpp"
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
    types::Type* innerType = _ctx.types.table.getNodeType(node.getInnerExpr());
    _ctx.types.table.setNodeType(&node, innerType);
}
void TypeCheckerPass::visitIntLiteralExpr(ast::IntLiteralExprNode& node) {
    using BTK = types::BuiltinTypeKind;
    
    ast::AstWalker::visitIntLiteralExpr(node);

    // Integer literals are always i32
    // TODO: Support suffixes for different literal types
    types::BuiltinType* intType = _ctx.types.table.getBuiltin(types::BuiltinTypeKind::I32);
    _ctx.types.table.setNodeType(&node, intType);

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
    types::Type* floatType = _ctx.types.table.getBuiltin(types::BuiltinTypeKind::F64);
    _ctx.types.table.setNodeType(&node, floatType);

    // TODO: Maybe check it fits in F64
}
void TypeCheckerPass::visitStringLiteralExpr(ast::StringLiteralExprNode& node) {
    ast::AstWalker::visitStringLiteralExpr(node);

    // String literals are always string
    types::Type* stringType = _ctx.types.table.getBuiltin(types::BuiltinTypeKind::String);
    _ctx.types.table.setNodeType(&node, stringType);
}
void TypeCheckerPass::visitBoolLiteralExpr(ast::BoolLiteralExprNode& node) {
    ast::AstWalker::visitBoolLiteralExpr(node);

    // Bool literals are always bool
    types::Type* boolType = _ctx.types.table.getBuiltin(types::BuiltinTypeKind::Bool);
    _ctx.types.table.setNodeType(&node, boolType);
}
void TypeCheckerPass::visitUnaryExpr(ast::UnaryExprNode& node) {
    ast::AstWalker::visitUnaryExpr(node);
    // ^^ Sets operand type

    types::ErrorType* errorType = _ctx.types.table.getError();
    types::Type* operandType = _ctx.types.table.getNodeType(node.getOperand());
    VEE_ASSERT(operandType != nullptr, "Failed to infer type for operand of unary expression");

    // Propogate errors early
    if (operandType == errorType) {
        _ctx.types.table.setNodeType(&node, errorType);
        return;
    }
    
    types::Type* resultType = errorType;

    // Lookup operator
    symbols::UnaryOperatorKind semaOp = astToSemaUnaryOp(node.getOperator());
    std::vector<symbols::OperatorSymbol*> result = lookupUnaryOperator(semaOp, operandType);

    if (result.empty()) {
        // No matching operator found
        _ctx.diagnostics.report(
            diagnostics::ERROR_UNARY_OPERATOR_NOT_FOUND,
            node.getRange(),
            ast::toString(node.getOperator()),
            operandType->toString()
        );
    } else if (result.size() > 1) {
        // Ambiguous operator
        _ctx.diagnostics.report(
            diagnostics::ERROR_UNARY_OPERATOR_AMBIGUOUS,
            node.getRange(),
            ast::toString(node.getOperator()),
            operandType->toString()
        );
    } else {
        // Found exactly one matching operator
        symbols::OperatorSymbol* opSymbol = result[0];
        node.symbol = opSymbol;
        resultType = opSymbol->getResultType();

        // Emit conversion diagnostics
        emitImplicitConversionDiagnostics(
            node.getRange(),
            operandType,
            opSymbol->getOperandType(0)
        );
    }

    _ctx.types.table.setNodeType(&node, resultType);
}
void TypeCheckerPass::visitBinaryExpr(ast::BinaryExprNode& node) {
    using BinOp = ast::BinaryOp;
    
    ast::AstWalker::visitBinaryExpr(node);
    // ^^ Infer types of lhs and rhs

    types::ErrorType* errorType = _ctx.types.table.getError();
    types::Type* lType = _ctx.types.table.getNodeType(node.getLeft());
    types::Type* rType = _ctx.types.table.getNodeType(node.getRight());
    VEE_ASSERT(lType != nullptr, "Failed to infer type for left-hand side of binary expression");
    VEE_ASSERT(rType != nullptr, "Failed to infer type for right-hand side of binary expression");

    // Propogate errors early
    if (lType == errorType || rType == errorType) {
        _ctx.types.table.setNodeType(&node, errorType);
        return;
    }
    
    types::Type* resultType = errorType;

    // Lookup operator
    symbols::BinaryOperatorKind semaOp = astToSemaBinaryOp(node.getOperator());
    std::vector<symbols::OperatorSymbol*> result = lookupBinaryOperator(semaOp, lType, rType);

    if (result.empty()) {
        // No matching operator found
        _ctx.diagnostics.report(
            diagnostics::ERROR_BINARY_OPERATOR_NOT_FOUND,
            node.getRange(),
            ast::toString(node.getOperator()),
            lType->toString(),
            rType->toString()
        );
    } else if (result.size() > 1) {
        // Ambiguous operator
        _ctx.diagnostics.report(
            diagnostics::ERROR_BINARY_OPERATOR_AMBIGUOUS,
            node.getRange(),
            ast::toString(node.getOperator()),
            lType->toString(),
            rType->toString()
        );
    } else {
        // Found exactly one matching operator
        symbols::OperatorSymbol* opSymbol = result[0];
        node.symbol = opSymbol;
        resultType = opSymbol->getResultType();

        // Emit conversion diagnostics
        emitImplicitConversionDiagnostics(
            node.getRange(),
            lType,
            opSymbol->getOperandType(0)
        );
        emitImplicitConversionDiagnostics(
            node.getRange(),
            rType,
            opSymbol->getOperandType(1)
        );
    }

    _ctx.types.table.setNodeType(&node, resultType);
}
void TypeCheckerPass::visitAssignmentExpr(ast::AssignmentExprNode& node) {
    ast::AstWalker::visitAssignmentExpr(node);

    types::Type* lType = _ctx.types.table.getNodeType(node.getLeft());
    types::Type* rType = _ctx.types.table.getNodeType(node.getRight());
    VEE_ASSERT(lType != nullptr, "Failed to infer type for left-hand side of assignment expression");
    VEE_ASSERT(rType != nullptr, "Failed to infer type for right-hand side of assignment expression");

    types::ErrorType* errorType = _ctx.types.table.getError();

    // Propogate errors
    if (lType == errorType || rType == errorType) {
        _ctx.types.table.setNodeType(&node, errorType);
        return;
    }

    // Assignment always has left-side type
    _ctx.types.table.setNodeType(&node, lType);
}
void TypeCheckerPass::visitNameExpr(ast::NameExprNode& node) {
    ast::AstWalker::visitNameExpr(node);

    types::ErrorType* errorType = _ctx.types.table.getError();

    // Type is just type of resolved symbol
    const symbols::Symbol* resolvedSymbol = node.getResolvedSymbol();
    if (resolvedSymbol) {
        types::Type* symbolType = errorType;

        // Function sets are a special case, since we dont actually know which overload
        // is being called in the current context. This is the one case where we can dont
        // set the type of the symbol (keep it null), and we will instead rely on CallExpr to set it
        if (auto* funcSet = resolvedSymbol->as<symbols::FunctionSetSymbol>()) {
            return;
        }
        // Not a function set -> standard symbol resolution
        else switch (resolvedSymbol->getKind()) {
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
        _ctx.types.table.setNodeType(&node, symbolType);
    } else {
        // Cannot infer unresolved symbol
        _ctx.types.table.setNodeType(&node, errorType);
    }
}
void TypeCheckerPass::visitCallExpr(ast::CallExprNode& node) {
    ast::AstWalker::visitCallExpr(node);

    types::ErrorType* errorType = _ctx.types.table.getError();

    // Propogate errors
    types::Type* calleeType = _ctx.types.table.getNodeType(node.getCallee());
    if (calleeType == errorType) {
        // ^^ Note that this wont break if the callee is a function set since it
        // will actually be nullptr instead
        _ctx.types.table.setNodeType(&node, errorType);
        return;
    }

    // Handle function sets specially, since we can only decide which overload here when
    // we have argument context
    ast::NameExprNode* calleeNameExpr = ast::ast_cast<ast::NameExprNode>(node.getCallee());
    if (calleeNameExpr) {
        symbols::Symbol* resolvedSymbol = calleeNameExpr->getResolvedSymbol();
        if (!resolvedSymbol) {
            // Cannot infer unresolved symbol
            _ctx.types.table.setNodeType(&node, errorType);
            return;
        }

        symbols::FunctionSetSymbol* funcSet = resolvedSymbol->as<symbols::FunctionSetSymbol>();
        if (!funcSet) {
            // Cannot call non-function set symbols
            _ctx.diagnostics.report(
                diagnostics::ERROR_CALL_NON_FUNCTION,
                node.getRange(),
                _ctx.strings.get(resolvedSymbol->getNameValue())
            );

            _ctx.types.table.setNodeType(&node, errorType);
            return;
        }

        // Grab argument types
        std::vector<types::Type*> argTypes;
        for (ast::ExpressionNode* arg : node.getArgs()) {
            types::Type* argType = _ctx.types.table.getNodeType(arg);
            VEE_ASSERT(argType != nullptr, "Failed to infer type for argument of call expression");
            argTypes.push_back(argType);
        }

        // Lookup overloads
        std::vector<symbols::FunctionSymbol*> overloads = lookupOverload(funcSet, argTypes);

        if (overloads.empty()) {
            // No matching overload found
            _ctx.diagnostics.report(
                diagnostics::ERROR_FUNCTION_OVERLOAD_NOT_FOUND,
                node.getRange(),
                _ctx.strings.get(funcSet->getNameValue())
            );

            _ctx.types.table.setNodeType(calleeNameExpr, errorType);
            _ctx.types.table.setNodeType(&node, errorType);
            return;
        } else if (overloads.size() > 1) {
            // Ambiguous overload
            _ctx.diagnostics.report(
                diagnostics::ERROR_FUNCTION_OVERLOAD_AMBIGUOUS,
                node.getRange(),
                _ctx.strings.get(funcSet->getNameValue())
            );

            _ctx.types.table.setNodeType(calleeNameExpr, errorType);
            _ctx.types.table.setNodeType(&node, errorType);
            return;
        } else {
            // Found exactly one matching overload
            // IMPORTANT: Use function type for name expression node,
            // and return type for call expression node
            symbols::FunctionSymbol* funcSymbol = overloads[0];
            types::FunctionType* funcType = funcSymbol->getType()->as<types::FunctionType>();
            VEE_ASSERT(funcType != nullptr, "Function symbol does not have a function type");
            
            _ctx.types.table.setNodeType(calleeNameExpr, funcType);
            _ctx.types.table.setNodeType(&node, funcType->getReturnType());

            // Output conversion diagnostics for args
            const std::vector<types::Type*>& paramTypes = funcType->getParameterTypes();
            for (size_t i = 0; i < argTypes.size(); ++i) {
                types::Type* argType = argTypes[i];
                types::Type* paramType = paramTypes[i];

                emitImplicitConversionDiagnostics(
                    node.getArgs()[i]->getRange(),
                    argType,
                    paramType
                );
            }

            return;
        }
    }

    // Any function-typed object
    types::FunctionType* calleeTypeAsFunction = calleeType->as<types::FunctionType>();
    if (calleeTypeAsFunction) {
        // Result type is the return type of the function
        types::Type* resultType = calleeTypeAsFunction->getReturnType();
        _ctx.types.table.setNodeType(&node, resultType);
        return;
    }

    // Not a function (or object with function type)
    _ctx.types.table.setNodeType(&node, errorType);
}
void TypeCheckerPass::visitIndexExpr(ast::IndexExprNode& node) {
    ast::AstWalker::visitIndexExpr(node);

    types::Type* objectType = _ctx.types.table.getNodeType(node.getObject());
    types::Type* indexType = _ctx.types.table.getNodeType(node.getIndex());

    types::ErrorType* errorType = _ctx.types.table.getError();

    // Propogate errors
    if (objectType == errorType || indexType == errorType) {
        _ctx.types.table.setNodeType(&node, errorType);
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
            _ctx.types.table.setNodeType(&node, resultType);
            return;
        }
    }

    // TODO: Support index operator overloads (perhaps based on index type too?)
    _ctx.types.table.setNodeType(&node, errorType);
}
void TypeCheckerPass::visitMemberAccessExpr(ast::MemberAccessExprNode& node) {
    ast::AstWalker::visitMemberAccessExpr(node);

    types::Type* objectType = _ctx.types.table.getNodeType(node.getObject());
    VEE_ASSERT(objectType != nullptr, "Failed to infer type for object of member access expression");
    
    types::ErrorType* errorType = _ctx.types.table.getError();

    // Propogate errors
    if (objectType == errorType) {
        _ctx.types.table.setNodeType(&node, errorType);
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

        _ctx.types.table.setNodeType(&node, memberType);
        return;
    }
}
void TypeCheckerPass::visitConstructExpr(ast::ConstructExprNode& node) {
    ast::AstWalker::visitConstructExpr(node);
    // ^^ Walks type and args

    types::Type* type = _ctx.types.table.getNodeType(node.getType());
    
    types::ErrorType* errorType = _ctx.types.table.getError();

    // Propogate errors
    if (type == errorType) {
        _ctx.types.table.setNodeType(&node, errorType);
        return;
    }

    // Result type is the type being constructed
    _ctx.types.table.setNodeType(&node, type);
}

void TypeCheckerPass::visitVariableDecl(ast::VariableDeclNode& node) {
    ast::AstWalker::visitVariableDecl(node);
    // ^^ Walks type and initializer

    types::ErrorType* errorType = _ctx.types.table.getError();

    // NOTE: We dont want to use setNodeType here, since its a declaration,
    // instead, only set the type of the symbol vv
    symbols::VariableSymbol* varSymbol = node.symbol;

    // If we have no initializer we dont need to do any checks
    // UNLESS the variable has no explicit type
    if (node.getInitializer() == nullptr) {
        if (varSymbol->getType() == nullptr) {
            // Cannot infer type for variable without type or initializer
            _ctx.diagnostics.report(
                diagnostics::ERROR_VAR_DECL_NO_TYPE_OR_INIT,
                node.getRange(),
                _ctx.strings.get(node.getName().id)
            );

            varSymbol->setType(_ctx.types.table.getError());
        }
        return;
    }

    types::Type* initType = _ctx.types.table.getNodeType(node.getInitializer());
    VEE_ASSERT(initType != nullptr, "Failed to infer type for initializer of variable declaration");
    
    // Attempt to infer type if not explicitly specified
    if (varSymbol->getType() == nullptr) {
        varSymbol->setType(initType);

        // If we inferred an error type, return early to avoid further errors
        if (initType == errorType) {
            return;
        }
    }

    // Validate initializer can be assigned to variable
    if (!implicitConversionPossible(initType, varSymbol->getType())) {
        // Cannot convert initializer type to variable type
        _ctx.diagnostics.report(
            diagnostics::ERROR_VAR_ASSIGNMENT_TYPE_MISMATCH,
            node.getInitializer()->getRange(),
            initType->toString(),
            varSymbol->getType()->toString()
        );
    }

    // Emit conversion diagnostics for initializer
    emitImplicitConversionDiagnostics(
        node.getInitializer()->getRange(),
        initType,
        varSymbol->getType()
    );
}

bool TypeCheckerPass::implicitConversionPossible(types::Type* from, types::Type* to) {
    return _ctx.types.system.canConvert(from, to, types::ConversionMode::Implicit);
}
u32 TypeCheckerPass::implicitConversionCost(types::Type* from, types::Type* to) {
    return _ctx.types.system.conversionCost(from, to, types::ConversionMode::Implicit);
}

template<typename T, typename CostFn>
std::vector<T*> TypeCheckerPass::findBestCandidates(std::span<T* const> candidates, CostFn&& costFn) {
    std::vector<T*> results;

    u32 lowestCost = types::kNoConversionCost;
    for (const auto& candidate : candidates) {
        u32 cost = costFn(candidate);

        if (cost < lowestCost) {
            lowestCost = cost;
            results.clear();
            results.push_back(candidate);
        }
        else if (cost == lowestCost) {
            results.push_back(candidate);
        }
    }

    return results;
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

std::vector<symbols::OperatorSymbol*> TypeCheckerPass::lookupUnaryOperator(symbols::UnaryOperatorKind kind, types::Type* operandType) {    
    VEE_ASSERT(operandType != nullptr, "Operand type must not be null");

    // Early out if error type is passed
    if (operandType == _ctx.types.table.getError()) {
        return {};
    }
    
    std::span<symbols::OperatorSymbol* const> candidates = _ctx.sema.operators.getUnaryOperators(kind);

    return findBestCandidates(candidates, [&](symbols::OperatorSymbol* opSymbol) {
        types::Type* opType = opSymbol->getOperandType(0);
        return implicitConversionCost(operandType, opType);
    });
}
std::vector<symbols::OperatorSymbol*> TypeCheckerPass::lookupBinaryOperator(symbols::BinaryOperatorKind kind, types::Type* leftType, types::Type* rightType) {
    VEE_ASSERT(leftType != nullptr, "Left operand type must not be null");
    VEE_ASSERT(rightType != nullptr, "Right operand type must not be null");
    
    // Early out if error type is passed
    types::Type* errorType = _ctx.types.table.getError();
    if (leftType == errorType || rightType == errorType) {
        return {};
    }
    
    std::span<symbols::OperatorSymbol* const> candidates = _ctx.sema.operators.getBinaryOperators(kind);

    return findBestCandidates(candidates, [&](symbols::OperatorSymbol* opSymbol) {
        // Left cost
        types::Type* leftOpType = opSymbol->getOperandType(0);
        u32 leftCost = implicitConversionCost(leftType, leftOpType);
        if (leftCost == types::kNoConversionCost)
            return types::kNoConversionCost;

        // Right cost
        types::Type* rightOpType = opSymbol->getOperandType(1);
        u32 rightCost = implicitConversionCost(rightType, rightOpType);
        if (rightCost == types::kNoConversionCost)
            return types::kNoConversionCost;

        // Combined cost is sum of left and right costs
        return leftCost + rightCost;
    });
}
std::vector<symbols::FunctionSymbol*> TypeCheckerPass::lookupOverload(symbols::FunctionSetSymbol* set, const std::vector<types::Type*>& argTypes) {
    VEE_ASSERT(set != nullptr, "Function set symbol must not be null");
    for (const auto* argType : argTypes) {
        VEE_ASSERT(argType != nullptr, "Argument type must not be null");
    }

    // Early out if error type is passed
    types::Type* errorType = _ctx.types.table.getError();
    if (std::any_of(argTypes.begin(), argTypes.end(), [&](types::Type* t) { return t == errorType; })) {
        return {};
    }

    std::span<symbols::FunctionSymbol* const> candidates = set->getOverloads();

    return findBestCandidates(candidates, [&](symbols::FunctionSymbol* funcSymbol) {
        types::FunctionType* funcType = funcSymbol->getType()->as<types::FunctionType>();
        VEE_ASSERT(funcType != nullptr, "Function symbol must have a function type");

        const auto& paramTypes = funcType->getParameterTypes();
        if (paramTypes.size() != argTypes.size()) {
            return types::kNoConversionCost;
        }

        u32 totalCost = 0;
        for (size_t i = 0; i < paramTypes.size(); ++i) {
            u32 cost = implicitConversionCost(argTypes[i], paramTypes[i]);
            if (cost == types::kNoConversionCost) {
                return types::kNoConversionCost;
            }
            totalCost += cost;
        }

        return totalCost;
    });
}

void TypeCheckerPass::emitImplicitConversionDiagnostics(
    const source::SourceRange& range,
    types::Type* fromType,
    types::Type* toType
) {
    using ConversionRank = types::ConversionRank;

    VEE_ASSERT(fromType != nullptr, "From type must not be null");
    VEE_ASSERT(toType != nullptr, "To type must not be null");

    // Early out no conversion
    if (fromType == toType) {
        return;
    }

    ConversionRank rank = _ctx.types.system.rankConversion(fromType, toType, types::ConversionMode::Implicit);
    switch (rank) {
        case ConversionRank::ExactMatch:
        case ConversionRank::Promotion:
        case ConversionRank::Conversion:
        case ConversionRank::UserDefinedConversion:
        case ConversionRank::ExplicitConversion:
            // No diagnostics for these ranks
            break;
        
        case ConversionRank::NarrowingConversion:
            _ctx.diagnostics.report(
                diagnostics::WARNING_NARROWING_CONVERSION,
                range,
                fromType->toString(),
                toType->toString()
            );
            break;

        case ConversionRank::NoConversion:
            _ctx.diagnostics.report(
                diagnostics::ERROR_NO_IMPLICIT_CONVERSION_AVAILABLE,
                range,
                fromType->toString(),
                toType->toString()
            );
            break;

        default:
            VEE_UNREACHABLE("Unknown conversion rank");
    }
}

} // namespace ast_passes
VEEC_NAMESPACE_END
