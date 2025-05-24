#ifndef LEXER_H
#define LEXER_H

#pragma once
#include <string>
#include <vector>
#include "token.h"
// using namespace std;
// enum class TokenType {
//     FRACTION, NUMBER, STRING, KEYWORD, IDENTIFIER,
//     OPERATOR, COMPARISON, PUNCTUATION, UNKNOWN, EOF_TOKEN
// };

// struct Token {
//     TokenType type;
//     std::string value;
// };

// 詞法分析函式：輸入原始碼字串，輸出 Token 向量
std::vector<Token> lexer(const std::string& code);

// 將 TokenType 轉成字串（用於 debug）
std::string tokenTypeToString(TokenType type);

#endif // LEXER_H
