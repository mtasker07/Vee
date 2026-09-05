#include "veec/codegen/c/CConstructGenerator.hpp"

#include <string>
#include <utility>
#include <vector>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/mir/MirFwd.hpp"
#include "veec/mir/MirKind.hpp"
#include "veec/mir/Module.hpp"
#include "veec/mir/Function.hpp"
#include "veec/mir/Argument.hpp"
#include "veec/mir/BasicBlock.hpp"
#include "veec/mir/Instruction.hpp"
#include "veec/mir/Value.hpp"
#include "veec/mir/Constant.hpp"
#include "veec/mir/MirType.hpp"
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

construct::CCompilationUnit CConstructGenerator::generateModule(const mir::Module& module) {
    // No need to reset _unit since moved every call
    for (const mir::Function* func : module.getFunctions()) {
        _unit.functions.push_back(generateFunction(*func));
    }

    return std::move(_unit);
}
construct::CCompilationUnit CConstructGenerator::generateModules(const std::vector<const mir::Module*>& modules) {
    // No need to reset _unit since moved every call
    for (const mir::Module* module : modules) {
        for (const mir::Function* func : module->getFunctions()) {
            _unit.functions.push_back(generateFunction(*func));
        }
    }

    return std::move(_unit);
}

construct::CFunction* CConstructGenerator::generateFunction(const mir::Function& func) {
    // Reset per-function state
    _valueNames.clear();
    _blockLabels.clear();
    _tempCounter = 0;
    _labelCounter = 0;

    construct::CFunction* cFunc = _cctx.constructArena.create<construct::CFunction>();

    cFunc->returnType = getType(func.getReturnType());
    cFunc->name = _ctx.codegen.nameMangler.mangleFunction(func);

    u32 paramCounter = 1;
    for (const mir::Argument* arg : func.getArgs()) {
        construct::CFunctionParam* cParam = _cctx.constructArena.create<construct::CFunctionParam>();
        cParam->name = "p" + std::to_string(paramCounter++);
        cParam->type = getType(arg->getType());
        cFunc->params.push_back(cParam);

        _valueNames[arg] = cParam->name;
    }

    construct::CBlockStmt* body = _cctx.constructArena.create<construct::CBlockStmt>();

    for (const mir::BasicBlock* block : func.getBlocks()) {
        for (const mir::Instruction* inst : block->getInstructions()) {
            if (!inst->producesValue() || inst->getType()->isVoid()) {
                continue;
            }

            construct::CDeclStmt* decl = _cctx.constructArena.create<construct::CDeclStmt>();
            decl->type = getType(inst->getType());
            decl->name = getValueName(inst);
            body->statements.push_back(decl);
        }
    }

    for (const mir::BasicBlock* block : func.getBlocks()) {
        generateBlock(*block, body->statements);
    }

    cFunc->body = body;

    return cFunc;
}
construct::CStruct* CConstructGenerator::generateStruct(const mir::MirStructType* type) {
    construct::CStruct* cStruct = _cctx.constructArena.create<construct::CStruct>();

    cStruct->name = _ctx.codegen.nameMangler.mangleType(type);

    u32 fieldCounter = 1;
    for (const mir::MirType* fieldTy : type->getMemberTypes()) {
        construct::CStructMember* member = _cctx.constructArena.create<construct::CStructMember>();
        member->name = "field_" + std::to_string(fieldCounter++);
        member->type = getType(fieldTy);
        cStruct->members.push_back(member);
    }

    return cStruct;
}
void CConstructGenerator::generateBlock(const mir::BasicBlock& block, std::vector<construct::CStmt*>& out) {
    const std::string& label = getBlockLabel(&block);

    construct::CStmt* firstStmt = nullptr;
    if (block.getInstructions().empty()) {
        firstStmt = _cctx.constructArena.create<construct::CEmptyStmt>();
    } else {
        firstStmt = generateStmt(*block.getInstruction(0));
    }

    construct::CLabelStmt* labeled = _cctx.constructArena.create<construct::CLabelStmt>();
    labeled->label = label;
    labeled->stmt = firstStmt;
    out.push_back(labeled);

    for (size_t i = 1; i < block.getInstructionCount(); ++i) {
        out.push_back(generateStmt(*block.getInstruction(i)));
    }
}
construct::CStmt* CConstructGenerator::generateStmt(const mir::Instruction& inst) {
    using Op = mir::InstructionOpcode;

    // Wraps assignment for instructions that produce a (non-void) value
    auto assign = [&](construct::CExpr* rhs) -> construct::CStmt* {
        construct::CAssignmentStmt* stmt = _cctx.constructArena.create<construct::CAssignmentStmt>();
        construct::CIdentifierExpr* lhs = _cctx.constructArena.create<construct::CIdentifierExpr>();
        lhs->name = getValueName(&inst);
        stmt->lhs = lhs;
        stmt->rhs = rhs;
        return stmt;
    };
    // Build binary expr from instruction's two operands
    auto binary = [&](construct::CBinaryOperator op) -> construct::CExpr* {
        construct::CBinaryExpr* expr = _cctx.constructArena.create<construct::CBinaryExpr>();
        expr->op = op;
        expr->lhs = generateValue(inst.getOperand(0));
        expr->rhs = generateValue(inst.getOperand(1));
        return expr;
    };
    // Build unary expr from instruction's single operand
    auto unary = [&](construct::CUnaryOperator op) -> construct::CExpr* {
        construct::CUnaryExpr* expr = _cctx.constructArena.create<construct::CUnaryExpr>();
        expr->op = op;
        expr->operand = generateValue(inst.getOperand(0));
        return expr;
    };
    // Single operand cast to instruction result type
    auto cast = [&]() -> construct::CExpr* {
        construct::CCastExpr* expr = _cctx.constructArena.create<construct::CCastExpr>();
        expr->targetType = getType(inst.getType());
        expr->operand = generateValue(inst.getOperand(0));
        return expr;
    };

    switch (inst.getOpcode()) {
        case Op::Nop:
            return _cctx.constructArena.create<construct::CEmptyStmt>();

        case Op::Phi:
            VEE_FATAL("C backend doesn't support Phi instructions yet");

        //
        // Integer arithmetic
        //
        case Op::Neg:
            return assign(unary(construct::CUnaryOperator::Neg));
        case Op::Add:
            return assign(binary(construct::CBinaryOperator::Add));
        case Op::Sub:
            return assign(binary(construct::CBinaryOperator::Sub));
        case Op::Mul:
            return assign(binary(construct::CBinaryOperator::Mul));
        case Op::SDiv:
        case Op::UDiv:
            return assign(binary(construct::CBinaryOperator::Div));
        case Op::SMod:
        case Op::UMod:
            return assign(binary(construct::CBinaryOperator::Mod));

        //
        // Bitwise
        //
        case Op::BitNot:
            return assign(unary(construct::CUnaryOperator::BitNot));
        case Op::BitAnd:
            return assign(binary(construct::CBinaryOperator::BitAnd));
        case Op::BitOr:
            return assign(binary(construct::CBinaryOperator::BitOr));
        case Op::BitXor:
            return assign(binary(construct::CBinaryOperator::BitXor));
        case Op::BitShl:
            return assign(binary(construct::CBinaryOperator::Shl));
        case Op::BitLShr:
        case Op::BitAShr:
            return assign(binary(construct::CBinaryOperator::Shr));

        //
        // Floating arithmetic
        //
        case Op::FNeg:
            return assign(unary(construct::CUnaryOperator::Neg));
        case Op::FAdd:
            return assign(binary(construct::CBinaryOperator::Add));
        case Op::FSub:
            return assign(binary(construct::CBinaryOperator::Sub));
        case Op::FMul:
            return assign(binary(construct::CBinaryOperator::Mul));
        case Op::FDiv:
            return assign(binary(construct::CBinaryOperator::Div));

        //
        // Logical
        //
        case Op::LogicalNot:
            return assign(unary(construct::CUnaryOperator::LogicalNot));
        case Op::LogicalAnd:
            return assign(binary(construct::CBinaryOperator::LogicalAnd));
        case Op::LogicalOr:
            return assign(binary(construct::CBinaryOperator::LogicalOr));

        //
        // Comparison (integer & floating share the same C operators; signedness is
        // already encoded in the operand's C type)
        //
        case Op::ICmpEq:
        case Op::FCmpEq:
            return assign(binary(construct::CBinaryOperator::Eq));
        case Op::ICmpNe:
        case Op::FCmpNe:
            return assign(binary(construct::CBinaryOperator::Ne));
        case Op::ICmpSlt:
        case Op::ICmpUlt:
        case Op::FCmpLt:
            return assign(binary(construct::CBinaryOperator::Lt));
        case Op::ICmpSle:
        case Op::ICmpUle:
        case Op::FCmpLe:
            return assign(binary(construct::CBinaryOperator::Le));
        case Op::ICmpSgt:
        case Op::ICmpUgt:
        case Op::FCmpGt:
            return assign(binary(construct::CBinaryOperator::Gt));
        case Op::ICmpSge:
        case Op::ICmpUge:
        case Op::FCmpGe:
            return assign(binary(construct::CBinaryOperator::Ge));

        //
        // Memory
        //
        case Op::Store: {
            construct::CAssignmentStmt* stmt = _cctx.constructArena.create<construct::CAssignmentStmt>();
            construct::CUnaryExpr* lhs = _cctx.constructArena.create<construct::CUnaryExpr>();
            lhs->op = construct::CUnaryOperator::Deref;
            lhs->operand = generateValue(inst.getOperand(0));
            stmt->lhs = lhs;
            stmt->rhs = generateValue(inst.getOperand(1));
            return stmt;
        }
        case Op::Load:
            return assign(unary(construct::CUnaryOperator::Deref));

        //
        // Data
        //
        case Op::Construct: {
            construct::CCompoundLiteralExpr* expr = _cctx.constructArena.create<construct::CCompoundLiteralExpr>();
            expr->type = getType(inst.getType());
            for (size_t i = 0; i < inst.getOperandCount(); ++i) {
                expr->values.push_back(generateValue(inst.getOperand(i)));
            }
            return assign(expr);
        }

        //
        // Conversion / casting
        //
        case Op::TruncateInt:
        case Op::ZeroExtendInt:
        case Op::SignExtendInt:
        case Op::TruncateFloat:
        case Op::ExtendFloat:
        case Op::IntToFloat:
        case Op::UIntToFloat:
        case Op::FloatToInt:
        case Op::FloatToUInt:
        case Op::PtrToInt:
        case Op::IntToPtr:
        case Op::Bitcast:
            return assign(cast());

        //
        // Control flow
        //
        case Op::Call: {
            const mir::Value* calleeValue = inst.getOperand(0);
            VEE_ASSERT(calleeValue->getNodeKind() == mir::MirKind::Function, "Call target must be a function!");
            const mir::Function* callee = static_cast<const mir::Function*>(calleeValue);

            construct::CIdentifierExpr* calleeExpr = _cctx.constructArena.create<construct::CIdentifierExpr>();
            calleeExpr->name = _ctx.codegen.nameMangler.mangleFunction(*callee);

            construct::CCallExpr* callExpr = _cctx.constructArena.create<construct::CCallExpr>();
            callExpr->callee = calleeExpr;
            for (size_t i = 1; i < inst.getOperandCount(); ++i) {
                callExpr->args.push_back(generateValue(inst.getOperand(i)));
            }

            if (inst.getType()->isVoid()) {
                construct::CExprStmt* stmt = _cctx.constructArena.create<construct::CExprStmt>();
                stmt->expr = callExpr;
                return stmt;
            }
            return assign(callExpr);
        }
        case Op::Ret: {
            construct::CReturnStmt* stmt = _cctx.constructArena.create<construct::CReturnStmt>();
            if (inst.getOperandCount() > 0) {
                stmt->value = generateValue(inst.getOperand(0));
            }
            return stmt;
        }
        case Op::Br: {
            const mir::Value* targetValue = inst.getOperand(0);
            VEE_ASSERT(targetValue->getNodeKind() == mir::MirKind::BasicBlock, "Br target must be a basic block!");

            construct::CGotoStmt* stmt = _cctx.constructArena.create<construct::CGotoStmt>();
            stmt->label = getBlockLabel(static_cast<const mir::BasicBlock*>(targetValue));
            return stmt;
        }
        case Op::CondBr: {
            const mir::Value* trueValue = inst.getOperand(1);
            const mir::Value* falseValue = inst.getOperand(2);
            VEE_ASSERT(trueValue->getNodeKind() == mir::MirKind::BasicBlock, "CondBr target must be a basic block!");
            VEE_ASSERT(falseValue->getNodeKind() == mir::MirKind::BasicBlock, "CondBr target must be a basic block!");

            construct::CGotoStmt* thenGoto = _cctx.constructArena.create<construct::CGotoStmt>();
            thenGoto->label = getBlockLabel(static_cast<const mir::BasicBlock*>(trueValue));

            construct::CGotoStmt* elseGoto = _cctx.constructArena.create<construct::CGotoStmt>();
            elseGoto->label = getBlockLabel(static_cast<const mir::BasicBlock*>(falseValue));

            construct::CIfStmt* stmt = _cctx.constructArena.create<construct::CIfStmt>();
            stmt->condition = generateValue(inst.getOperand(0));
            stmt->thenStmt = thenGoto;
            stmt->elseStmt = elseGoto;
            return stmt;
        }
        case Op::Unreachable:
            return _cctx.constructArena.create<construct::CEmptyStmt>();
    }

    VEE_UNREACHABLE("Unknown instruction opcode: {}", std::to_string(static_cast<int>(inst.getOpcode())));
}

