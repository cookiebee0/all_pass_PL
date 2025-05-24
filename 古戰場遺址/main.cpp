#include <iostream>
#include <vector>
#include "token.h"
#include "parser.h"
#include "lexer.h"
#include "ast.h"

// 遞迴印 AST
void printAST(const ASTNode* node, int indent = 0) {
    if (!node) return;
    std::string padding(indent, ' ');

    if (auto number = dynamic_cast<const Number*>(node)) {
        std::cout << padding << "Number: " << number->value << "\n";
    } else if (auto binary = dynamic_cast<const BinaryExpr*>(node)) {
        std::cout << padding << "BinaryExpr: " << binary->op << "\n";
        printAST(binary->left.get(), indent + 2);
        printAST(binary->right.get(), indent + 2);
    } else {
        std::cout << padding << "Unknown ASTNode\n";
    }
}

int main() {
    // std::string code = R"(1 * 1 + 1)";
    std::string code = R"(print("X is large");
)";
    std::cout << code << '\n';
    
    std::vector<Token> tokens = lexer(code);
    
    for (const auto& token : tokens) {
        std::cout << tokenTypeToString(token.type) << ": " << token.value << "\n";
    }  //
    tokens.push_back({TokenType::EOF_TOKEN, ""}); // 手動加上 EOF token
    Parser parser(tokens);

    try {
        auto ast = parser.parseExpression();
        std::cout << "Parsing succeeded!\n";
        printAST(ast.get());
    } catch (const std::exception& e) {
        std::cerr << "Parsing failed: " << e.what() << "\n";
    }

    return 0;
}
