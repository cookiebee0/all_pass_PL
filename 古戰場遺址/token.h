// token.h
#pragma once
#include <string>

enum class TokenType {
    FRACTION, NUMBER, STRING, KEYWORD, IDENTIFIER,
    OPERATOR, COMPARISON, PUNCTUATION, UNKNOWN, EOF_TOKEN
};


class Token {
public:
    TokenType type;
    std::string value;

    Token(TokenType t, const std::string& v) : type(t), value(v) {}
};
