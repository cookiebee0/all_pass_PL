// ast.h
#pragma once
#include <string>
#include <memory>

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
