#include "veec/parsing/Parser.hpp"

#include <optional>

#include "vee/core/CoreDefines.hpp"
#include "vee/core/CoreTypedefs.hpp"
#include "vee/core/InternalErrorHandling.hpp"
#include "veec/CoreDefines.hpp"
#include "veec/compilation/CompilationContext.hpp"
#include "veec/basic/StringId.hpp"
#include "veec/basic/StringPool.hpp"
#include "veec/basic/Token.hpp"
#include "veec/source/SourceManager.hpp"
#include "veec/source/Identifier.hpp"
#include "veec/source/SourceRange.hpp"
#include "veec/source/SourceLocation.hpp"
#include "veec/constants/Keywords.hpp"
#include "veec/ast/AstContext.hpp"
#include "veec/ast/AstKind.hpp"
#include "veec/ast/AstFwd.hpp"
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
#include "veec/diagnostics/DiagnosticEngine.hpp"
#include "veec/diagnostics/DiagnosticCatalog.hpp"

VEEC_NAMESPACE_BEGIN
namespace parsing {

ast::CompilationUnitNode* Parser::parse() {
    return parseCompilationUnit();
}

ast::CompilationUnitNode* Parser::parseCompilationUnit() {
    // Cant parse if there are no tokens
	if (_tokenList.tokens.empty()) {
		return nullptr;
	}

    std::vector<ast::ItemNode*> items;

    while (!isAtEnd()) {
        // Parse a top-level item
        ast::ItemNode* node = parseItem();
        if (node) {
            items.push_back(node);
        }
        else synchronize();
    }

	// If no statements were parsed, return nullptr to indicate an empty module.
    // This also avoids using statements.front() and back() which throw
    // on an empty vector
    if (items.empty()) {
        return nullptr;
    }

    source::SourceRange moduleRange = combineRanges(
        items.front()->getRange(),
        items.back()->getRange()
    );
    ast::CompilationUnitNode* compilationUnit = _ast.makeNode<ast::CompilationUnitNode>(
        moduleRange,
        std::move(items)
    );
    _ast.modules.push_back(compilationUnit);
    return compilationUnit;
}
ast::ItemNode* Parser::parseItem() {
    if (auto* decl = parseDeclaration())
        return decl;

    if (auto* stmt = parseStatement())
        return stmt;

    return nullptr;
}
ast::ItemNode* Parser::parseMemberItem() {
    if (auto* decl = parseMemberDeclaration())
        return decl;

    if (auto* stmt = parseMemberStatement())
        return stmt;

    return nullptr;
}

ast::QualifiedNameNode* Parser::parseQualifiedName() {
    std::vector<ast::QualifiedNameSegment> segments;

    const Token& beginToken = peek();

    while (!isAtEnd()) {
        ast::QualifiedNameSegment segment;

        // Identifier
        if (!match(TokenType::Identifier)) {
            _ctx.diagnostics.report(diagnostics::ERROR_IDENTIFIER_EXPECTED, peek().range());
            return nullptr;
        }
        const Token& idToken = previous();
        source::Identifier idName = tokenToIdentifier(idToken);
		segment.identifier = idName;

        // '::' Indicates either more segments or generic args
        if (!match(TokenType::ColonColon)) {
            // No more segments or generic args
            segments.push_back(segment);
            break;
        }

        if (!check(TokenType::Less)) {
            segments.push_back(segment);
            continue; // More segments, no generic args for this segment
        }

        // Generic args for *this* segment
        ast::GenericArgsNode* genericArgs = parseGenericArgs();
        segment.genericArgs = genericArgs;
        segments.push_back(segment);

        // Do we have more segments?
        if (!match(TokenType::ColonColon)) {
            break;
        }
    }

    const Token& endToken = previous();
    const source::SourceRange nameRange = tokenRange(beginToken, endToken);
    return _ast.makeNode<ast::QualifiedNameNode>(
        nameRange,              // range
        std::move(segments)     // segments
    );
}

ast::GenericArgsNode* Parser::parseGenericArgs() {
    VEE_ASSERT(match(TokenType::Less), "Expected '<' token before parsing generic arguments");
    const Token& argsBegin = previous();

    std::vector<ast::TypeNode*> args;
    do {
        ast::TypeNode* arg = parseType();
        if (!arg) {
            _ctx.diagnostics.report(diagnostics::ERROR_TYPE_EXPECTED, peek().range());
            return nullptr;
        }
        args.push_back(arg);
    } while (!isAtEnd() && match(TokenType::Comma));

    expect(TokenType::Greater, diagnostics::ERROR_GREATER_EXPECTED);

    const Token& argsEnd = previous();
    source::SourceRange argsRange = tokenRange(argsBegin, argsEnd);
    return _ast.makeNode<ast::GenericArgsNode>(
        argsRange,
        std::move(args)
    );
}

ast::DeclarationNode* Parser::parseDeclaration() {
    switch (peek().type()) {
        case TokenType::Module:
		    return parseModuleDeclaration();

        case TokenType::Func:
            return parseFunctionDeclaration();

        case TokenType::Let:
		case TokenType::Var:
			return parseVariableDeclaration();
        
		case TokenType::Class:
			return parseClassDeclaration();

        default:
            return nullptr;
    }
}
ast::DeclarationNode* Parser::parseMemberDeclaration() {
    // Essentially the same as parseDeclaration, but for
    // member declarations inside a class.

    // The main difference is fields, which do not require
    // 'let' or 'var' in a class.

    // Additionally, we dispatch 'func' to parseMethodDeclaration
    // instead of parseFunctionDeclaration.

    switch (peek().type()) {
        case TokenType::Module:
			return parseModuleDeclaration();

        case TokenType::Identifier:
            return parseFieldDeclaration();

        case TokenType::Func:
            return parseMethodDeclaration();
        
		case TokenType::Class:
			return parseClassDeclaration();

        default:
            return nullptr;
    }
}
ast::ModuleDeclNode* Parser::parseModuleDeclaration() {
    // Syntax:
    // module <identifier> {
    //   <items>
    // }

    VEE_ASSERT(match(TokenType::Module), "Expected 'module' keyword before parsing module declaration");
    const Token& moduleToken = previous();

    const Token* idToken = expect(TokenType::Identifier, diagnostics::ERROR_IDENTIFIER_EXPECTED);
    if (!idToken) {
        return nullptr;
    }

    expect(TokenType::LCurly, diagnostics::ERROR_LCURLY_EXPECTED);

    std::vector<ast::ItemNode*> items;
    while (!isAtEnd() && !check(TokenType::RCurly)) {
        ast::ItemNode* item = parseItem();
        if (item) {
            items.push_back(item);
        }
    }

    expect(TokenType::RCurly, diagnostics::ERROR_RCURLY_EXPECTED);

    source::SourceRange moduleRange = tokenRange(moduleToken, previous());
    source::Identifier moduleName = tokenToIdentifier(*idToken);
    return _ast.makeNode<ast::ModuleDeclNode>(
        moduleRange,        // range
        moduleName,         // name
        std::move(items)    // items
    );
}
ast::FunctionDeclNode* Parser::parseFunctionDeclaration() {
    // Syntax:
    // func <identifier>[<types>]([<params>]) [-> <return_type>] {
    //     <body>
    // }

    VEE_ASSERT(match(TokenType::Func), "Expected 'func' keyword before parsing function declaration");
    const Token& funcToken = previous();

    const Token* idToken = expect(TokenType::Identifier, diagnostics::ERROR_IDENTIFIER_EXPECTED);
    if (!idToken) {
        return nullptr;
    }

    ast::GenericArgsNode* genericParams = nullptr;
    if (check(TokenType::Less)) {
        genericParams = parseGenericArgs();
    }

    expect(TokenType::LParen, diagnostics::ERROR_LPAREN_EXPECTED);

    std::vector<ast::ParameterDeclNode*> parameters;
    while (!isAtEnd() && !check(TokenType::RParen)) {
        do {
            ast::ParameterDeclNode* param = parseParameterDeclaration();
            if (!param) {
                return nullptr;
            }
            parameters.push_back(param);
        } while (match(TokenType::Comma));
    }

    expect(TokenType::RParen, diagnostics::ERROR_RPAREN_EXPECTED);

    ast::TypeNode* returnType = nullptr;
    if (match(TokenType::RArrow)) {
        returnType = parseType();
        if (!returnType) {
            return nullptr;
        }
    }

    ast::BlockStmtNode* body = parseBlockStatement();
    if (!body) {
        return nullptr;
    }

    // TODO: Maybe previous() usage is bad here?
    source::SourceRange funcRange = tokenRange(funcToken, previous());
    source::Identifier funcName = tokenToIdentifier(*idToken);
    return _ast.makeNode<ast::FunctionDeclNode>(
        funcRange,                  // range
        funcName,                   // name
        genericParams,              // generic parameters
        std::move(parameters),      // parameters
        returnType,                 // return type
        body                        // body
    );
}
ast::ParameterDeclNode* Parser::parseParameterDeclaration() {
    const Token* identifier = expect(TokenType::Identifier, diagnostics::ERROR_IDENTIFIER_EXPECTED);
    if (!identifier) {
        return nullptr;
    }

    expect(TokenType::Colon, diagnostics::ERROR_COLON_EXPECTED);

    ast::TypeNode* type = parseType();
    if (!type) {
        return nullptr;
    }

    source::SourceRange paramRange = combineRanges(identifier->range(), type->getRange());
    source::Identifier paramName = tokenToIdentifier(*identifier);
    return _ast.makeNode<ast::ParameterDeclNode>(
        paramRange,         // range
        paramName,          // identifier
        type                // type
    );
}
ast::ClassDeclNode* Parser::parseClassDeclaration() {
    VEE_ASSERT(match(TokenType::Class), "Expected 'class' keyword before parsing class declaration");
    const Token& classToken = previous();

    const Token* idToken = expect(TokenType::Identifier, diagnostics::ERROR_IDENTIFIER_EXPECTED);
    if (!idToken) {
        return nullptr;
    }

    expect(TokenType::LCurly, diagnostics::ERROR_LCURLY_EXPECTED);

    std::vector<ast::ItemNode*> items;
    while (!isAtEnd() && !check(TokenType::RCurly)) {
        ast::ItemNode* item = parseMemberItem();
        if (item) {
            items.push_back(item);
        }
        else synchronize();
    }

    expect(TokenType::RCurly, diagnostics::ERROR_RCURLY_EXPECTED);

    source::SourceRange classRange = tokenRange(classToken, previous());
    source::Identifier className = tokenToIdentifier(*idToken);
    return _ast.makeNode<ast::ClassDeclNode>(
        classRange,         // range
        className,          // name
        std::move(items)    // items
    );
}
ast::MethodDeclNode* Parser::parseMethodDeclaration() {
    // Syntax:
    // func <identifier>(<parameters>) [-> <return_type>] {
    //     <body>
    // }

    VEE_ASSERT(match(TokenType::Func), "Expected 'func' keyword before parsing method declaration");
    const Token& funcToken = previous();

    const Token* idToken = expect(TokenType::Identifier, diagnostics::ERROR_IDENTIFIER_EXPECTED);
    if (!idToken) {
        return nullptr;
    }

    expect(TokenType::LParen, diagnostics::ERROR_LPAREN_EXPECTED);

    std::vector<ast::ParameterDeclNode*> parameters;
    while (!isAtEnd() && !check(TokenType::RParen)) {
        do {
            ast::ParameterDeclNode* param = parseParameterDeclaration();
            if (!param) {
                return nullptr;
            }
            parameters.push_back(param);
        } while (match(TokenType::Comma));
    }

    expect(TokenType::RParen, diagnostics::ERROR_RPAREN_EXPECTED);

    ast::TypeNode* returnType = nullptr;
    if (match(TokenType::RArrow)) {
        returnType = parseType();
        if (!returnType) {
            return nullptr;
        }
    }

    ast::BlockStmtNode* body = parseBlockStatement();
    if (!body) {
        return nullptr;
    }

    source::SourceRange methodRange = combineRanges(funcToken.range(), body->getRange());
    source::Identifier methodName = tokenToIdentifier(*idToken);
    return _ast.makeNode<ast::MethodDeclNode>(
        methodRange,                // range
        methodName,                 // name
        std::move(parameters),      // parameters
        returnType,                 // return type
        body                        // body
    );
}
ast::FieldDeclNode* Parser::parseFieldDeclaration() {
    VEE_ASSERT(match(TokenType::Identifier), "Expected identifier before parsing field declaration");
    const Token& idToken = previous();

    expect(TokenType::Colon, diagnostics::ERROR_COLON_EXPECTED);

    ast::TypeNode* type = parseType();
    if (!type) {
        return nullptr;
    }

    expect(TokenType::SemiColon, diagnostics::ERROR_SEMICOLON_EXPECTED);

    source::SourceRange fieldRange = combineRanges(idToken.range(), type->getRange());
    source::Identifier fieldName = tokenToIdentifier(idToken);
    return _ast.makeNode<ast::FieldDeclNode>(
        fieldRange,         // range
        fieldName,          // identifier
        type                // type
    );
}
ast::VariableDeclNode* Parser::parseVariableDeclaration() {
    ast::VariableDeclKind kind;
    if (match(TokenType::Var))
        kind = ast::VariableDeclKind::Var;
    else if (match(TokenType::Let))
        kind = ast::VariableDeclKind::Let;
    else
        VEE_FATAL("Expected 'var' or 'let' keyword before parsing variable declaration");

    const Token& varToken = previous();

    const Token* identifier = expect(TokenType::Identifier, diagnostics::ERROR_IDENTIFIER_EXPECTED);
    if (!identifier) {
        return nullptr;
    }

    // Explicit type
    ast::TypeNode* type = nullptr;
    if (match(TokenType::Colon)) {
        type = parseType();
        if (!type) {
            return nullptr;
        }
    }

    // Optional initializer
    ast::ExpressionNode* initializer = nullptr;
    if (match(TokenType::Equal)) {
        initializer = parseExpression();
        if (!initializer) {
            return nullptr;
        }
    }

    expect(TokenType::SemiColon, diagnostics::ERROR_SEMICOLON_EXPECTED);

    source::SourceRange varRange = tokenRange(varToken, previous());
    source::Identifier varName = tokenToIdentifier(*identifier);
    return _ast.makeNode<ast::VariableDeclNode>(
        varRange,           // range
		kind,			    // kind
        varName,            // name
        type,               // type
        initializer         // initializer
    );
}

ast::StatementNode* Parser::parseStatement() {
    switch (peek().type()) {
        case TokenType::If:
            return parseIfStatement();
        case TokenType::Loop:
            return parseLoopStatement();
        case TokenType::While:
            return parseWhileStatement();
        case TokenType::For:
            return parseForStatement();
        case TokenType::Return:
            return parseReturnStatement();
		case TokenType::Use:
			return parseUseStatement();
        case TokenType::LCurly:
            return parseBlockStatement();

        default:
            return parseExpressionStatement();
    }
}
ast::StatementNode* Parser::parseMemberStatement() {
    switch (peek().type()) {
        case TokenType::If:
            return parseIfStatement();
        case TokenType::Loop:
            return parseLoopStatement();
        case TokenType::While:
            return parseWhileStatement();
        case TokenType::For:
            return parseForStatement();
        case TokenType::Return:
            return parseReturnStatement();
		case TokenType::Use:
			return parseUseStatement();
        case TokenType::LCurly:
            return parseBlockStatement();

        default:
            return parseExpressionStatement();
    }
}
ast::BlockStmtNode* Parser::parseBlockStatement() {
    const Token* lBrace = expect(TokenType::LCurly, diagnostics::ERROR_LCURLY_EXPECTED);
    if (!lBrace) {
        return nullptr;
    }

    std::vector<ast::ItemNode*> items;
    while (!isAtEnd() && !check(TokenType::RCurly)) {
        ast::ItemNode* item = parseItem();
        if (item) {
            items.push_back(item);
        }
        else synchronize();
    }

    const Token* rBrace = expect(TokenType::RCurly, diagnostics::ERROR_RCURLY_EXPECTED);
    if (!rBrace) {
        return nullptr;
    }

    source::SourceRange blockRange = tokenRange(*lBrace, *rBrace);
    return _ast.makeNode<ast::BlockStmtNode>(
        blockRange,
        std::move(items)
    );
}
ast::ExpressionStmtNode* Parser::parseExpressionStatement() {
    ast::ExpressionNode* expr = parseExpression();
	if (!expr) {
		return nullptr;
	}

    expect(TokenType::SemiColon, diagnostics::ERROR_SEMICOLON_EXPECTED);

	return _ast.makeNode<ast::ExpressionStmtNode>(
		expr->getRange(),
		expr
	);
}
ast::IfStmtNode* Parser::parseIfStatement() {
    // Syntax:
	// if <condition> <then_branch> [else <else_branch>]

    VEE_ASSERT(match(TokenType::If), "Expected 'if' keyword before parsing if statement");
    const Token& ifToken = previous();

    ast::ExpressionNode* condition = parseExpression();
    if (!condition) {
        _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
        return nullptr;
    }

    ast::StatementNode* thenBranch = parseStatement();
    if (!thenBranch) {
        _ctx.diagnostics.report(diagnostics::ERROR_STATEMENT_EXPECTED, peek().range());
        return nullptr;
    }

    ast::StatementNode* elseBranch = nullptr;
    if (match(TokenType::Else)) {
        elseBranch = parseStatement();
        if (!elseBranch) {
            _ctx.diagnostics.report(diagnostics::ERROR_STATEMENT_EXPECTED, peek().range());
            return nullptr;
        }
    }

    source::SourceRange ifRange = tokenRange(ifToken, previous());
    return _ast.makeNode<ast::IfStmtNode>(
        ifRange,        // range
        condition,      // condition
        thenBranch,     // then branch
        elseBranch      // else branch
    );
}
ast::LoopStmtNode* Parser::parseLoopStatement() {
    // Syntax:
    // loop <body>

    VEE_ASSERT(match(TokenType::Loop), "Expected 'loop' keyword before parsing loop statement");
    const Token& loopToken = previous();

    ast::StatementNode* body = parseStatement();
    if (!body) {
        _ctx.diagnostics.report(diagnostics::ERROR_STATEMENT_EXPECTED, peek().range());
        return nullptr;
    }

    source::SourceRange loopRange = tokenRange(loopToken, previous());
    return _ast.makeNode<ast::LoopStmtNode>(
        loopRange,     // range
        body           // body
    );
}
ast::WhileStmtNode* Parser::parseWhileStatement() {
    // Syntax:
	// while <condition> <body>

	VEE_ASSERT(match(TokenType::While), "Expected 'while' keyword before parsing while statement");
	const Token& whileToken = previous();

    ast::ExpressionNode* condition = parseExpression();
    if (!condition) {
        _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
        return nullptr;
    }

    ast::StatementNode* body = parseStatement();
    if (!body) {
        _ctx.diagnostics.report(diagnostics::ERROR_STATEMENT_EXPECTED, peek().range());
        return nullptr;
    }

    source::SourceRange whileRange = tokenRange(whileToken, previous());
    return _ast.makeNode<ast::WhileStmtNode>(
        whileRange,     // range
        condition,      // condition
        body            // body
    );
}
ast::ForStmtNode* Parser::parseForStatement() {
    // Syntax:
    // for [<initializer>]; [<condition>]; [<increment>]; <body>

    VEE_ASSERT(match(TokenType::For), "Expected 'for' keyword before parsing for statement");
    const Token& forToken = previous();

    // Init
    ast::StatementNode* init = nullptr;
    if (!match(TokenType::SemiColon)) {
        init = parseStatement();
        if (!init) {
            _ctx.diagnostics.report(diagnostics::ERROR_STATEMENT_EXPECTED, peek().range());
            return nullptr;
        }
        expect(TokenType::SemiColon, diagnostics::ERROR_SEMICOLON_EXPECTED);
    }

    // Condition
    ast::ExpressionNode* condition = nullptr;
    if (!match(TokenType::SemiColon)) {
        condition = parseExpression();
        if (!condition) {
            _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
            return nullptr;
        }
        expect(TokenType::SemiColon, diagnostics::ERROR_SEMICOLON_EXPECTED);
    }

    // Increment
    ast::ExpressionNode* increment = nullptr;
    if (!match(TokenType::LCurly)) {
        increment = parseExpression();
        if (!increment) {
            _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
            return nullptr;
        }
        expect(TokenType::LCurly, diagnostics::ERROR_LCURLY_EXPECTED);
    }

    // Body
    ast::StatementNode* body = parseStatement();
    if (!body) {
        _ctx.diagnostics.report(diagnostics::ERROR_STATEMENT_EXPECTED, peek().range());
        return nullptr;
    }

    source::SourceRange forRange = tokenRange(forToken, previous());
    return _ast.makeNode<ast::ForStmtNode>(
        forRange,       // range
        init,           // init
        condition,      // condition
        increment,      // increment
        body            // body
    );
}
ast::ReturnStmtNode* Parser::parseReturnStatement() {
	VEE_ASSERT(match(TokenType::Return), "Expected 'return' keyword before parsing return statement");

	const Token& returnToken = previous();
	ast::ExpressionNode* returnValue = nullptr;
	if (!match(TokenType::SemiColon)) {
		returnValue = parseExpression();
		if (!returnValue) {
			_ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
			return nullptr;
		}
		expect(TokenType::SemiColon, diagnostics::ERROR_SEMICOLON_EXPECTED);
	}

	source::SourceRange returnRange = tokenRange(returnToken, previous());
	return _ast.makeNode<ast::ReturnStmtNode>(
		returnRange,        // range
		returnValue         // return value
	);
}
ast::UseStmtNode* Parser::parseUseStatement() {
    VEE_ASSERT(match(TokenType::Use), "Expected 'use' keyword before parsing use statement");
    const Token& useToken = previous();

    std::vector<source::Identifier> modulePath;
    std::vector<source::Identifier> imports;
    while (!isAtEnd()) {
        // MODULE PATH
        if (!match(TokenType::Identifier)) {
            _ctx.diagnostics.report(diagnostics::ERROR_MODULE_NAME_EXPECTED, peek().range());
            return nullptr;
        }
        modulePath.push_back(tokenToIdentifier(previous()));

        // identifier::
        if (!match(TokenType::ColonColon)) {
            break;
        }

        // identifier::{
        if (!match(TokenType::LCurly)) {
            continue;
        }

        // IMPORTS
        do {
            if (!match(TokenType::Identifier)) {
                _ctx.diagnostics.report(diagnostics::ERROR_IMPORT_NAME_EXPECTED, peek().range());
                return nullptr;
            }
            imports.push_back(tokenToIdentifier(previous()));
        } while (!isAtEnd() && match(TokenType::Comma));
        expect(TokenType::RCurly, diagnostics::ERROR_RCURLY_EXPECTED);
        break;
    }

    expect(TokenType::SemiColon, diagnostics::ERROR_SEMICOLON_EXPECTED);

    const Token& endToken = previous();
    source::SourceRange useRange = tokenRange(useToken, endToken);
    return _ast.makeNode<ast::UseStmtNode>(
        useRange,               // range
        std::move(modulePath),  // module path
        std::move(imports)      // imports
    );
}

ast::ExpressionNode* Parser::parseExpression() {
    return parseExpressionBP(0);
}
ast::ExpressionNode* Parser::parseExpressionBP(i32 minBP) {
    ast::ExpressionNode* lhs = parsePrefixExpression();
    if (!lhs) {
        return nullptr;
    }

    while (true) {
        const Token& opToken = peek();
        auto bp = getInfixBindingPower(opToken.type());

        if (!bp || bp->left < minBP) {
            break;
        }

        lhs = parseInfixExpression(lhs);
        if (!lhs) {
            return nullptr;
        }
    }

    return lhs;
}
ast::ExpressionNode* Parser::parsePrefixExpression() {
    using TT = TokenType;

    auto prefixBP = getPrefixBindingPower(peek().type());
    if (prefixBP) {
        return parsePrefixUnaryExpression();
    }

    switch (peek().type()) {
        case TT::Identifier:
            return parseIdentifierLikeExpression();

        case TT::IntegerLiteral:
            return parseIntLiteralExpression();
        case TT::FloatLiteral:
            return parseFloatLiteralExpression();
        case TT::StringLiteral:
            return parseStringLiteralExpression();
        case TT::True:
        case TT::False:
            return parseBoolLiteralExpression();

        case TT::LParen:
            return parseParenthesizedExpression();

        default:
            _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
            return nullptr;
    }
}
ast::ExpressionNode* Parser::parseInfixExpression(ast::ExpressionNode* lhs) {
    switch (peek().type())
    {
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::Star:
        case TokenType::Slash:
        case TokenType::Percent:
        case TokenType::EqualEqual:
        case TokenType::BangEqual:
        case TokenType::Less:
        case TokenType::LessEqual:
        case TokenType::Greater:
        case TokenType::GreaterEqual:
            return parseBinaryExpression(lhs);

        case TokenType::Equal:
        case TokenType::PlusEqual:
        case TokenType::MinusEqual:
        case TokenType::StarEqual:
        case TokenType::SlashEqual:
            return parseAssignmentExpression(lhs);

        case TokenType::LParen:
            return parseCallExpression(lhs);

        case TokenType::LBrack:
            return parseIndexExpression(lhs);

        case TokenType::Dot:
        case TokenType::RArrow:
            return parseMemberAccessExpression(lhs);

        // TODO
        /*case TokenType::Question:
            return parseTryExpression(lhs);*/

        /*case TokenType::PlusPlus:
        case TokenType::MinusMinus:
            return parsePostfixUnaryExpression(lhs);*/

        default:
            VEE_UNREACHABLE("Unexpected token type in infix expression parsing");
    }
}
ast::NameExprNode* Parser::parseNameExpression() {
    ast::QualifiedNameNode* qualifiedName = parseQualifiedName();
    if (!qualifiedName) {
        return nullptr; // Error reported by parseQualifiedName()
    }

    return _ast.makeNode<ast::NameExprNode>(
        qualifiedName->getRange(),      // range
        qualifiedName                   // qualified name
    );
}
ast::ExpressionNode* Parser::parseIdentifierLikeExpression() {
    ast::QualifiedNameNode* qualifiedName = parseQualifiedName();
    if (!qualifiedName) {
        return nullptr; // Error reported by parseQualifiedName()
    }

    // Object construction
    if (check(TokenType::LCurly)) {
        // Qualified name is a type here, direct instantiation without parseType() usage.
        ast::NamedTypeNode* typeNode = _ast.makeNode<ast::NamedTypeNode>(
            qualifiedName->getRange(),      // range
            qualifiedName                   // qualified name
        );
        return parseConstructExpression(typeNode);
    }

    // Regular name expression
    return _ast.makeNode<ast::NameExprNode>(
        qualifiedName->getRange(),      // range
        qualifiedName                   // qualified name
    );
}
ast::ExpressionNode* Parser::parseConstructExpression(ast::TypeNode* type) {
    VEE_ASSERT(match(TokenType::LCurly), "Expected '{' token before parsing construct expression");
    const Token& lBraceToken = previous();

    std::vector<ast::ExpressionNode*> args;
    while (!isAtEnd() && !check(TokenType::RCurly)) {
        ast::ExpressionNode* arg = parseExpression();
        if (!arg) {
            _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
            return nullptr;
        }
        args.push_back(arg);

        if (!match(TokenType::Comma)) {
            break; // No more arguments
        }
    }

    expect(TokenType::RCurly, diagnostics::ERROR_RCURLY_EXPECTED);
    const Token& rBraceToken = previous();

    source::SourceRange constructRange = tokenRange(lBraceToken, rBraceToken);
    return _ast.makeNode<ast::ConstructExprNode>(
        constructRange,     // range
        type,               // type
        std::move(args)     // arguments
    );
}
ast::IntLiteralExprNode* Parser::parseIntLiteralExpression() {
    VEE_ASSERT(match(TokenType::IntegerLiteral), "Expected integer literal token before parsing int literal expression");
    const Token& valueToken = previous();

    std::string_view valueStr = tokenText(valueToken);
    return _ast.makeNode<ast::IntLiteralExprNode>(
        valueToken.range(),    // range
        valueToken,            // token
        // TODO: Detect base and suffix/prefixes
        basic::BigInt::fromString(valueStr, 10, false)
    );
}
ast::FloatLiteralExprNode* Parser::parseFloatLiteralExpression() {
    VEE_ASSERT(match(TokenType::FloatLiteral), "Expected float literal token before parsing float literal expression");
    const Token& valueToken = previous();

    std::string valueStr = std::string(tokenText(valueToken));
    return _ast.makeNode<ast::FloatLiteralExprNode>(
        valueToken.range(),    // range
        valueToken,            // token
        std::stod(valueStr)
    );
}
ast::StringLiteralExprNode* Parser::parseStringLiteralExpression() {
    VEE_ASSERT(match(TokenType::StringLiteral), "Expected string literal token before parsing string literal expression");
    const Token& valueToken = previous();

    std::string_view valueStr = tokenText(valueToken);
    return _ast.makeNode<ast::StringLiteralExprNode>(
        valueToken.range(),    // range
        valueToken,            // token
        std::string(valueStr)
    );
}
ast::BoolLiteralExprNode* Parser::parseBoolLiteralExpression() {
    VEE_ASSERT(match(TokenType::True) || match(TokenType::False), "Expected boolean literal token before parsing bool literal expression");
    const Token& valueToken = previous();

    bool value = (valueToken.type() == TokenType::True);
    return _ast.makeNode<ast::BoolLiteralExprNode>(
        valueToken.range(),    // range
        valueToken,            // token
        value
    );
}
ast::ExpressionNode* Parser::parsePrefixUnaryExpression() {
    const Token& opToken = advance();

    auto bp = getPrefixBindingPower(opToken.type());
    VEE_ASSERT(bp.has_value(), "Expected binding power for unary operator");

    ast::ExpressionNode* operand = parseExpressionBP(*bp);
    if (!operand) {
        _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
        return nullptr;
    }

    ast::UnaryOp op = tokenTypeToUnaryOp(opToken.type());

    source::SourceRange range = combineRanges(opToken.range(), operand->getRange());
    return _ast.makeNode<ast::UnaryExprNode>(
        range,          // range
        operand,        // operand
        op              // operator
    );
}
ast::ExpressionNode* Parser::parsePostfixUnaryExpression(ast::ExpressionNode*) {
    VEE_FATAL("Unsupported!");
}
ast::ExpressionNode* Parser::parseBinaryExpression(ast::ExpressionNode* lhs) {
    const Token& opToken = advance();

    auto bp = getInfixBindingPower(opToken.type());
    VEE_ASSERT(bp.has_value(), "Expected binding power for binary operator");

    ast::ExpressionNode* rhs = parseExpressionBP(bp->right);
    if (!rhs) {
        _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
        return nullptr;
    }

    ast::BinaryOp op = tokenTypeToBinaryOp(opToken.type());

    source::SourceRange range = combineRanges(lhs->getRange(), rhs->getRange());
    return _ast.makeNode<ast::BinaryExprNode>(
        range,          // range
        lhs,            // left-hand side
        rhs,            // right-hand side
        op              // operator
    );
}
ast::ExpressionNode* Parser::parseAssignmentExpression(ast::ExpressionNode* lhs) {
    const Token& opToken = advance();

    auto bp = getInfixBindingPower(opToken.type());
    VEE_ASSERT(bp.has_value(), "Expected binding power for assignment operator");

    ast::ExpressionNode* rhs = parseExpressionBP(bp->right);
    if (!rhs) {
        _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
        return nullptr;
    }

    ast::AssignmentOp op = tokenTypeToAssignmentOp(opToken.type());

    source::SourceRange range = combineRanges(lhs->getRange(), rhs->getRange());
    return _ast.makeNode<ast::AssignmentExprNode>(
        range,          // range
        lhs,            // left-hand side
        rhs,            // right-hand side
        op              // operator
    );
}
ast::ExpressionNode* Parser::parseCallExpression(ast::ExpressionNode* callee) {
    // Syntax:
    //
    // <callee>(<args>)

    VEE_ASSERT(match(TokenType::LParen), "Expected '(' token before parsing call expression");

    std::vector<ast::ExpressionNode*> args;
    while (!isAtEnd() && !check(TokenType::RParen)) {
        ast::ExpressionNode* arg = parseExpression();
        if (!arg) {
            _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
            return nullptr;
        }
        args.push_back(arg);

        if (!match(TokenType::Comma)) {
            break; // No more arguments
        }
    }

    expect(TokenType::RParen, diagnostics::ERROR_RPAREN_EXPECTED);
    const Token& rParenToken = previous();

    source::SourceRange callRange = combineRanges(callee->getRange(), rParenToken.range());
    return _ast.makeNode<ast::CallExprNode>(
        callRange,      // range
        callee,         // callee
        args            // args
    );
}
ast::ExpressionNode* Parser::parseIndexExpression(ast::ExpressionNode* object) {
    // Syntax:
    //
    // <object>[<index>]

    VEE_ASSERT(match(TokenType::LBrack), "Expected '[' token before parsing index expression");

    ast::ExpressionNode* index = parseExpression();
    if (!index) {
        _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
        return nullptr;
    }

    expect(TokenType::RBrack, diagnostics::ERROR_RBRACK_EXPECTED);
    const Token& rBrackToken = previous();
    
    source::SourceRange indexRange = combineRanges(object->getRange(), rBrackToken.range());
    return _ast.makeNode<ast::IndexExprNode>(
        indexRange,      // range
        object,          // object
        index            // index
    );
}
ast::MemberAccessExprNode* Parser::parseMemberAccessExpression(ast::ExpressionNode* object) {
    VEE_ASSERT(matchAny(TokenType::Dot, TokenType::RArrow),
        "Expected '.' or '->' token before parsing member access expression");

    // Operator
    const Token& opToken = previous();
    ast::MemberAccessOp op = tokenTypeToMemberAccessOp(opToken.type());

    // Member name
    if (!match(TokenType::Identifier)) {
        _ctx.diagnostics.report(diagnostics::ERROR_MEMBER_NAME_EXPECTED, peek().range());
        return nullptr;
    }
    const Token& memberToken = previous();
    source::Identifier memberName = tokenToIdentifier(memberToken);

    source::SourceRange maRange = combineRanges(object->getRange(), memberToken.range());
    return _ast.makeNode<ast::MemberAccessExprNode>(
        maRange,        // range
        object,         // object
        memberName,     // member name
        op              // operator
    );
}
ast::ParenthesizedExprNode* Parser::parseParenthesizedExpression() {
    VEE_ASSERT(match(TokenType::LParen), "Expected '(' token before parsing parenthesized expression");
    const Token& lParenToken = previous();

    ast::ExpressionNode* expr = parseExpression();
    if (!expr) {
        _ctx.diagnostics.report(diagnostics::ERROR_EXPRESSION_EXPECTED, peek().range());
        return nullptr;
    }

    expect(TokenType::RParen, diagnostics::ERROR_RPAREN_EXPECTED);
    const Token& rParenToken = previous();

    source::SourceRange parenRange = tokenRange(lParenToken, rParenToken);
    return _ast.makeNode<ast::ParenthesizedExprNode>(
        parenRange,     // range
        expr            // inner expression
    );
}

ast::TypeNode* Parser::parseType() {
    // Builtin type
    if (peek().isTypeSpecifier()) {
        const Token& typeToken = advance();
        ast::BuiltinTypeKind builtinType = tokenTypeToBuiltinType(typeToken.type());

        return _ast.makeNode<ast::BuiltinTypeNode>(
            typeToken.range(),      // range
            builtinType             // built-in type
        );
    }

    // Named type
    ast::QualifiedNameNode* qualifiedName = parseQualifiedName();
    if (!qualifiedName) {
        _ctx.diagnostics.report(diagnostics::ERROR_TYPE_EXPECTED, peek().range());
        return nullptr;
    }

    return _ast.makeNode<ast::NamedTypeNode>(
        qualifiedName->getRange(),  // range
        qualifiedName               // qualified name
    );
}

ast::UnaryOp Parser::tokenTypeToUnaryOp(TokenType type) const {
    using TT = TokenType;
    using UnOp = ast::UnaryOp;

    switch (type) {
        case TT::Plus:          return UnOp::Plus;
        case TT::Minus:         return UnOp::Minus;
        case TT::PlusPlus:      return UnOp::Increment;
        case TT::MinusMinus:    return UnOp::Decrement;
        case TT::Bang:          return UnOp::LogicalNot;
        case TT::Tilde:         return UnOp::BitwiseNot;
        case TT::Star:          return UnOp::Dereference;
        case TT::Ampersand:     return UnOp::AddressOf;

        default:
            VEE_FATAL("Invalid token type for unary operator conversion");
    }
}
ast::BinaryOp Parser::tokenTypeToBinaryOp(TokenType type) const {
    using TT = TokenType;
    using BinOp = ast::BinaryOp;

    switch (type) {
        case TT::Plus:          return BinOp::Add;
        case TT::Minus:         return BinOp::Subtract;
        case TT::Star:          return BinOp::Multiply;
        case TT::Slash:         return BinOp::Divide;
        case TT::Percent:       return BinOp::Modulo;
        case TT::EqualEqual:    return BinOp::Equal;
        case TT::BangEqual:     return BinOp::NotEqual;
        case TT::Less:          return BinOp::LessThan;
        case TT::LessEqual:     return BinOp::LessThanOrEqual;
        case TT::Greater:       return BinOp::GreaterThan;
        case TT::GreaterEqual:  return BinOp::GreaterThanOrEqual;

        default:
            VEE_FATAL("Invalid token type for binary operator conversion");
    }
}
ast::AssignmentOp Parser::tokenTypeToAssignmentOp(TokenType type) const {
    using TT = TokenType;
    using AssignOp = ast::AssignmentOp;

    switch (type) {
        case TT::Equal:         return AssignOp::Assign;
        case TT::PlusEqual:     return AssignOp::AddAssign;
        case TT::MinusEqual:    return AssignOp::SubtractAssign;
        case TT::StarEqual:     return AssignOp::MultiplyAssign;
        case TT::SlashEqual:    return AssignOp::DivideAssign;

        default:
            VEE_FATAL("Invalid token type for assignment operator conversion");
    }
}
ast::MemberAccessOp Parser::tokenTypeToMemberAccessOp(TokenType type) const {
    using TT = TokenType;
    using MAOp = ast::MemberAccessOp;

    switch (type) {
        case TT::Dot:       return MAOp::Dot;
        case TT::RArrow:    return MAOp::Arrow;

        default:
            VEE_FATAL("Invalid token type for member access operator conversion");
    }
}
ast::BuiltinTypeKind Parser::tokenTypeToBuiltinType(TokenType type) const {
    using TT = TokenType;
    using BTK = ast::BuiltinTypeKind;

    switch (type) {
        case TT::I8:  return BTK::I8;
        case TT::U8:  return BTK::U8;
        case TT::I16: return BTK::I16;
        case TT::U16: return BTK::U16;
        case TT::I32: return BTK::I32;
        case TT::U32: return BTK::U32;
        case TT::I64: return BTK::I64;
        case TT::U64: return BTK::U64;
        case TT::F32: return BTK::F32;
        case TT::F64: return BTK::F64;

        default:
            VEE_FATAL("Invalid token type for built-in type conversion");
    }
}

std::optional<u8> Parser::getPrefixBindingPower(TokenType type) const {
    switch (type) {
        
        // Unary operators
        case TokenType::Plus:
        case TokenType::Minus:
        case TokenType::PlusPlus:
        case TokenType::MinusMinus:
        case TokenType::Bang:
        case TokenType::Tilde:
        case TokenType::Star:       // dereference
        case TokenType::Ampersand:  // reference/address-of
            return (u8)95;


        default:
            return std::nullopt;
    }
}
std::optional<Parser::BindingPower> Parser::getInfixBindingPower(TokenType type) const {
    switch (type) {

        // Assignment (right associative)
        case TokenType::Equal:
        case TokenType::PlusEqual:
        case TokenType::MinusEqual:
        case TokenType::StarEqual:
        case TokenType::SlashEqual:
            return BindingPower{ 10, 10 };


        // Logical OR
        case TokenType::Pipe:
            return BindingPower{ 20, 21 };


        // Logical AND
        case TokenType::Ampersand:
            return BindingPower{ 30, 31 };


        // Bitwise XOR
        case TokenType::Caret:
            return BindingPower{ 40, 41 };


        // Equality
        case TokenType::EqualEqual:
        case TokenType::BangEqual:
            return BindingPower{ 50, 51 };


        // Comparison
        case TokenType::Less:
        case TokenType::LessEqual:
        case TokenType::Greater:
        case TokenType::GreaterEqual:
            return BindingPower{ 60, 61 };


        // Additive
        case TokenType::Plus:
        case TokenType::Minus:
            return BindingPower{ 70, 71 };


        // Multiplicative
        case TokenType::Star:
        case TokenType::Slash:
        case TokenType::Percent:
            return BindingPower{ 80, 81 };


        // Exponentiation (right associative)
        case TokenType::StarStar:
            return BindingPower{ 90, 90 };


        // Postfix operators
        case TokenType::LParen:     // function call
        case TokenType::LBrack:     // indexing
        case TokenType::Dot:        // member access
        case TokenType::RArrow:     // pointer member access
        case TokenType::PlusPlus:   // postfix increment
        case TokenType::MinusMinus: // postfix decrement
            return BindingPower{ 100, 101 };


        default:
            return std::nullopt;
    }
}

} // namespace parsing
VEEC_NAMESPACE_END
