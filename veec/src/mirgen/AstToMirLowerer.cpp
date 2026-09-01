#include "veec/mirgen/AstToMirLowerer.hpp"

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/BigInt.hpp"
#include "veec/basic/APInt.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/CompilationUnitNode.hpp"
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
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/ParameterDeclNode.hpp"
#include "veec/ast/decl/VariableDeclNode.hpp"
#include "veec/ast/stmt/StatementNode.hpp"
#include "veec/ast/stmt/BlockStmtNode.hpp"
#include "veec/ast/stmt/ExpressionStmtNode.hpp"
#include "veec/ast/stmt/IfStmtNode.hpp"
#include "veec/ast/stmt/LoopStmtNode.hpp"
#include "veec/ast/stmt/WhileStmtNode.hpp"
#include "veec/ast/stmt/ForStmtNode.hpp"
#include "veec/ast/stmt/ReturnStmtNode.hpp"
#include "veec/ast/type/TypeNode.hpp"
#include "veec/mir/Module.hpp"
#include "veec/mir/Function.hpp"
#include "veec/mir/BasicBlock.hpp"
#include "veec/mir/Value.hpp"
#include "veec/mir/Constant.hpp"
#include "veec/mir/MirType.hpp"
#include "veec/mirgen/support/FunctionCollector.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/types/TypeContext.hpp"
#include "veec/types/TypeTable.hpp"

