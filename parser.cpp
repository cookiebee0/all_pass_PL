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
            // std::cout << "[match] token consumed: " << previous().value << "\n"; // debug
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
            // UnaryExpression 可自己定義AST類別
            // 這裡簡單示範用BinaryExpression表達(不完美)
            return std::make_unique<BinaryExpr>(nullptr, op, std::move(right));
        }
    }
    return parsePrimary();
}

std::unique_ptr<ASTNode> Parser::parsePrimary() {
    if (match({TokenType::NUMBER})) {
        return std::make_unique<Number>(std::stod(previous().value));
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
