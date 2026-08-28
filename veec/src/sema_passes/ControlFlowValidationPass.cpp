#include "veec/sema_passes/ControlFlowValidationPass.hpp"

#include <stack>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/ast/AstNode.hpp"
#include "veec/ast/AstFwd.hpp"
#include "veec/ast/AstWalker.hpp"
#include "veec/ast/expr/ExpressionNode.hpp"
#include "veec/ast/decl/DeclarationNode.hpp"
#include "veec/ast/decl/FunctionDeclNode.hpp"
#include "veec/ast/decl/MethodDeclNode.hpp"
#include "veec/ast/stmt/BlockStmtNode.hpp"
#include "veec/ast/stmt/ExpressionStmtNode.hpp"
#include "veec/ast/stmt/IfStmtNode.hpp"
#include "veec/ast/stmt/LoopStmtNode.hpp"
#include "veec/ast/stmt/WhileStmtNode.hpp"
#include "veec/ast/stmt/ForStmtNode.hpp"
#include "veec/ast/stmt/ReturnStmtNode.hpp"
#include "veec/sema/Pass.hpp"
#include "veec/sema/SemaContext.hpp"
#include "veec/sema/FlowInfo.hpp"
#include "veec/symbols/Symbol.hpp"
#include "veec/symbols/ent/FunctionSymbol.hpp"
#include "veec/types/TypeFwd.hpp"
#include "veec/types/Type.hpp"
#include "veec/types/BuiltinType.hpp"
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace sema_passes {

namespace {

class ControlFlowAnalyzer : public ast::AstWalker {
public:
    ControlFlowAnalyzer() = default;
    virtual ~ControlFlowAnalyzer() = default;

    sema::FlowInfo analyze(ast::AstNode& node) {
        _flowInfoStack.push(sema::FlowInfo{});
        walk(node);
        sema::FlowInfo result = _flowInfoStack.top();
        _flowInfoStack.pop();
        return result;
    }

private:
    std::stack<sema::FlowInfo> _flowInfoStack;

    sema::FlowInfo& currentFlowInfo() {
        return _flowInfoStack.top();
    }

    virtual void visitExpression(ast::ExpressionNode& node) override;
    virtual void visitCallExpr(ast::CallExprNode& node) override;
    virtual void visitBlockStmt(ast::BlockStmtNode& node) override;
    virtual void visitExpressionStmt(ast::ExpressionStmtNode& node) override;
    virtual void visitIfStmt(ast::IfStmtNode& node) override;
    virtual void visitLoopStmt(ast::LoopStmtNode& node) override;
    virtual void visitWhileStmt(ast::WhileStmtNode& node) override;
    virtual void visitForStmt(ast::ForStmtNode& node) override;
    virtual void visitReturnStmt(ast::ReturnStmtNode& node) override;
};

void ControlFlowAnalyzer::visitExpression(ast::ExpressionNode&) {
    // Expressions do not affect control flow
}
void ControlFlowAnalyzer::visitCallExpr(ast::CallExprNode&) {
    // When we have a CFG, we can analyze the function to see if it
    // exits or not for example in panic() or exit().
}
void ControlFlowAnalyzer::visitBlockStmt(ast::BlockStmtNode& node) {
    sema::FlowInfo& flow = currentFlowInfo();

    for (ast::ItemNode* item : node.getItems()) {
        sema::FlowInfo itemFlow = analyze(*item);

        flow.canReturn |= itemFlow.canReturn;
        flow.canBreak |= itemFlow.canBreak;
        flow.canContinue |= itemFlow.canContinue;

        flow.canFallThrough = itemFlow.canFallThrough;

        if (!flow.canFallThrough) {
			break; // TODO: Report unreachable after this point
        }
    }
}
void ControlFlowAnalyzer::visitExpressionStmt(ast::ExpressionStmtNode& node) {
    sema::FlowInfo& flow = currentFlowInfo();

    sema::FlowInfo exprFlow = analyze(*node.getExpression());

    flow = exprFlow;
}
void ControlFlowAnalyzer::visitIfStmt(ast::IfStmtNode& node) {
    sema::FlowInfo& flow = currentFlowInfo();

    sema::FlowInfo thenFlow = analyze(*node.getThen());
    sema::FlowInfo elseFlow; // << canFallThrough is true by default

    if (node.getElse()) {
        elseFlow = analyze(*node.getElse());
    }

    flow.canFallThrough = thenFlow.canFallThrough || elseFlow.canFallThrough;
    flow.canReturn = thenFlow.canReturn || elseFlow.canReturn;
    flow.canBreak = thenFlow.canBreak || elseFlow.canBreak;
    flow.canContinue = thenFlow.canContinue || elseFlow.canContinue;
}
void ControlFlowAnalyzer::visitLoopStmt(ast::LoopStmtNode& node) {
    sema::FlowInfo& flow = currentFlowInfo();

    sema::FlowInfo bodyFlow = analyze(*node.getBody());

    flow.canReturn = bodyFlow.canReturn;

    // A break exits the loop and reaches the statement after it
    flow.canFallThrough = bodyFlow.canBreak;

    // Don't escape the loop
    flow.canBreak = false;
    flow.canContinue = false;
}
void ControlFlowAnalyzer::visitWhileStmt(ast::WhileStmtNode& node) {
    sema::FlowInfo& flow = currentFlowInfo();

    sema::FlowInfo bodyFlow = analyze(*node.getBody());

    // While loops can always fall through due to the condition
    // Later down the line, we can identify if the condition is constant
    flow.canFallThrough = true;
    flow.canReturn = bodyFlow.canReturn;
    flow.canBreak = false;
    flow.canContinue = false;
}
void ControlFlowAnalyzer::visitForStmt(ast::ForStmtNode& node) {
    sema::FlowInfo& flowInfo = currentFlowInfo();

    sema::FlowInfo bodyFlowInfo = analyze(*node.getBody());

    // For loops can always fall through due to the condition
    // Later down the line, we can identify if the condition is constant
    flowInfo.canFallThrough = true;
    flowInfo.canReturn = bodyFlowInfo.canReturn;
    flowInfo.canBreak = false;
    flowInfo.canContinue = false;
}
void ControlFlowAnalyzer::visitReturnStmt(ast::ReturnStmtNode&) {
    sema::FlowInfo& flowInfo = currentFlowInfo();

    flowInfo.canFallThrough = false;
    flowInfo.canReturn = true;
    flowInfo.canBreak = false;
    flowInfo.canContinue = false;
}

} // namespace

void ControlFlowValidationPass::visitFunctionDecl(ast::FunctionDeclNode& node) {
    ast::DeclarationNode* oldFunctionOrMethod = _currentFunctionOrMethod;
    _currentFunctionOrMethod = &node;
    
    ast::AstWalker::visitFunctionDecl(node);

	sema::FlowInfo bodyFlow = analyzeFlowInfo(*node.getBody());
	if (functionReturnsValue(node) && bodyFlow.canFallThrough) {
		_ctx.diagnostics.report(
            diagnostics::ERROR_NOT_ALL_CODE_PATHS_RETURN_VALUE,
            node.getRange()
        );
	}

    _currentFunctionOrMethod = oldFunctionOrMethod;
}
void ControlFlowValidationPass::visitMethodDecl(ast::MethodDeclNode& node) {
    ast::DeclarationNode* oldFunctionOrMethod = _currentFunctionOrMethod;
    _currentFunctionOrMethod = &node;
    
    ast::AstWalker::visitMethodDecl(node);

	sema::FlowInfo bodyFlow = analyzeFlowInfo(*node.getBody());
	if (methodReturnsValue(node) && bodyFlow.canFallThrough) {
		_ctx.diagnostics.report(
            diagnostics::ERROR_NOT_ALL_CODE_PATHS_RETURN_VALUE,
            node.getRange()
        );
	}

    _currentFunctionOrMethod = oldFunctionOrMethod;
}

void ControlFlowValidationPass::visitReturnStmt(ast::ReturnStmtNode& node) {
    ast::AstWalker::visitReturnStmt(node);

    // Outside function or method?
	if (!_currentFunctionOrMethod) {
		_ctx.diagnostics.report(
			diagnostics::ERROR_RETURN_OUTSIDE_FUNCTION_OR_METHOD,
			node.getRange()
		);
		return;
	}
}

bool ControlFlowValidationPass::functionReturnsValue(const ast::FunctionDeclNode& node) {
	return node.symbol->getReturnType() != _ctx.types.table.getBuiltin(types::BuiltinTypeKind::Void);
}
bool ControlFlowValidationPass::methodReturnsValue(const ast::MethodDeclNode& node) {
	return node.symbol->getReturnType() != _ctx.types.table.getBuiltin(types::BuiltinTypeKind::Void);
}

sema::FlowInfo ControlFlowValidationPass::analyzeFlowInfo(ast::AstNode& node) {
    static ControlFlowAnalyzer analyzer;
    return analyzer.analyze(node);
}

} // namespace sema_passes
VEEC_NAMESPACE_END
