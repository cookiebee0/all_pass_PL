// parser.h
#pragma once
#include <vector>
#include <memory>
#include "token.h"
#include "ast.h"

class Parser {
    const std::vector<Token>& tokens;
    size_t current;

public:
    Parser(const std::vector<Token>& toks) : tokens(toks), current(0) {}

    const Token& peek() const;
    const Token& advance();
    bool isAtEnd() const;
    const Token& previous() const;
    bool match(const std::vector<TokenType>& types);

    // 主要入口
    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseStatement();

    // 表達式相關
    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePrimary();

    // 陳述式相關
    std::unique_ptr<ASTNode> parseVarDecl();
    std::unique_ptr<ASTNode> parseIfStatement();
    std::unique_ptr<ASTNode> parseWhenStatement();
    std::vector<std::unique_ptr<ASTNode>> parseBlock();
};