construct::CExpr* CConstructGenerator::generateValue(const mir::Value* value) {
    switch (value->getNodeKind()) {
        case mir::MirKind::Constant:
            return generateConstant(static_cast<const mir::Constant*>(value));

        case mir::MirKind::Argument:
        case mir::MirKind::Instruction: {
            construct::CIdentifierExpr* expr = _cctx.constructArena.create<construct::CIdentifierExpr>();
            expr->name = getValueName(value);
            return expr;
        }

        case mir::MirKind::Function: {
            const mir::Function* func = static_cast<const mir::Function*>(value);
            construct::CIdentifierExpr* expr = _cctx.constructArena.create<construct::CIdentifierExpr>();
            expr->name = _ctx.codegen.nameMangler.mangleFunction(*func);
            return expr;
        }

        default:
            VEE_FATAL("Value kind cannot be used as a C expression operand");
    }
}
construct::CExpr* CConstructGenerator::generateConstant(const mir::Constant* constant) {
    if (const mir::ConstantInt* intConstant = constant->asInteger()) {
        construct::CIntLiteralExpr* expr = _cctx.constructArena.create<construct::CIntLiteralExpr>(
            intConstant->getValue(),
            constant->getType()->asInteger()->isSigned()
        );
        return expr;
    }
    if (const mir::ConstantFloat* floatConstant = constant->asFloat()) {
        construct::CFloatLiteralExpr* expr = _cctx.constructArena.create<construct::CFloatLiteralExpr>(
            floatConstant->getValue(),
            constant->getType()->asFloat()->getBitWidth() == 64
        );
        return expr;
    }
    if (const mir::ConstantBool* boolConstant = constant->asBool()) {
        construct::CBoolLiteralExpr* expr = _cctx.constructArena.create<construct::CBoolLiteralExpr>(
            boolConstant->getValue()
        );
        return expr;
    }
    if (const mir::ConstantString* stringConstant = constant->asString()) {
        construct::CStringLiteralExpr* expr = _cctx.constructArena.create<construct::CStringLiteralExpr>(
            std::string(stringConstant->getValue())
        );
        return expr;
    }

    VEE_FATAL("Unknown MIR constant kind");
}