VEEC_NAMESPACE_BEGIN
namespace mirgen {

mir::Module* AstToMirLowerer::lower(ast::CompilationUnitNode& node) {
    walk(node);
    VEE_ASSERT(_currentModule != nullptr, "Module not generated!");
    return _currentModule;
}

// Entry point vv

void AstToMirLowerer::lowerAllFunctions(const ast::AstNode& node) {
    // Collect all function declarations
    support::FunctionCollector collector;
    std::vector<const ast::FunctionDeclNode*> functionDecls = collector.collectFunctions(node);

    // Lower declarations
    for (const ast::FunctionDeclNode* funcDecl : functionDecls) {
        lowerFunctionDecl(*funcDecl);
    }

    // Lower bodies
    for (const ast::FunctionDeclNode* funcDecl : functionDecls) {
        lowerFunctionBody(*funcDecl);
    }
}

//
// Basic
//

void AstToMirLowerer::visitCompilationUnit(const ast::CompilationUnitNode& node) {
    // Create new module
    _currentModule = _mir.factory.createModule();

    // Lower all functions in the unit
    lowerAllFunctions(node);
}

//
// Expressions
//

void AstToMirLowerer::visitParenthesizedExpr(const ast::ParenthesizedExprNode& node) {
    // Parenthesized expressions are just a syntactic concept, we can
    // essentially remove them here entirely by just setting the last value
    // to whatever is inside
    _lastValue = lowerExpression(*node.getInnerExpr());
}
void AstToMirLowerer::visitIntLiteralExpr(const ast::IntLiteralExprNode& node) {
    // Create new constant for integer literal
    const mir::MirType* mirType = _typeConverter.convert(_ctx.types.table.getNodeType(&node));
    VEE_ASSERT(mirType->isInteger(),
        "Type for integer literal is not an integer type");

    u32 bitWidth = mirType->getBitWidth();
    
    // TODO: Use better conversion, string is slow and inefficient
    basic::APInt value = basic::APInt::fromString(bitWidth, node.getValue().toString(), 10);
    
    _lastValue = _mir.factory.getConstantInt(_currentModule, mirType, value, "const_int");
}
void AstToMirLowerer::visitFloatLiteralExpr(const ast::FloatLiteralExprNode& node) {
    // Create new constant for float literal
    const mir::MirType* mirType = _typeConverter.convert(_ctx.types.table.getNodeType(&node));
    VEE_ASSERT(mirType->isFloat(),
        "Type for float literal is not a float type");

    double value = node.getValue();

    _lastValue = _mir.factory.getConstantFloat(_currentModule, mirType, value, "const_float");
}
void AstToMirLowerer::visitStringLiteralExpr(const ast::StringLiteralExprNode& node) {
    // Create new constant for string literal
    const mir::MirType* mirType = _typeConverter.convert(_ctx.types.table.getNodeType(&node));
    VEE_ASSERT(mirType->isStruct(),
        "Type for string literal is not a string type");

    std::string_view value = node.getValue();

    _lastValue = _mir.factory.getConstantString(_currentModule, mirType, value, "const_string");
}
void AstToMirLowerer::visitBoolLiteralExpr(const ast::BoolLiteralExprNode& node) {
    // Create new constant for bool literal
    const mir::MirType* mirType = _typeConverter.convert(_ctx.types.table.getNodeType(&node));
    VEE_ASSERT(mirType->isBool(),
        "Type for bool literal is not a bool type");

    bool value = node.getValue();

    _lastValue = _mir.factory.getConstantBool(_currentModule, mirType, value, "const_bool");
}
void AstToMirLowerer::visitUnaryExpr(const ast::UnaryExprNode& node) {
    mir::Value* operandValue = lowerExpression(*node.getOperand());

    // Identify conversions
    symbols::OperatorSymbol* opSymbol = node.symbol;
    VEE_ASSERT(opSymbol != nullptr, "Unresolved unary expression");

    const mir::MirType* operandMirType = operandValue->getType();
    VEE_ASSERT(operandMirType != nullptr,
        "Unary expression operand has null type");

    const mir::MirType* desiredOperandMirType = _typeConverter.convert(opSymbol->getOperandType(0));
    VEE_ASSERT(desiredOperandMirType != nullptr,
        "Unary expression operator has null operand type");

    operandValue = handleAnyConversion(operandValue, desiredOperandMirType);
    VEE_ASSERT(operandValue->getType() == desiredOperandMirType,
        "Unary expression operand conversion failed");

    operandMirType = desiredOperandMirType;

    if (operandMirType->isInteger()) {
        u32 bitWidth = operandMirType->getBitWidth();

        //
        // Integer operations
        //

        switch (node.getOperator()) {
            case ast::UnaryOp::Plus:
                // Plus is purely syntactic, since values are default-positive
                // TODO: Make unary + an abs() like operation
                setLoweredValue(operandValue);
                return;
            case ast::UnaryOp::Minus:
                setLoweredValue(_builder.createNeg(operandValue));
                return;
            case ast::UnaryOp::Increment: {
                basic::APInt one = basic::APInt::one(bitWidth);
                mir::ConstantInt* oneValue = _mir.factory.getConstantInt(
                    _currentModule,
                    operandMirType,
                    one,
                    "const_one"
                );
                setLoweredValue(_builder.createAdd(operandValue, oneValue));
                return;
            }
            case ast::UnaryOp::Decrement: {
                basic::APInt one = basic::APInt::one(bitWidth);
                mir::ConstantInt* oneValue = _mir.factory.getConstantInt(
                    _currentModule,
                    operandMirType,
                    one,
                    "const_one"
                );
                setLoweredValue(_builder.createSub(operandValue, oneValue));
                return;
            }
            case ast::UnaryOp::BitwiseNot:
                setLoweredValue(_builder.createBitNot(operandValue));
                return;

            default:
                VEE_UNREACHABLE("Unknown unary operator for integer");
        }
    }

    //
    // Float operations
    //

    else if (operandMirType->isFloat()) {
        switch (node.getOperator()) {
            case ast::UnaryOp::Plus:
                // vvv See integer plus, same reasoning
                setLoweredValue(operandValue);
                return;
            case ast::UnaryOp::Minus:
                setLoweredValue(_builder.createFNeg(operandValue));
                return;
            case ast::UnaryOp::Increment: {
                mir::ConstantFloat* oneValue = _mir.factory.getConstantFloat(
                    _currentModule,
                    operandMirType,
                    1.0,
                    "const_one"
                );
                setLoweredValue(_builder.createFAdd(operandValue, oneValue));
                return;
            }
            case ast::UnaryOp::Decrement: {
                mir::ConstantFloat* oneValue = _mir.factory.getConstantFloat(
                    _currentModule,
                    operandMirType,
                    1.0,
                    "const_one"
                );
                setLoweredValue(_builder.createFSub(operandValue, oneValue));
                return;
            }

            default:
                VEE_UNREACHABLE("Unknown unary operator for float");
                return;
        }
    }

    //
    // Bool operations
    //

    else if (operandMirType->isBool()) {
        switch (node.getOperator()) {
            case ast::UnaryOp::LogicalNot:
                setLoweredValue(_builder.createLogicalNot(operandValue));
                return;

            default:
                VEE_UNREACHABLE("Unknown unary operator for bool");
                return;
        }
    }

    //
    // Pointer operations
    //

    else if (operandMirType->isPointer()) {
        switch (node.getOperator()) {
            case ast::UnaryOp::Dereference:
                setLoweredValue(_builder.createLoad(operandValue));
                return;

            default:
                VEE_UNREACHABLE("Unknown unary operator for pointer");
                return;
        }
    }

    VEE_UNREACHABLE("Unknown unary operator");
}
void AstToMirLowerer::visitBinaryExpr(const ast::BinaryExprNode& node) {
    mir::Value* lhsValue = lowerExpression(*node.getLeft());
    mir::Value* rhsValue = lowerExpression(*node.getRight());

    // Identify conversions
    symbols::OperatorSymbol* opSymbol = node.symbol;
    VEE_ASSERT(opSymbol != nullptr, "Unresolved binary expression");

    const mir::MirType* lhsType = lhsValue->getType();
    const mir::MirType* rhsType = rhsValue->getType();
    VEE_ASSERT(lhsType != nullptr && rhsType != nullptr,
        "Binary expression operands have null types");

    const mir::MirType* desiredLhsType = _typeConverter.convert(opSymbol->getOperandType(0));
    const mir::MirType* desiredRhsType = _typeConverter.convert(opSymbol->getOperandType(1));
    VEE_ASSERT(desiredLhsType != nullptr && desiredRhsType != nullptr,
        "Binary expression operator has null operand types");

    lhsValue = handleAnyConversion(lhsValue, desiredLhsType);
    rhsValue = handleAnyConversion(rhsValue, desiredRhsType);

    lhsType = lhsValue->getType();
    rhsType = rhsValue->getType();

    VEE_ASSERT(lhsType == rhsType,
        "Binary expression operands have mismatched types after conversion");

    if (lhsType->isInteger()) {
        bool isSigned = lhsType->isSigned();

        //
        // Integer operations
        //

        switch (node.getOperator()) {
            case ast::BinaryOp::Add:
                setLoweredValue(_builder.createAdd(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::Subtract:
                setLoweredValue(_builder.createSub(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::Multiply:
                setLoweredValue(_builder.createMul(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::Divide: {
                // TODO: Handle signedness within builder
                if (isSigned) {
                    setLoweredValue(_builder.createSDiv(lhsValue, rhsValue));
                } else {
                    setLoweredValue(_builder.createUDiv(lhsValue, rhsValue));
                }
                return;
            }
            case ast::BinaryOp::Modulo: {
                if (isSigned) {
                    setLoweredValue(_builder.createSMod(lhsValue, rhsValue));
                } else {
                    setLoweredValue(_builder.createUMod(lhsValue, rhsValue));
                }
                return;
            }
            case ast::BinaryOp::Equal:
                setLoweredValue(_builder.createICmpEq(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::NotEqual:
                setLoweredValue(_builder.createICmpNe(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::LessThan: {
                if (isSigned) {
                    setLoweredValue(_builder.createICmpSlt(lhsValue, rhsValue));
                } else {
                    setLoweredValue(_builder.createICmpUlt(lhsValue, rhsValue));
                }
                return;
            }
            case ast::BinaryOp::LessThanOrEqual: {
                if (isSigned) {
                    setLoweredValue(_builder.createICmpSle(lhsValue, rhsValue));
                } else {
                    setLoweredValue(_builder.createICmpUle(lhsValue, rhsValue));
                }
                return;
            }
            case ast::BinaryOp::GreaterThan:
                if (isSigned) {
                    setLoweredValue(_builder.createICmpSgt(lhsValue, rhsValue));
                } else {
                    setLoweredValue(_builder.createICmpUgt(lhsValue, rhsValue));
                }
                return;
            case ast::BinaryOp::GreaterThanOrEqual:
                if (isSigned) {
                    setLoweredValue(_builder.createICmpSge(lhsValue, rhsValue));
                } else {
                    setLoweredValue(_builder.createICmpUge(lhsValue, rhsValue));
                }
                return;

            default:
                VEE_UNREACHABLE("Unknown binary operator for integer");
        }
    }

    //
    // Float operations
    //

    else if (lhsType->isFloat()) {
        switch (node.getOperator()) {
            case ast::BinaryOp::Add:
                setLoweredValue(_builder.createFAdd(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::Subtract:
                setLoweredValue(_builder.createFSub(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::Multiply:
                setLoweredValue(_builder.createFMul(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::Divide:
                setLoweredValue(_builder.createFDiv(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::Equal:
                setLoweredValue(_builder.createFCmpEq(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::NotEqual:
                setLoweredValue(_builder.createFCmpNe(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::LessThan:
                setLoweredValue(_builder.createFCmpLt(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::LessThanOrEqual:
                setLoweredValue(_builder.createFCmpLe(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::GreaterThan:
                setLoweredValue(_builder.createFCmpGt(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::GreaterThanOrEqual:
                setLoweredValue(_builder.createFCmpGe(lhsValue, rhsValue));
                return;

            default:
                VEE_UNREACHABLE("Unknown binary operator for float");    
                return;
        }
    }

    //
    // Bool operations
    //

    else if (lhsType->isBool()) {
        switch (node.getOperator()) {
            case ast::BinaryOp::Equal:
                setLoweredValue(_builder.createICmpEq(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::NotEqual:
                setLoweredValue(_builder.createICmpNe(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::LogicalAnd:
                setLoweredValue(_builder.createLogicalAnd(lhsValue, rhsValue));
                return;
            case ast::BinaryOp::LogicalOr:
                setLoweredValue(_builder.createLogicalOr(lhsValue, rhsValue));
                return;

            default:
                VEE_UNREACHABLE("Unknown binary operator for bool");    
                return;
        }
    }

    VEE_UNREACHABLE("Unknown binary operator");
}
void AstToMirLowerer::visitAssignmentExpr(const ast::AssignmentExprNode& node) {
    // Lower expression
    mir::Value* rhsValue = lowerExpression(*node.getRight());

    // Assign to variable
    symbols::VariableSymbol* assigneeSymbol = node.assigneeSymbol;
    VEE_ASSERT(assigneeSymbol != nullptr, "Assignee symbol is null for assignment expression");

    _variableMap[assigneeSymbol] = rhsValue;
}
void AstToMirLowerer::visitNameExpr(const ast::NameExprNode& node) {
    const symbols::Symbol* resolvedSymbol = node.getResolvedSymbol();
    VEE_ASSERT(resolvedSymbol != nullptr, "Name expression has no resolved symbol");

    if (const symbols::VariableSymbol* varSymbol = resolvedSymbol->as<const symbols::VariableSymbol>()) {
        // Variable
        auto it = _variableMap.find(varSymbol);
        VEE_ASSERT(it != _variableMap.end(), "Variable symbol not found in variable map");
        setLoweredValue(it->second);
    } else if (const symbols::FunctionSymbol* funcSymbol = resolvedSymbol->as<const symbols::FunctionSymbol>()) {
        // Function
        auto it = _functionMap.find(funcSymbol);
        VEE_ASSERT(it != _functionMap.end(), "Function symbol not found in function map");
        setLoweredValue(it->second);
    } else {
        VEE_UNREACHABLE("Name expression resolved to unknown symbol type");
    }
}
void AstToMirLowerer::visitCallExpr(const ast::CallExprNode& node) {
    // Get function
    symbols::FunctionSymbol* funcSymbol = node.symbol;
    VEE_ASSERT(funcSymbol != nullptr, "Function symbol is null for call expression");

    // Lower args
    const std::vector<ast::ExpressionNode*>& argNodes = node.getArgs();
    VEE_ASSERT(argNodes.size() == funcSymbol->getParameterCount(),
        "Argument count does not match parameter count");
    
    basic::SmallVector<mir::Value*, 2> argValues;
    for (size_t i = 0; i < argNodes.size(); ++i) {
        mir::Value* argValue = lowerExpression(*argNodes[i]);
        const mir::MirType* paramMirType = _typeConverter.convert(funcSymbol->getParameter(i)->getType());
        argValues.push_back(handleAnyConversion(argValue, paramMirType));
    }

    mir::Function* func = _functionMap[funcSymbol];
    VEE_ASSERT(func != nullptr, "Function not found in function map for call expression");

    // Create call
    setLoweredValue(_builder.createCall(func, argValues));
}
void AstToMirLowerer::visitIndexExpr(const ast::IndexExprNode&) {
    // TODO
}
void AstToMirLowerer::visitMemberAccessExpr(const ast::MemberAccessExprNode&) {
    // TODO
}
void AstToMirLowerer::visitConstructExpr(const ast::ConstructExprNode& node) {
    const mir::MirType* mirConstructType = _typeConverter.convert(_ctx.types.table.getNodeType(node.getType()));

    basic::SmallVector<mir::Value*, 2> argValues;
    for (const auto& arg : node.getArgs()) {
        argValues.push_back(lowerExpression(*arg));
    }

    setLoweredValue(_builder.createConstruct(mirConstructType, std::move(argValues)));
}

//
// Declarations
//

void AstToMirLowerer::visitFunctionDecl(const ast::FunctionDeclNode&) {
    VEE_UNREACHABLE("Function declarations should be lowered in lowerFunctionDecl()"
                    "and lowerFunctionBody() directly from the compilation unit");
}
void AstToMirLowerer::visitParameterDecl(const ast::ParameterDeclNode&) {
    VEE_UNREACHABLE("Parameter declarations should be not be lowered directly,"
                    "they are lowered as part of creating the mir::Function");
}
void AstToMirLowerer::visitVariableDecl(const ast::VariableDeclNode& node) {
    VEE_ASSERT(_currentFunction != nullptr, "Current function is null!");
    symbols::VariableSymbol* varSymbol = node.symbol;
    VEE_ASSERT(varSymbol != nullptr, "Variable symbol is null!");

    if (node.getInitializer()) {
        mir::Value* value = lowerExpression(*node.getInitializer());
        value = handleAnyConversion(value, _typeConverter.convert(varSymbol->getType()));
        _variableMap[varSymbol] = value;
    }
}

//
// Statements
//

void AstToMirLowerer::visitBlockStmt(const ast::BlockStmtNode& node) {
    beginBlock(node);

    ast::ConstAstWalker::visitBlockStmt(node);

    finishBlock();
}
void AstToMirLowerer::visitExpressionStmt(const ast::ExpressionStmtNode& node) {
    lowerExpression(*node.getExpression());
}
void AstToMirLowerer::visitIfStmt(const ast::IfStmtNode& node) {
    // Lower the condition expression
    mir::Value* conditionValue = lowerExpression(*node.getCondition());

    bool hasElse = node.getElse() != nullptr;

    // We need to create the blocks here, so we can use them as targets for the branch
    mir::BasicBlock* thenBlock = _mir.factory.createBasicBlock(_currentFunction, "if_then");
    mir::BasicBlock* elseBlock = hasElse ? _mir.factory.createBasicBlock(_currentFunction, "if_else") : nullptr;
    mir::BasicBlock* mergeBlock = _mir.factory.createBasicBlock(_currentFunction, "if_merge");

    // Create branch
    if (hasElse) {
        _builder.createConditionalBranch(conditionValue, thenBlock, elseBlock);
    } else {
        _builder.createConditionalBranch(conditionValue, thenBlock, mergeBlock);
    }

    // Then
    _builder.setInsertBlock(thenBlock);
    walk(*node.getThen());

    // Else
    if (node.getElse() != nullptr) {
        _builder.setInsertBlock(elseBlock);
        walk(*node.getElse());
    }

    // Merge
    _builder.setInsertBlock(mergeBlock);
}
void AstToMirLowerer::visitLoopStmt(const ast::LoopStmtNode& node) {
    // Create blocks
    mir::BasicBlock* bodyBlock = _mir.factory.createBasicBlock(_currentFunction, "loop_body");
    mir::BasicBlock* mergeBlock = _mir.factory.createBasicBlock(_currentFunction, "loop_merge");

    // Create branch to body
    _builder.createBranch(bodyBlock);

    // Body
    _builder.setInsertBlock(bodyBlock);
    walk(*node.getBody());

    // Branch back to loop header
    _builder.createBranch(bodyBlock);

    // Merge
    _builder.setInsertBlock(mergeBlock);
}
void AstToMirLowerer::visitWhileStmt(const ast::WhileStmtNode& node) {
    // Create blocks
    mir::BasicBlock* conditionBlock = _mir.factory.createBasicBlock(_currentFunction, "while_condition");
    mir::BasicBlock* bodyBlock = _mir.factory.createBasicBlock(_currentFunction, "while_body");
    mir::BasicBlock* mergeBlock = _mir.factory.createBasicBlock(_currentFunction, "while_merge");

    // Create branch to condition
    _builder.createBranch(conditionBlock);

    // Condition
    _builder.setInsertBlock(conditionBlock);
    mir::Value* conditionValue = lowerExpression(*node.getCondition());
    _builder.createConditionalBranch(conditionValue, bodyBlock, mergeBlock);

    // Body
    _builder.setInsertBlock(bodyBlock);
    walk(*node.getBody());

    // Branch back to condition
    _builder.createBranch(conditionBlock);

    // Merge
    _builder.setInsertBlock(mergeBlock);
}
void AstToMirLowerer::visitForStmt(const ast::ForStmtNode& node) {
    // Create blocks
    mir::BasicBlock* initBlock = _mir.factory.createBasicBlock(_currentFunction, "for_init");
    mir::BasicBlock* conditionBlock = _mir.factory.createBasicBlock(_currentFunction, "for_condition");
    mir::BasicBlock* bodyBlock = _mir.factory.createBasicBlock(_currentFunction, "for_body");
    mir::BasicBlock* incrementBlock = _mir.factory.createBasicBlock(_currentFunction, "for_increment");
    mir::BasicBlock* mergeBlock = _mir.factory.createBasicBlock(_currentFunction, "for_merge");

    // Create branch to init
    _builder.createBranch(initBlock);

    // Init
    _builder.setInsertBlock(initBlock);
    if (node.getInit() != nullptr) {
        walk(*node.getInit());
    }
    _builder.createBranch(conditionBlock);

    // Condition
    _builder.setInsertBlock(conditionBlock);
    if (node.getCondition() != nullptr) {
        mir::Value* conditionValue = lowerExpression(*node.getCondition());
        _builder.createConditionalBranch(conditionValue, bodyBlock, mergeBlock);
    } else {
        // If no condition is provided, we assume it's always true
        _builder.createBranch(bodyBlock);
    }

    // Body
    _builder.setInsertBlock(bodyBlock);
    walk(*node.getBody());
    _builder.createBranch(incrementBlock);

    // Increment
    _builder.setInsertBlock(incrementBlock);
    if (node.getIncrement() != nullptr) {
        lowerExpression(*node.getIncrement());
    }
    _builder.createBranch(conditionBlock);

    // Merge
    _builder.setInsertBlock(mergeBlock);
}
void AstToMirLowerer::visitReturnStmt(const ast::ReturnStmtNode& node) {
    if (node.getValue() != nullptr) {
        mir::Value* returnValue = lowerExpression(*node.getValue());
        _builder.createRet(returnValue);
    } else {
        _builder.createRetVoid();
    }
}

void AstToMirLowerer::lowerFunctionDecl(const ast::FunctionDeclNode& node) {
    symbols::FunctionSymbol* funcSymbol = node.symbol;
    VEE_ASSERT(funcSymbol != nullptr, "Function symbol is null!");
    
    // Create new function & add to map
    std::string_view funcName = _ctx.strings.get(funcSymbol->getNameValue());

    // Convert param types
    basic::SmallVector<const mir::MirType*> paramTypes;
    for (size_t i = 0; i < funcSymbol->getParameterCount(); ++i) {
        const mir::MirType* mirParamType = _typeConverter.convert(
            funcSymbol->getParameter(i)->getType()
        );
        paramTypes.push_back(mirParamType);
    }
    // Convert return type
    const mir::MirType* returnType = _typeConverter.convert(funcSymbol->getReturnType());

    // Get function type
    const mir::MirFunctionType* funcType = _mir.types.getFunction(
        returnType,
        paramTypes
    );

    mir::Function* func = _mir.factory.createFunction(_currentModule, funcType, funcName);
    _functionMap[funcSymbol] = func;
    _currentFunction = func;

    // Create parameters
    for (size_t i = 0; i < funcSymbol->getParameterCount(); ++i) {
        symbols::VariableSymbol* paramSymbol = funcSymbol->getParameter(i);
        // Name
        std::string_view paramName = _ctx.strings.get(paramSymbol->getNameValue());
        // Type
        const mir::MirType* paramMirType = func->getParameterTypes()[i];
        _mir.factory.createArgument(func, paramMirType, paramName);
        // DONT add to variable map yet (just declaring)
    }

    // Create entry block
    mir::BasicBlock* entry = _mir.factory.createBasicBlock(_currentFunction, "entry");
    _builder.setInsertBlock(entry);
}
void AstToMirLowerer::lowerFunctionBody(const ast::FunctionDeclNode& node) {
	symbols::FunctionSymbol* funcSymbol = node.symbol;
	VEE_ASSERT(funcSymbol != nullptr, "Function symbol is null!");
    
    // Get mir function
    mir::Function* func = _functionMap[funcSymbol];
    VEE_ASSERT(func != nullptr, "Function not found in function map for function body!");
    _currentFunction = func;

    // Map params -> values
    VEE_ASSERT(funcSymbol->getParameterCount() == func->getArgCount(),
        "Function parameter count mismatch between symbol and mir function");
        
    _variableMap.clear();
    for (size_t i = 0; i < funcSymbol->getParameterCount(); ++i) {
		symbols::VariableSymbol* paramSymbol = funcSymbol->getParameter(i);
        mir::Value* paramValue = func->getArg(i);
		_variableMap[paramSymbol] = paramValue;
    }

    // Initial entry block
    _builder.setInsertBlock(func->getEntryBlock());

    // Lower the body
    if (node.getBody() != nullptr) {
        walk(*node.getBody());
    }
}
void AstToMirLowerer::beginBlock(const ast::BlockStmtNode&) {
    // TODO: Call constructors, etc
}
void AstToMirLowerer::finishBlock() {
    // TODO: Call destructors, etc
}

void AstToMirLowerer::setLoweredValue(mir::Value* value) {
    _lastValue = value;
}
mir::Value* AstToMirLowerer::lowerExpression(const ast::ExpressionNode& node) {
    // Reset last value
    _lastValue = nullptr;

    // Walk the expression node
    walk(node);

    // Return the last value produced by the expression
    VEE_ASSERT(_lastValue != nullptr, "Expression did not produce a value!");
    return _lastValue;
}
std::vector<mir::Value*> AstToMirLowerer::lowerExpressionList(const std::vector<ast::ExpressionNode*>& nodes) {
    std::vector<mir::Value*> values;
    values.reserve(nodes.size());
    for (const ast::ExpressionNode* node : nodes) {
        values.push_back(lowerExpression(*node));
    }
    return values;
}

//
// Conversions / casts
//

std::vector<mir::Value*> AstToMirLowerer::lowerExpressionListWithConversions(
    const std::vector<mir::Value*>& values,
    const std::vector<const mir::MirType*>& toTypes
) {
    VEE_ASSERT(values.size() == toTypes.size(),
        "Mismatched sizes for values and types");

    std::vector<mir::Value*> convertedValues;
    convertedValues.reserve(values.size());
    for (size_t i = 0; i < values.size(); ++i) {
        convertedValues.push_back(handleAnyConversion(values[i], toTypes[i]));
    }
    return convertedValues;
}
void AstToMirLowerer::validateConversion(const mir::MirType* from, const mir::MirType* to) {
    // Internal validation
    // Since this is where we actually perform the conversion, we dont trust the conversion
    // table here, we validate the conversion ourselves to ensure that it is valid.
    // Also, we dont emit any diagnostics here, this should be checked beforehand, here
    // we just assert.

    if (from == to) {
        // No conversion needed
        return;
    }

    // Check if the conversion is valid
    if (from->isInteger() && to->isInteger()) {
        // Integer to integer conversion is always valid
        return;
    }
    if (from->isFloat() && to->isFloat()) {
        // Float to float conversion is always valid
        return;
    }
    if (from->isInteger() && to->isFloat()) {
        // Integer to float conversion is always valid
        return;
    }
    if (from->isFloat() && to->isInteger()) {
        // Float to integer conversion is always valid
        return;
    }

    // TODO: Add more conversion rules here
    VEE_FATAL("Unsupported MIR type conversion");
}
mir::Value* AstToMirLowerer::handleAnyConversion(mir::Value* value, const mir::MirType* to) {
    const mir::MirType* from = value->getType();
    validateConversion(from, to);

    // No conversion needed
    if (from == to) {
        return value;
    }

    // Lower the conversion
    return lowerConversion(value, to);
}
mir::Value* AstToMirLowerer::lowerConversion(mir::Value* value, const mir::MirType* to) {
    const mir::MirType* from = value->getType();
    validateConversion(from, to);

    if (from == to) {
        return value;
    }

    // Builtin type conversions
    if (from != nullptr && to != nullptr) {
        // Get bit widths
        u32 fromBitWidth = from->getBitWidth();
        u32 toBitWidth = to->getBitWidth();

        // Signedness?
        bool fromSigned = from->isSigned();
        bool toSigned = to->isSigned();

        auto assertSameSignedness = [&]() {
            VEE_ASSERT(fromSigned == toSigned, "Signedness mismatch in conversion");
        };

        // Int -> int
        if (from->isInteger() && to->isInteger()) {
            if (fromBitWidth < toBitWidth) {
                // Widening conversion
                assertSameSignedness();
                if (toSigned) {
                    return _builder.createSExtendInt(value, to);
                } else {
                    return _builder.createZExtendInt(value, to);
                }
            } else if (fromBitWidth > toBitWidth) {
                // Narrowing conversion
                return _builder.createTruncateInt(value, to);
            } else {
                // Same bit width, no conversion needed
                return value;
            }
        }
        // Float -> float
        if (from->isFloat() && to->isFloat()) {
            if (fromBitWidth < toBitWidth) {
                // Widening conversion
                return _builder.createExtendFloat(value, to);
            } else if (fromBitWidth > toBitWidth) {
                // Narrowing conversion
                return _builder.createTruncateFloat(value, to);
            } else {
                // Same bit width, no conversion needed
                return value;
            }
        }
        // Int -> float
        if (from->isInteger() && to->isFloat()) {
            // Integer to float conversion is always valid
            return _builder.createIntToFloat(value, to);
        }
        // Float -> int
        if (from->isFloat() && to->isInteger()) {
            // Float to integer conversion is always valid
            return _builder.createFloatToInt(value, to);
        }
    }

    VEE_UNREACHABLE("MIR type conversion is not implemented");
}

} // namespace mirgen
VEEC_NAMESPACE_END
