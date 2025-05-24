// parser.h
#pragma once
#include <vector>
#include <memory>
#include "token.h"
#include "ast.h"

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    std::unique_ptr<ASTNode> parseExpression();
    std::unique_ptr<ASTNode> parseStatement();
    std::unique_ptr<ASTNode> parseBlock();      // ✅ 加入這行，並確保型別一致
    std::unique_ptr<ASTNode> parseProgram();    // ✅ 加入這行

private:
    const std::vector<Token>& tokens;
    size_t current = 0;

    const Token& peek() const;
    const Token& advance();
    const Token& previous() const;
    bool isAtEnd() const;
    bool match(const std::vector<TokenType>& types);

    std::unique_ptr<ASTNode> parseEquality();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseTerm();
    std::unique_ptr<ASTNode> parseFactor();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parsePrimary();
};
