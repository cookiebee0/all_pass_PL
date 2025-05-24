// ast.h
#pragma once
#include <string>
#include <memory>
#include <vector>

class ASTNode {
public:
    virtual ~ASTNode() = default;
};

class Number : public ASTNode {
public:
    double value;
    Number(double val) : value(val) {}
};

class Identifier : public ASTNode {
public:
    std::string name;
    Identifier(const std::string& n) : name(n) {}
};

class BinaryExpr : public ASTNode {
public:
    std::unique_ptr<ASTNode> left;
    std::string op;
    std::unique_ptr<ASTNode> right;

    BinaryExpr(std::unique_ptr<ASTNode> l, const std::string& oper, std::unique_ptr<ASTNode> r)
        : left(std::move(l)), op(oper), right(std::move(r)) {}
};

// 新增：if語句節點
class IfStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBranch;

    IfStmt(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBr)
        : condition(std::move(cond)), thenBranch(std::move(thenBr)) {}
};

// 新增：print語句節點
class PrintStmt : public ASTNode {
public:
    std::unique_ptr<ASTNode> expression;

    PrintStmt(std::unique_ptr<ASTNode> expr)
        : expression(std::move(expr)) {}
};

// 新增：區塊語句節點 (可包含多個語句)
class BlockStmt : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    BlockStmt(std::vector<std::unique_ptr<ASTNode>> stmts)
        : statements(std::move(stmts)) {}
};

class StringLiteral : public ASTNode {
public:
    std::string value;
    StringLiteral(const std::string& val) : value(val) {}
};
