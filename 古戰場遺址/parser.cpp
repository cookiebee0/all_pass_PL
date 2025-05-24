// parser.cpp
#include "parser.h"
#include <stdexcept>
#include <iostream> // debug

const Token& Parser::peek() const {
    return tokens.at(current);
}

const Token& Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() const {
    return current >= tokens.size();
}

const Token& Parser::previous() const {
    return tokens.at(current - 1);
}

bool Parser::match(const std::vector<TokenType>& types) {
    if (isAtEnd()) return false;
    for (auto type : types) {
        if (peek().type == type) {
            advance();
            return true;
        }
    }
    return false;
}

std::unique_ptr<ASTNode> Parser::parseExpression() {
    return parseEquality();
}

std::unique_ptr<ASTNode> Parser::parseEquality() {
    auto node = parseComparison();

    while (match({TokenType::COMPARISON})) {
        std::string op = previous().value;
        auto right = parseComparison();
        node = std::make_unique<BinaryExpr>(std::move(node), op, std::move(right));
    }

    return node;
}

std::unique_ptr<ASTNode> Parser::parseComparison() {
    auto node = parseTerm();

    while (match({TokenType::OPERATOR})) {
        std::string op = previous().value;
        if (op != "<" && op != "<=" && op != ">" && op != ">=") break;

        auto right = parseTerm();
        node = std::make_unique<BinaryExpr>(std::move(node), op, std::move(right));
    }

    return node;
}

std::unique_ptr<ASTNode> Parser::parseTerm() {
    auto node = parseFactor();

    while (!isAtEnd() && peek().type == TokenType::OPERATOR &&
           (peek().value == "+" || peek().value == "-")) {
        std::string op = advance().value;
        auto right = parseFactor();
        node = std::make_unique<BinaryExpr>(std::move(node), op, std::move(right));
    }

    return node;
}

std::unique_ptr<ASTNode> Parser::parseFactor() {
    auto node = parseUnary();

    while (!isAtEnd() && peek().type == TokenType::OPERATOR &&
           (peek().value == "*" || peek().value == "/")) {
        std::string op = advance().value;
        auto right = parseUnary();
        node = std::make_unique<BinaryExpr>(std::move(node), op, std::move(right));
    }

    return node;
}

std::unique_ptr<ASTNode> Parser::parseUnary() {
    if (match({TokenType::OPERATOR})) {
        std::string op = previous().value;
        if (op == "!" || op == "-") {
            auto right = parseUnary();
            return std::make_unique<BinaryExpr>(nullptr, op, std::move(right));
        }
    }
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    std::cout << "[DEBUG] parsePrimary token: " << peek().value << " type: " << static_cast<int>(peek().type) << "\n";
    if (match({TokenType::NUMBER})) {
        return std::make_unique<Number>(std::stod(previous().value));
    }
    if (match({TokenType::STRING})) {
        std::string raw = previous().value;
        // 去掉首尾雙引號
        if (raw.size() >= 2 && raw.front() == '"' && raw.back() == '"') {
            raw = raw.substr(1, raw.size() - 2);
        }
        return std::make_unique<StringLiteral>(raw);
    }
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<Identifier>(previous().value);
    }
    if (match({TokenType::PUNCTUATION}) && previous().value == "(") {
        auto expr = parseExpression();
        if (!match({TokenType::PUNCTUATION}) || previous().value != ")") {
            throw std::runtime_error("Expected ')' after expression.");
        }
        return expr;
    }
    throw std::runtime_error("Expected expression.");
}


// === 新增支援 if 語句與 print 語句的函式 ===
std::unique_ptr<ASTNode> Parser::parseStatement() {
    std::cout << "[DEBUG] current token: " << peek().value << "\n";
    if (match({TokenType::KEYWORD}) && previous().value == "if") {
        if (!match({TokenType::PUNCTUATION}) || previous().value != "(") {
            throw std::runtime_error("Expected '(' after 'if'");
        }
        auto condition = parseExpression();
        if (!match({TokenType::PUNCTUATION}) || previous().value != ")") {
            throw std::runtime_error("Expected ')' after condition");
        }
        std::unique_ptr<ASTNode> thenBranch;
        if (peek().type == TokenType::PUNCTUATION && peek().value == "{") {
            thenBranch = parseBlock();
        } else {
            thenBranch = parseStatement();  // 單句 statement
        }

        return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch));
    }

    if (match({TokenType::KEYWORD}) && previous().value == "print") {
        if (!match({TokenType::PUNCTUATION}) || previous().value != "(") {
            throw std::runtime_error("Expected '(' after 'print'");
        }
        auto value = parseExpression();
        if (!match({TokenType::PUNCTUATION}) || previous().value != ")") {
            throw std::runtime_error("Expected ')' after expression");
        }
        if (!match({TokenType::PUNCTUATION}) || previous().value != ";") {
            throw std::runtime_error("Expected ';' after print statement");
        }
        return std::make_unique<PrintStmt>(std::move(value));
    }

    // default: expression statement
    auto expr = parseExpression();
    if (!match({TokenType::PUNCTUATION}) || previous().value != ";") {
        throw std::runtime_error("Expected ';' after expression statement");
    }
    return expr;
}

std::unique_ptr<ASTNode> Parser::parseBlock() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    if (!match({TokenType::PUNCTUATION}) || previous().value != "{") {
        throw std::runtime_error("Expected '{' to start block");
    }
    while (!isAtEnd() && !(peek().type == TokenType::PUNCTUATION && peek().value == "}")) {
        statements.push_back(parseStatement());
    }
    if (!match({TokenType::PUNCTUATION}) || previous().value != "}") {
        throw std::runtime_error("Expected '}' to close block");
    }
    return std::make_unique<BlockStmt>(std::move(statements));
}

std::unique_ptr<ASTNode> Parser::parseProgram() {
    std::vector<std::unique_ptr<ASTNode>> statements;
    while (!isAtEnd()) {
        statements.push_back(parseStatement());
    }
    return std::make_unique<BlockStmt>(std::move(statements));
}