const std::string& CConstructGenerator::getValueName(const mir::Value* value) {
    auto it = _valueNames.find(value);
    if (it != _valueNames.end()) {
        return it->second;
    }

    std::string name = "t" + std::to_string(_tempCounter++);
    return _valueNames.emplace(value, std::move(name)).first->second;
}
const std::string& CConstructGenerator::getBlockLabel(const mir::BasicBlock* block) {
    auto it = _blockLabels.find(block);
    if (it != _blockLabels.end()) {
        return it->second;
    }

    std::string label = "L" + std::to_string(_labelCounter++);
    return _blockLabels.emplace(block, std::move(label)).first->second;
}

construct::CType* CConstructGenerator::getType(const mir::MirType* type) {
    //
    // VOID
    //
    if (type->isVoid()) {
        return _cctx.constructArena.create<construct::CPrimitiveType>(
            construct::CPrimitiveTypeKind::Void
        );
    }
    //
    // BOOL
    //
    else if (type->isBool()) {
        return _cctx.constructArena.create<construct::CPrimitiveType>(
            construct::CPrimitiveTypeKind::Bool
        );
    }
    //
    // INTEGER
    //
    else if (auto* intType = type->asInteger()) {
        bool isSigned = intType->isSigned();
        switch (intType->getBitWidth()) {
            case 8:
                return _cctx.constructArena.create<construct::CPrimitiveType>(
                    isSigned
                        ? construct::CPrimitiveTypeKind::Char
                        : construct::CPrimitiveTypeKind::UnsignedChar
                );
            case 16:
                return _cctx.constructArena.create<construct::CPrimitiveType>(
                    isSigned
                        ? construct::CPrimitiveTypeKind::Short
                        : construct::CPrimitiveTypeKind::UnsignedShort
                );
            case 32:
                return _cctx.constructArena.create<construct::CPrimitiveType>(
                    isSigned
                        ? construct::CPrimitiveTypeKind::Int
                        : construct::CPrimitiveTypeKind::UnsignedInt
                );
            case 64:
                return _cctx.constructArena.create<construct::CPrimitiveType>(
                    isSigned
                        ? construct::CPrimitiveTypeKind::LongLong
                        : construct::CPrimitiveTypeKind::UnsignedLongLong
                );

            default:
                VEE_FATAL("C backend doesn't support integer bit width {}", intType->getBitWidth());
        }
    }
    //
    // FLOAT
    //
    else if (auto* floatType = type->asFloat()) {
        switch (floatType->getBitWidth()) {
            case 32:
                return _cctx.constructArena.create<construct::CPrimitiveType>(
                    construct::CPrimitiveTypeKind::Float
                );
            case 64:
                return _cctx.constructArena.create<construct::CPrimitiveType>(
                    construct::CPrimitiveTypeKind::Double
                );
            default:
                VEE_FATAL("C backend doesn't support float bit width {}", floatType->getBitWidth());
        }
    }
    //
    // POINTER
    //
    else if (auto* pointerType = type->asPointer()) {
        construct::CPointerType* cPtrTy = _cctx.constructArena.create<construct::CPointerType>();
        cPtrTy->pointeeType = getType(pointerType->getPointeeType());
        return cPtrTy;
    }
    //
    // FUNCTION
    //
    else if (auto* functionType = type->asFunction()) {
        construct::CFunctionType* cFuncTy = _cctx.constructArena.create<construct::CFunctionType>();
        cFuncTy->returnType = getType(functionType->getReturnType());
        for (auto* param : functionType->getParameterTypes()) {
            cFuncTy->paramTypes.push_back(getType(param));
        }
        return cFuncTy;
    }
    //
    // STRUCT
    //
    else if (auto* structType = type->asStruct()) {
        construct::CStruct* cStruct;
        auto it = _structMap.find(structType);
        if (it != _structMap.end()) {
            cStruct = it->second;
        } else {
            cStruct = generateStruct(structType);
            _structMap[structType] = cStruct;
            _unit.structs.push_back(cStruct);
        }

        construct::CStructType* cStructTy = _cctx.constructArena.create<construct::CStructType>();
        cStructTy->kind = construct::CStructTypeKind::ReferenceToDefined;
        cStructTy->referenceToDefined = cStruct;
        return cStructTy;
    }
    VEE_FATAL("C backend doesn't support type {}", type->toString());
}

} // namespace c
} // namespace codegen
VEEC_NAMESPACE_END
